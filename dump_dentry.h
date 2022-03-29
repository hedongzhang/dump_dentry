/*
 * Date: 2022-03-22 18:32:03
 * Author: hdzhang<hdzhang920@163.com>
 * Description: 
*/

#ifndef __DUMP_DENTRY_
#define __DUMP_DENTRY_

#include <linux/fs.h>
#include <linux/fdtable.h>
#include <linux/sched.h>
#include <linux/path.h>
#include <linux/mount.h>
#include <uapi/linux/fcntl.h>
#include <asm/current.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

struct dentry_counter {
        
}

struct super_block *get_superblock(const char *filename);

size_t dump_dentry_path(struct dentry *dentry, char *buf, int buflen);

void output(char *buf, int len, int new);

int dump_dentry(const char *filename);

#endif

