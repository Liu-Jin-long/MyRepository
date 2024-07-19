// Saved registers for kernel context switches.
struct context {
  uint64 ra;//返回地址寄存器，保存函数返回地址
  uint64 sp;//堆栈指针寄存器，指向当前的堆栈顶部

  // callee-saved  被调用者保存寄存器 
  // 函数调用过程中，被调用函数callee有责任保存和恢复调用者caller的寄存器，以保护调用函数的上下文环境
  uint64 s0;
  uint64 s1;
  uint64 s2;
  uint64 s3;
  uint64 s4;
  uint64 s5;
  uint64 s6;
  uint64 s7;
  uint64 s8;
  uint64 s9;
  uint64 s10;
  uint64 s11;
};

// Per-CPU state.
struct cpu {
  struct proc *proc;          // The process running on this cpu, or null.
  //当这个CPU需要切换到调度器（scheduler）时，就会在这个context中保存当前的寄存器状态。
  // 调度器线程运行在CPU对应的bootstack上，即启动步骤过程中start.s在这个区域创建的栈
  struct context context;     // swtch() here to enter scheduler(). 保存每个CPU上特殊线程scheduler()调度器线程的上下文
  int noff;                   // Depth of push_off() nesting. push_off(用于关闭中断)函数的嵌套深度 
  //调用push_off()之前，中断是否是开启的  用于恢复中断状态
  int intena;                 // Were interrupts enabled before push_off()?  
};

extern struct cpu cpus[NCPU];

// per-process data for the trap handling code in trampoline.S.
// sits in a page by itself just under the trampoline page in the
// user page table. not specially mapped in the kernel page table.
// uservec in trampoline.S saves user registers in the trapframe,
// then initializes registers from the trapframe's
// kernel_sp, kernel_hartid, kernel_satp, and jumps to kernel_trap.
// usertrapret() and userret in trampoline.S set up
// the trapframe's kernel_*, restore user registers from the
// trapframe, switch to the user page table, and enter user space.
// the trapframe includes callee-saved user registers like s0-s11 because the
// return-to-user path via usertrapret() doesn't return through
// the entire kernel call stack.
// 保存进程在陷入（trap）时的状态，包括各种寄存器的值。
// 陷入是指当一个进程由于某些原因（例如，执行了一个系统调用，或者发生了一个异常）需要切换到内核模式时的情况。
struct trapframe {
  /*   0 */ uint64 kernel_satp;   // kernel page table 内核页表的地址
  /*   8 */ uint64 kernel_sp;     // top of process's kernel stack 进程的内核栈顶地址
  /*  16 */ uint64 kernel_trap;   // usertrap()的地址，用于处理用户模式的陷入
  /*  24 */ uint64 epc;           // saved user program counter 保存的用户程序计数器，这是陷入发生时的下一条指令的地址
  /*  32 */ uint64 kernel_hartid; // saved kernel tp 保存的内核线程指针
  /*  40 */ uint64 ra; // 返回地址寄存器
  /*  48 */ uint64 sp; // 栈指针寄存器
  /*  56 */ uint64 gp; // 全局指针寄存器
  /*  64 */ uint64 tp; // 线程指针寄存器
  /*  72 */ uint64 t0; // t0到t6是临时寄存器
  /*  80 */ uint64 t1;
  /*  88 */ uint64 t2;
  /*  96 */ uint64 s0; // s0到s11是保存寄存器，这些寄存器在函数调用过程中需要被保存和恢复。
  /* 104 */ uint64 s1;
  /* 112 */ uint64 a0; // a0到a7是参数寄存器，用于传递函数参数。
  /* 120 */ uint64 a1;
  /* 128 */ uint64 a2;
  /* 136 */ uint64 a3;
  /* 144 */ uint64 a4;
  /* 152 */ uint64 a5;
  /* 160 */ uint64 a6;
  /* 168 */ uint64 a7;
  /* 176 */ uint64 s2;
  /* 184 */ uint64 s3;
  /* 192 */ uint64 s4;
  /* 200 */ uint64 s5;
  /* 208 */ uint64 s6;
  /* 216 */ uint64 s7;
  /* 224 */ uint64 s8;
  /* 232 */ uint64 s9;
  /* 240 */ uint64 s10;
  /* 248 */ uint64 s11;
  /* 256 */ uint64 t3;
  /* 264 */ uint64 t4;
  /* 272 */ uint64 t5;
  /* 280 */ uint64 t6;
};

enum procstate { UNUSED, USED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

// Per-process state
struct proc {
  struct spinlock lock; // 用于同步的自旋锁，确保对进程状态的某些部分进行互斥访问

  // p->lock must be held when using these:
  enum procstate state;        // Process state
  //channel用于表示在并发编程中用于通信或同步的抽象概念。是一种用于进程间同步的机制，就像一个通道一样，进程可以通过它等待事件或资源
  void *chan;                  // If non-zero, sleeping on chan  类似条件变量 Xv6通常使用等待过程中涉及的内核数据结构的地址
  int killed;                  // If non-zero, have been killed
  int xstate;                  // Exit status to be returned to parent's wait
  int pid;                     // Process ID

  // wait_lock must be held when using this:
  struct proc *parent;         // Parent process 指向父进程的指针，在进程层次结构中用于维护进程树

  // these are private to the process, so p->lock need not be held.
  uint64 kstack;               // Virtual address of kernel stack 进程的内核栈的虚拟地址
  uint64 sz;                   // Size of process memory (bytes) 进程内存的大小 同时也是进程地址空间中堆顶地址 起始时sz=栈底=堆底=堆顶
  pagetable_t pagetable;       // User page table 进程的用户页表，用于管理虚拟内存
  struct trapframe *trapframe; // data page for trampoline.S trampoline.S的数据页，通常用于处理陷阱和异常
  // 在内核线程和调度器线程之间切换时，需要保存和恢复的数据
  struct context context;      // swtch() here to run process 与用户线程对应的内核线程的上下文，用于保存内核线程的执行状态。当需要切换到这个进程时，会使用switch()函数来恢复这个上下文。
  // 进程级-文件描述符表 (file descriptor table)
  struct file *ofile[NOFILE];  // Open files 文件指针数组，表示进程打开的文件
  struct inode *cwd;           // Current directory 当前目录的inode
  char name[16];               // Process name (debugging)
};
