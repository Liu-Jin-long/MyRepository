// On-disk file system format.
// Both the kernel and user programs use this header file.

#define ROOTINO 1  // root i-number
#define BSIZE 1024 // block size

// Disk layout:
// [ boot block b0 | super block b1 | log b2~b31(log_header b2) | inode blocks b32~b45 | free bit map b46 | data blocks b47~ ]
//
// mkfs computes the super block and builds an initial file system.
// The super block describes the disk layout:
struct superblock
{
  uint magic;      // Must be FSMAGIC
  uint size;       // Size of file system image (blocks)
  uint nblocks;    // Number of data blocks
  uint ninodes;    // Number of inodes.
  uint nlog;       // Number of log blocks
  uint logstart;   // Block number of first log block
  uint inodestart; // Block number of first inode block
  uint bmapstart;  // Block number of first free map block
};

#define FSMAGIC 0x10203040

#define NDIRECT 12                       //直接块direct blocks 可用12KB
#define NINDIRECT (BSIZE / sizeof(uint)) //间接块indirect block 可用256KB
#define MAXFILE (NDIRECT + NINDIRECT)    //最大支持12+256=268KB的文件

// On-disk inode structure  64字节
struct dinode
{
  short type;              // File type 文件、目录和特殊文件（设备） 零表示磁盘inode空闲未使用
  short major;             // Major device number (T_DEVICE only)
  short minor;             // Minor device number (T_DEVICE only)
  short nlink;             // Number of links to inode in file system 硬链接数
  uint size;               // Size of file (bytes)
  uint addrs[NDIRECT + 1]; // Data block addresses
};

// Inodes per block.
#define IPB (BSIZE / sizeof(struct dinode))

// Block containing inode i
#define IBLOCK(i, sb) ((i) / IPB + sb.inodestart)

// Bitmap bits per block
#define BPB (BSIZE * 8)

// Block of free map containing bit for block b
#define BBLOCK(b, sb) ((b) / BPB + sb.bmapstart)

// Directory is a file containing a sequence of dirent structures.
#define DIRSIZ 14
// directory entry目录项 16字节
struct dirent
{
  ushort inum;       //目录中文件或者子目录的inode编号
  char name[DIRSIZ]; //目录中文件或者子目录名
};
