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

#define DEF_DUMP_DEPTH 2
#define DUMP_MATCH_MAX 200000
#define DUMP_MATCH_KEY "tfa"

struct super_block *get_superblock(const char *filename);

size_t dump_dentry_path(struct dentry *dentry, char *buff, int buflen);

int dump_dentry(const char *filename);

int analyze_dentry(const char *filename);

#endif

