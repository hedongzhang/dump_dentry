/*
 * Date: 2022-03-29 15:42:33
 * Author: hdzhang<hdzhang920@163.com>
 * Description: 
*/

#ifndef __RB_DENTRY_
#define __RB_DENTRY_

#include <linux/rbtree.h>
#include <linux/slab.h>

#define DENTRY_NUM_MAX 64

struct dentry_counter {
        struct rb_node node;
        
        int hash;
        size_t count;
        size_t num;
        struct dentry *dentries[DENTRY_NUM_MAX];
};

int get_hash(const char *name);

struct dentry_counter* alloc_counter(struct dentry **dentries, size_t num);

struct dentry_counter *rb_search_dentry(struct dentry **dentries, size_t num);

struct dentry_counter *rb_insert_dentry(struct dentry **dentries, size_t num);

void rb_foreach_dentry(void *callback(struct rb_node*));

size_t rb_dump_dntry(struct dentry_counter *counter, char *buff, size_t len);

void rb_dump_dentrys(void);

int rb_init_dentry(void);

int rb_free_dentry(void);

#endif
