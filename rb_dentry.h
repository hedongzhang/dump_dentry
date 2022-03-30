/*
 * Date: 2022-03-29 15:42:33
 * Author: hdzhang<hdzhang920@163.com>
 * Description: 
*/

#ifndef __RB_DENTRY_
#define __RB_DENTRY_

#include <linux/rbtree.h>
#include <linux/slab.h>

#define COUNTER_NAME_LEN 256

struct dentry_counter {
        struct rb_node node;
        
        char name[COUNTER_NAME_LEN];
        int hash;
        size_t count;
};

int get_hash(const char *name);

struct dentry_counter* alloc_counter(const char *name);

int rb_init_dentry(void);

int rb_free_dentry(void);

struct dentry_counter *rb_search_dentry(const char *name);

struct dentry_counter *rb_insert_dentry(const char *name);

void rb_foreach_dentry(void *callback(struct rb_node*));

void rb_dump_dentry(void);

#endif
