/*
 * Date: 2022-03-21 23:14:45
 * Author: hdzhang<hdzhang920@163.com>
 * Description: 
*/

#include "dump_dentry.h"
#include "output.h"
#include "rb_dentry.h"

struct super_block *get_superblock(const char *filename)
{
        struct file *file;
        struct super_block *sb;

        file = filp_open(filename, O_RDONLY, 0);
        sb = file->f_path.mnt->mnt_sb;
        filp_close(file, current->files);
        return sb;
}

size_t dump_dentry_path(struct dentry *dentry, char *buff, int len)
{
        size_t free_len = len-1;
        size_t dentry_len = 0;

        //导出所有dentry
        while (dentry != dentry->d_parent && free_len > 0) {
                dentry_len = snprintf(buff, free_len, "%s/", dentry->d_iname);

                buff += dentry_len;
                free_len -= dentry_len;
                dentry = dentry->d_parent;
        }

        sprintf(buff, "\n");
        return len - free_len;
}

int dump_dentry(const char *filename)
{
        char *buff;
        size_t dump_len = 0;
        size_t dentry_count = 0;
        struct dentry *dentry;
        
        struct super_block *sb = get_superblock(filename);

        buff = (char *)kzalloc(OUTPUT_BUFFER_LEN, GFP_KERNEL);

        dump_len = snprintf(buff, OUTPUT_BUFFER_LEN, ">>> start dump \"%s\" dentry\n", filename);
        output(buff, dump_len, 1);
        memset(buff, 0, dump_len);

        list_for_each_entry(dentry, &sb->s_dentry_lru, d_lru) {
                dump_len = dump_dentry_path(dentry, buff, OUTPUT_BUFFER_LEN);
                output(buff, dump_len, 0);

                dentry_count++;
                memset(buff, 0, dump_len);
        }

        dump_len = snprintf(buff, OUTPUT_BUFFER_LEN, "<<< end dump \"%s\" dentry: %lu\n", filename, dentry_count);
        output(buff, dump_len, 0);

        kfree(buff);
        return 0;
}

int analyze_dentry(const char *filename)
{
        char *buff;
        size_t dump_len = 0;
        size_t dentry_count = 0;
        struct dentry *dentry, *curr_dentry;
        struct dentry *dentries[DEF_DUMP_DEPTH] = {NULL};
        int curr_depth, curr_count;

        struct super_block *sb = get_superblock(filename);
        
        buff = (char *)kzalloc(OUTPUT_BUFFER_LEN, GFP_KERNEL);
        dump_len = snprintf(buff, OUTPUT_BUFFER_LEN, ">>> start analyze \"%s\" dentry\n", filename);
        output(buff, dump_len, 1);

        list_for_each_entry(dentry, &sb->s_dentry_lru, d_lru) {
                curr_dentry = dentry;
                curr_depth = 0;
                curr_count = 0;

                while (curr_dentry != curr_dentry->d_parent) {
                        dentries[curr_depth] = curr_dentry;

                        curr_dentry = curr_dentry->d_parent;
                        curr_depth = (curr_depth+1) % DEF_DUMP_DEPTH;
                        curr_count++;
                }
                //将dentry插入rbtree
                if(curr_count < DEF_DUMP_DEPTH)
                        rb_insert_dentry(dentries, curr_count);
                else
                        rb_insert_dentry(dentries, DEF_DUMP_DEPTH);

                dentry_count++;
        }

        //导出计数信息
        rb_dump_dentrys();

        dump_len = snprintf(buff, OUTPUT_BUFFER_LEN, "<<< end analyze \"%s\" dentry: %lu\n", filename, dentry_count);
        output(buff, dump_len, 0);

        kfree(buff);
        return 0;
}


