#ifndef _EXPORTED_INODE_H
#define _EXPORTED_INODE_H


struct exp_inode {
	unsigned short		i_mode;
	unsigned short		i_opflags;
	unsigned int		i_flags;

	unsigned long		i_ino;
	unsigned int 		i_nlink;
	u32				i_rdev;
	unsigned int		i_size;
	int64_t				i_atime;
	int64_t				i_mtime;
	int64_t				i_ctime;
	unsigned short      i_bytes;
	u8				i_blkbits;
	u8				i_write_hint;
	u64				i_blocks;
};

#endif