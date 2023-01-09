/*
 * Date: 2022-03-22 18:32:03
 * Author: hdzhang<hdzhang920@163.com>
 * Description: 
*/

#ifndef __DUMP_DENTRY_
#define __DUMP_DENTRY_

#include <linux/mount.h>
#include <linux/slab.h>
#include <linux/string.h>

#define DEF_DUMP_DEPTH 1

#define DEF_DUMP_MATCH_MAX 200000

struct super_block *get_superblock(const char *filename);

size_t dump_dentry_path(struct dentry *dentry, char *buff, int buflen);

int dump_specific_dentry(const char *filename, char* match);

int analyze_dentry(const char *filename);

#endif

