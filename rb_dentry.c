/*
 * Date: 2022-03-29 15:39:01
 * Author: hdzhang<hdzhang920@163.com>
 * Description: 
*/

#include "rb_dentry.h"
#include "output.h"

struct rb_root rb_dentry_root = RB_ROOT;

/*
 * Desc: 计算hash，碰撞概率较大，后续优化
 * Args: 
 * Ret: 
*/
int get_hash(const char *name)
{
        int hash = 0;
        int len = strlen(name);

        while (len) {
                hash += (int)name[len-1] * len;
                --len;
        }
        return hash;
}

struct dentry_counter* alloc_counter(const char *name)
{
        struct dentry_counter *counter;
        
        counter = kzalloc(sizeof(struct dentry_counter), GFP_KERNEL);
        RB_CLEAR_NODE(&counter->node);
        strncpy(counter->name, name, COUNTER_NAME_LEN-1);
        counter->hash = get_hash(name);
        counter->count = 0;
        
        return counter;
}

void free_counter_root(void)
{
        struct dentry_counter *curr;
        struct rb_node *rb_curr_counter, *rb_next_countr;

        rb_curr_counter = rb_first(&rb_dentry_root);
        rb_next_countr = rb_next(rb_curr_counter);
        for(; rb_curr_counter; rb_next_countr = rb_next(rb_curr_counter)) {
                curr = rb_entry(rb_curr_counter, struct dentry_counter, node);
                kfree(curr);
                rb_curr_counter = rb_next_countr;
        }
}

/*
 * Desc: 
 * Args: 
 * Ret: 找到则返回dentry_counter指针，否则返回NULL
*/
struct dentry_counter *rb_search_dentry(const char *name)
{
        int hash;
        struct rb_node *rb_curr;
        struct dentry_counter *curr;

        hash = get_hash(name);
        rb_curr = rb_dentry_root.rb_node;

        while (rb_curr) {
                curr = rb_entry(rb_curr, struct dentry_counter, node);

                if(hash < curr->hash)
                        rb_curr = rb_curr->rb_left;
                else if(hash > curr->hash)
                        rb_curr = rb_curr->rb_right;
                else
                        return curr;
        }

        return NULL;
}

struct dentry_counter *rb_insert_dentry(const char *name)
{
        int hash;
        struct rb_node *rb_curr, **rb_curr_p;
        struct dentry_counter *curr, *new;

        hash = get_hash(name);
        
        rb_curr_p = &rb_dentry_root.rb_node;
        rb_curr = *rb_curr_p;
        
        while (*rb_curr_p) {
                rb_curr = *rb_curr_p;
                curr = rb_entry(rb_curr, struct dentry_counter, node);

                if(hash < curr->hash)
                        rb_curr_p = &rb_curr->rb_left;
                else if(hash > curr->hash)
                        rb_curr_p = &rb_curr->rb_right;
                else {
                        curr->count++;
                        return curr;
                }            
        }

        new = alloc_counter(name);
        if (!new)
                return NULL;
        
        rb_link_node(&new->node, rb_curr, rb_curr_p);
        rb_insert_color(&new->node, &rb_dentry_root);
        
        return new;
}

void rb_foreach_dentry(void *callback(struct rb_node*))
{
        struct rb_node *rb_curr;
        
        for(rb_curr = rb_first(&rb_dentry_root); rb_curr; rb_curr = rb_next(rb_curr))
                callback(rb_curr);
}

/*
 * Desc: 导出rbtree信息并释放rbtree内存
 * Args: 
 * Ret: 
*/
void dump_and_free_rb(void)
{
        char *buff;
        size_t dump_len = 0;
        struct dentry_counter *curr;
        struct rb_node *rb_curr;

        buff = (char *)kzalloc(OUTPUT_BUFFER_LEN, GFP_KERNEL);

        //遍历所有插入到rbtree的目录名，打印对应的dentry计数
        for(rb_curr = rb_first(&rb_dentry_root); rb_curr; rb_curr = rb_next(rb_curr)) {
                curr = rb_entry(rb_curr, struct dentry_counter, node);

                dump_len = snprintf(buff, OUTPUT_BUFFER_LEN, "directory: \"%s\" count: %lu\n", curr->name, curr->count);
                output(buff, dump_len, 0);

                memset(buff, 0, dump_len);
        }

        kfree(buff);
        return;
}

// void rb_foreach_dentry(struct rb_node *rb_curr, void *callback(struct rb_node*))
// {
//         if(!rb_curr) 
//                 return;
//         rb_foreach_dentry(rb_curr->rb_left, callback);
//         callback(rb_curr)
//         rb_foreach_dentry(rb_curr->rb_right, callback);
// }
