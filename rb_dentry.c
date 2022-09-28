/*
 * Date: 2022-03-29 15:39:01
 * Author: hdzhang<hdzhang920@163.com>
 * Description: 
*/

#include "rb_dentry.h"
#include "output.h"

static struct rb_root rb_dentry_root = RB_ROOT;

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

struct dentry_counter* alloc_counter(struct dentry **dentries, size_t num)
{
        size_t i;
        struct dentry_counter *counter;
        
        counter = kzalloc(sizeof(struct dentry_counter), GFP_KERNEL);
        RB_CLEAR_NODE(&counter->node);

        counter->hash = 0;
        counter->count = 1;
        counter->num = num;

        for (i = 0; i < num; i++) {
                if(dentries[i]) {
                        counter->dentries[i] = dentries[i];
                        counter->hash += get_hash(dentries[i]->d_iname);
                }
        }
        
        return counter;
}

/*
 * Desc: 
 * Args: 
 * Ret: 找到则返回dentry_counter指针，否则返回NULL
*/
struct dentry_counter *rb_search_dentry(struct dentry **dentries, size_t num)
{
        size_t i;
        int hash = 0;
        struct rb_node *rb_curr;
        struct dentry_counter *curr;

        for(i = 0; i < num; i++) {
                if (dentries[i])
                        hash += get_hash(dentries[i]->d_iname);
        }
        
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

struct dentry_counter *rb_insert_dentry(struct dentry **dentries, size_t num)
{
        size_t i;
        int hash = 0;
        struct rb_node *rb_curr, **rb_curr_p;
        struct dentry_counter *curr, *new;

        for(i = 0; i < num; i++) {
                if (dentries[i])
                        hash += get_hash(dentries[i]->d_iname);
        }
        
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

        new = alloc_counter(dentries, num);
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

size_t rb_dump_dntry(struct dentry_counter *counter, char *buff, size_t len)
{
        size_t i;
        size_t dentry_len = 0;
        size_t buff_free = len;

        for (i = 0; i < counter->num && buff_free > 64; i++)
        {
                if(!counter->dentries[i])
                        continue;
                        
                dentry_len = snprintf(buff, buff_free, "%s/", counter->dentries[i]->d_iname);
                buff += dentry_len;
                buff_free -= dentry_len;
        }
        dentry_len = snprintf(buff, 64, " Count: %lu\n", counter->count);
        buff_free -= dentry_len;

        return len - buff_free;
}

/*
 * Desc: 导出rbtree信息
 * Args: 
 * Ret: 
*/
void rb_dump_dentrys(void)
{
        char *buff;
        size_t buff_len;
        struct dentry_counter *curr;
        struct rb_node *rb_curr;

        buff = (char *)kzalloc(OUTPUT_BUFFER_LEN, GFP_KERNEL);

        //遍历所有插入到rbtree的目录名，打印对应的dentry计数
        for(rb_curr = rb_first(&rb_dentry_root); rb_curr; rb_curr = rb_next(rb_curr)) {
                curr = rb_entry(rb_curr, struct dentry_counter, node);
                
                buff_len = rb_dump_dntry(curr, buff, OUTPUT_BUFFER_LEN);
                
                output(buff, buff_len, 0);
                memset(buff, 0, buff_len);
        }

        kfree(buff);
        return;
}

int rb_init_dentry(void)
{
        return 0;
}

int rb_free_dentry(void)
{
        struct dentry_counter *curr;
        struct rb_node *rb_curr_counter, *rb_next_counter;

        if(RB_EMPTY_ROOT(&rb_dentry_root))
                return 0;

        rb_curr_counter = rb_first(&rb_dentry_root);
        rb_next_counter = rb_next(rb_curr_counter);
        for(; rb_curr_counter; ) {
                curr = rb_entry(rb_curr_counter, struct dentry_counter, node);
                rb_erase(rb_curr_counter, &rb_dentry_root);
                kfree(curr);

                if(!rb_next_counter)
                        break;
                rb_curr_counter = rb_next_counter;
                rb_next_counter = rb_next(rb_curr_counter);
        }

        return 0;
}