#ifndef GET_DENTRY__
#define GET_DENTRY__

int get_superblock(const char *filename);

int dump_dentry(struct super_block *sb);

#endif