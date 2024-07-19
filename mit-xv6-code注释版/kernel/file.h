struct file
{
  enum
  {
    FD_NONE,
    FD_PIPE,
    FD_INODE,
    FD_DEVICE
  } type;
  int ref; // reference count
  char readable;
  char writable;
  struct pipe *pipe; // FD_PIPE
  struct inode *ip;  // FD_INODE and FD_DEVICE
  uint off;          // FD_INODE
  short major;       // FD_DEVICE
};

// 在Unix和类Unix系统中，设备通常由两个数字来标识：主设备号(major number)和次设备号(minor number)。
// 主设备号用于标识设备类型或设备驱动程序，而次设备号则用于标识特定的设备实例或设备的某个子功能。
// Xv6将主设备号和次设备号编码到一个单独的设备号中，使得可以使用一个单独的数字来唯一标识一个设备
#define major(dev) ((dev) >> 16 & 0xFFFF)     //设备号高16位
#define minor(dev) ((dev)&0xFFFF)             //设备号低16位
#define mkdev(m, n) ((uint)((m) << 16 | (n))) //主设备号和次设备号构造设备号

// 内核将活跃的inode集合保存在内存中，即磁盘上部分struct dinode的内存副本
// in-memory copy of an inode  
struct inode
{
  uint dev;              // Device number
  uint inum;             // Inode number
  int ref;               // Reference count 引用内存中inode的C指针的数量，如果引用计数降至零，内核将从内存中丢弃该inode
  struct sleeplock lock; // protects everything below here
  int valid;             // inode has been read from disk?

  short type; // copy of disk inode
  short major;
  short minor;
  short nlink;
  uint size;
  uint addrs[NDIRECT + 1];
};

// device switch用于将主设备号映射到设备函数
// devsw结构体数组是一种设备驱动程序的接口，它允许操作系统以统一的方式处理不同类型的设备
// map major device number to device functions.
struct devsw
{
  int (*read)(int, uint64, int);
  int (*write)(int, uint64, int);
};

extern struct devsw devsw[];

#define CONSOLE 1
