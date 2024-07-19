//
// low-level driver routines for 16550a UART.
//

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"

// the UART control registers are memory-mapped
// at address UART0. this macro returns the
// address of one of the registers.
// UART(通用异步接收器发送器)的地址从0x10000000或UART0开始，每个UART控制寄存器的大小为1字节
#define Reg(reg) ((volatile unsigned char *)(UART0 + reg))

// the UART control registers.
// some have different meanings for
// read vs write.
// see http://byterunner.com/16550.html
// 当RHR中的一个字节被读取时，UART硬件将其从内部的FIFO硬盘中删除，当FIFO中为空时，LSR寄存器被置0
#define RHR 0 // receive holding register (for input bytes) 用来放置可以被用户进程读取的字节
#define THR 0 // transmit holding register (for output bytes) 当用户进程向THR写入一个字节时，UART将传输这个字节
#define IER 1 // interrupt enable register
#define IER_RX_ENABLE (1 << 0)
#define IER_TX_ENABLE (1 << 1)
#define FCR 2 // FIFO control register
#define FCR_FIFO_ENABLE (1 << 0)
#define FCR_FIFO_CLEAR (3 << 1) // clear the content of the two FIFOs
#define ISR 2                   // interrupt status register
#define LCR 3                   // line control register
#define LCR_EIGHT_BITS (3 << 0)
#define LCR_BAUD_LATCH (1 << 7) // special mode to set baud rate
#define LSR 5                   // line status register 指示输入的字节是否准备好被用户进程读取
#define LSR_RX_READY (1 << 0)   // input is waiting to be read from RHR
#define LSR_TX_IDLE (1 << 5)    // THR can accept another character to send

#define ReadReg(reg) (*(Reg(reg)))
#define WriteReg(reg, v) (*(Reg(reg)) = (v))

// the transmit output buffer.
struct spinlock uart_tx_lock;
#define UART_TX_BUF_SIZE 32
char uart_tx_buf[UART_TX_BUF_SIZE];
uint64 uart_tx_w; // write next to uart_tx_buf[uart_tx_w % UART_TX_BUF_SIZE]
uint64 uart_tx_r; // read next from uart_tx_buf[uart_tx_r % UART_TX_BUF_SIZE]

extern volatile int panicked; // from printf.c

void uartstart();

void uartinit(void)
{
  // disable interrupts.
  WriteReg(IER, 0x00);

  // special mode to set baud rate.
  WriteReg(LCR, LCR_BAUD_LATCH);

  // LSB for baud rate of 38.4K.
  WriteReg(0, 0x03);

  // MSB for baud rate of 38.4K.
  WriteReg(1, 0x00);

  // leave set-baud mode,
  // and set word length to 8 bits, no parity.
  WriteReg(LCR, LCR_EIGHT_BITS);

  // reset and enable FIFOs.
  WriteReg(FCR, FCR_FIFO_ENABLE | FCR_FIFO_CLEAR);

  // enable transmit and receive interrupts.
  WriteReg(IER, IER_TX_ENABLE | IER_RX_ENABLE);

  initlock(&uart_tx_lock, "uart");
}

// add a character to the output buffer and tell the
// UART to start sending if it isn't already.
// blocks if the output buffer is full.
// because it may block, it can't be called
// from interrupts; it's only suitable for use
// by write().
void uartputc(int c)
{
  acquire(&uart_tx_lock);

  if (panicked)
  {
    for (;;)
      ;
  }
  while (uart_tx_w == uart_tx_r + UART_TX_BUF_SIZE)
  {
    // buffer is full.
    // wait for uartstart() to open up space in the buffer.
    sleep(&uart_tx_r, &uart_tx_lock);
  }
  uart_tx_buf[uart_tx_w % UART_TX_BUF_SIZE] = c;
  uart_tx_w += 1;
  uartstart();
  release(&uart_tx_lock);
}

// alternate version of uartputc() that doesn't
// use interrupts, for use by kernel printf() and
// to echo characters. it spins waiting for the uart's
// output register to be empty.
void uartputc_sync(int c)
{
  push_off();

  if (panicked)
  {
    for (;;)
      ;
  }

  // wait for Transmit Holding Empty to be set in LSR.
  while ((ReadReg(LSR) & LSR_TX_IDLE) == 0)
    ;
  WriteReg(THR, c);

  pop_off();
}

// if the UART is idle, and a character is waiting
// in the transmit buffer, send it.
// caller must hold uart_tx_lock.
// called from both the top- and bottom-half.
void uartstart()
{
  while (1)
  {
    if (uart_tx_w == uart_tx_r)
    {
      // transmit buffer is empty.
      return;
    }

    if ((ReadReg(LSR) & LSR_TX_IDLE) == 0)
    {
      // the UART transmit holding register is full,
      // so we cannot give it another byte.
      // it will interrupt when it's ready for a new byte.
      return;
    }

    int c = uart_tx_buf[uart_tx_r % UART_TX_BUF_SIZE];
    uart_tx_r += 1;

    // maybe uartputc() is waiting for space in the buffer.
    wakeup(&uart_tx_r);

    WriteReg(THR, c);
  }
}

// read one input character from the UART.
// return -1 if none is waiting.
int uartgetc(void)
{
  if (ReadReg(LSR) & 0x01)
  {
    // input data is ready.
    return ReadReg(RHR);
  }
  else
  {
    return -1;
  }
}

// handle a uart interrupt, raised because input has
// arrived, or the uart is ready for more output, or
// both. called from devintr().
// uartintr将调用uartstart来判断设备是否确实已经完成发送，然后将下一个需要发送的字符发送给UART。
// 因此让UART传送多个字符时，第一个字符由uartputc对uartstart的调用传送，后面的字符由uartintr对uartstart的调用进行传送
//
// 每当UART发送完一个字节，它就会产生一个中断。uartintr调用uartstart，检查设备是否真的完成了发送，并将下一个缓冲的输出字符交给设备。
// 因此，如果一个进程向控制台写入多个字节，通常第一个字节将由uartputc调用uartstart发送，而剩余的缓冲字节将由uartintr调用uartstart发送，直到传输完成中断到来。
void uartintr(void)
{
  // read and process incoming characters.
  while (1)
  {
    int c = uartgetc();
    if (c == -1)
      break;
    consoleintr(c);
  }

  // send buffered characters.
  acquire(&uart_tx_lock);
  uartstart();
  release(&uart_tx_lock);
}
