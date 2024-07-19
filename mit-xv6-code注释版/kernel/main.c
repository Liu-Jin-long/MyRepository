#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

volatile static int started = 0;

// start() jumps here in supervisor mode on all CPUs.
// 内核0地址处
void main()
{
  if (cpuid() == 0)
  {
    consoleinit();
    printfinit();
    printf("\n");
    printf("xv6 kernel is booting\n");
    printf("\n");
    // 初始化页分配器
    kinit(); // physical page allocator
    // 创建内核页表
    kvminit(); // create kernel page table
    // 安装内核页表，将根页表页的物理地址写入寄存器satp
    // CPU开始使用内核页表转换地址。由于内核使用恒等映射，下一条指令的当前虚拟地址将映射到正确的物理内存地址
    kvminithart(); // turn on paging
    // 将各个内核栈分配给每个进程，将每个栈映射到KSTACK生成的虚拟地址，并为invalid的栈保护页面留下空间
    procinit();         // process table
    trapinit();         // trap vectors
    trapinithart();     // install kernel trap vector
    plicinit();         // set up interrupt controller
    plicinithart();     // ask PLIC for device interrupts
    binit();            // buffer cache
    iinit();            // inode table
    fileinit();         // file table
    virtio_disk_init(); // emulated hard disk
    userinit();         // first user process
    __sync_synchronize();
    started = 1;
  }
  else
  {
    while (started == 0)
      ;
    __sync_synchronize();
    printf("hart %d starting\n", cpuid());
    kvminithart();  // turn on paging
    trapinithart(); // install kernel trap vector
    plicinithart(); // ask PLIC for device interrupts  每个CPU核心启用和关注uart和virtio disk外设中断
  }

  scheduler();
}
