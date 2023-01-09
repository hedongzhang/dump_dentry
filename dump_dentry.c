/*
 * Date: 2022-03-21 23:14:45
 * Author: hdzhang<hdzhang920@163.com>
 * Description: 
*/
#include <linux/module.h>

#include "dump_dentry.h"
#include "output.h"
#include "rb_dentry.h"


static int dump_depth = DEF_DUMP_DEPTH;
module_param(dump_depth, int, S_IRUGO);
static int dump_match_max = DEF_DUMP_MATCH_MAX;
module_param(dump_match_max, int, S_IRUGO);

struct super_block *get_superblock(const char *filename)
{
        struct file *f;
        struct super_block *sb;

        f = filp_open(filename, O_RDONLY, 0);
        if (IS_ERR(f)) {
                printk("open file:%s failed\n", filename);
                return NULL;
        }

        sb = f->f_path.mnt->mnt_sb;

        filp_close(f, current->files);
        return sb;
}

//导出指定dentry的全路径
size_t dump_dentry_path(struct dentry *dentry, char *buff, int len)
{
        size_t free_len = len - 1;
        size_t dentry_len = 0;

        while (dentry != dentry->d_parent && free_len > 0) {
                dentry_len = snprintf(buff, free_len, "%s/", dentry->d_iname);

                buff += dentry_len;
                free_len -= dentry_len;
                dentry = dentry->d_parent;
        }

        sprintf(buff, "\n");
        return len - free_len;
}

//导出指定目录下的dentry总量，并输出包含指定关键字的dentry
int dump_specific_dentry(const char *filename, char* match)
{
        char *buff;
        size_t dump_len = 0;
        size_t dentry_count = 0;
        size_t match_count = 0;
        struct dentry *dentry;
        
        struct super_block *sb = get_superblock(filename);
        if(!sb) {
                printk("dump_dentry failed: get superblock error\n");
                return -1;
        }

        buff = (char *)kzalloc(OUTPUT_BUFFER_LEN, GFP_KERNEL);
        if(!buff) {
                printk("dump_dentry failed: kzalloc output buffer error\n");
                return -1;
        }

        dump_len = snprintf(buff, OUTPUT_BUFFER_LEN, ">>> Start dump \"%s\" dentry\n", filename);
        output(buff, dump_len, 1);
        memset(buff, 0, OUTPUT_BUFFER_LEN);

        list_for_each_entry(dentry, &sb->s_dentry_lru, d_lru) {
                dump_len = dump_dentry_path(dentry, buff, OUTPUT_BUFFER_LEN);

                if(strnstr(buff, match, dump_len)) {
                        match_count++;
                        output(buff, dump_len, 0);
                }

                dentry_count++;
                memset(buff, 0, OUTPUT_BUFFER_LEN);

                if(match_count > dump_match_max)
                        break;
        }

        dump_len = snprintf(buff, OUTPUT_BUFFER_LEN, "<<< End dump \"%s\" dentry:%lu match \"%s\":%lu\n", 
                filename, dentry_count, match, match_count);
        output(buff, dump_len, 0);

        kfree(buff);
        return 0;
}

//导出指定目录下的denry情况
int analyze_dentry(const char *filename)
{
        char *buff;
        size_t dump_len = 0;
        size_t dentry_count = 0;
        struct dentry *dentry, *curr_dentry;
        struct dentry *dentries[DEF_DENTRY_NUM_MAX] = {NULL};
        int curr_index;

        struct super_block *sb = get_superblock(filename);
        if(!sb) {
                printk("analyze_dentry failed: get superblock error\n");
                return -1;
        }
        
        buff = (char *)kzalloc(OUTPUT_BUFFER_LEN, GFP_KERNEL);
        if(!buff) {
                printk("analyze_dentry failed: kzalloc buffer error\n");
                return -1;
        }

        dump_len = snprintf(buff, OUTPUT_BUFFER_LEN, ">>> Start analyze \"%s\" dentry\n", filename);
        output(buff, dump_len, 1);
        memset(buff, 0, OUTPUT_BUFFER_LEN);

        list_for_each_entry(dentry, &sb->s_dentry_lru, d_lru) {
                curr_dentry = dentry;
                curr_index = 0;

                while (curr_dentry != curr_dentry->d_parent) {
                        dentries[curr_index] = curr_dentry;

                        curr_dentry = curr_dentry->d_parent;
                        curr_index = (curr_index + 1) % DEF_DENTRY_NUM_MAX;
                }
                //将dentry插入rbtree
                rb_insert_dentry(dentries, curr_index, dump_depth);

                dentry_count++;
        }

        //导出计数信息
        rb_dump_dentrys();

        dump_len = snprintf(buff, OUTPUT_BUFFER_LEN, "<<< End analyze \"%s\" dentry: %lu\n", filename, dentry_count);
        output(buff, dump_len, 0);

        kfree(buff);
        return 0;
}


