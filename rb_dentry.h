/*
 * Date: 2022-03-29 15:42:33
 * Author: hdzhang<hdzhang920@163.com>
 * Description: 
*/

#ifndef __RB_DENTRY_
#define __RB_DENTRY_

#include <linux/rbtree.h>
#include <linux/slab.h>

#define DEF_DENTRY_NUM_MAX 64

struct dentry_counter {
        struct rb_node node;

        //dentry数量统计
        size_t count;
        
        //通过depth计算hash值
        size_t depth;
        size_t hash;
        
        size_t num;
        struct dentry *dentries[DEF_DENTRY_NUM_MAX];
};

size_t get_hash(const char *name);

size_t get_dentries_hash(struct dentry **dentries, size_t num, size_t depth);

struct dentry_counter* alloc_counter(struct dentry **dentries, size_t num, size_t depth);

struct dentry_counter *rb_search_dentry(struct dentry **dentries, size_t num, size_t depth);

struct dentry_counter *rb_insert_dentry(struct dentry **dentries, size_t num, size_t depth);

void rb_foreach_dentry(void *callback(struct rb_node*));

size_t rb_dump_dntry(struct dentry_counter *counter, char *buff, size_t len);

void rb_dump_dentrys(void);

int rb_init_dentry(void);

int rb_free_dentry(void);

#endif
