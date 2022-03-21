/*
 * Date: 2022-03-21 23:17:49
 * Author: hdzhang<hdzhang920@163.com>
 * Description: 
*/

#ifndef __GET_DENTRY_
#define __GET_DENTRY_

#include <linux/fs.h>
#include <linux/fdtable.h>
#include <linux/sched.h>
#include <linux/path.h>
#include <linux/mount.h>
#include <uapi/linux/fcntl.h>
#include <asm/current.h>

int get_superblock(const char *filename);

int dump_dentry(struct super_block *sb);

#endif