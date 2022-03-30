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
        size_t free_len = len - 1;
        size_t dentry_len = 0;
        struct dentry *child_dentry = NULL;

        while (free_len) {
                dentry_len = snprintf(buff, free_len, "%s/", dentry->d_iname);
                // dentry_len = snprintf(buff, dentry->d_name.len+1, "%s/", dentry->d_name.name);
                if (dentry == dentry->d_parent)
                        break;

                buff += dentry_len;
                free_len -= dentry_len;
                child_dentry = dentry;
                dentry = dentry->d_parent;
        }

        //将路径中最上层目录名插入rbtree
        rb_insert_dentry(child_dentry->d_iname);
        
        sprintf(buff, "\n");
        return len - free_len;
}

int dump_dentry(const char *filename)
{
        struct dentry *dentry;
        size_t dump_len = 0;
        size_t dentry_count = 0;
        char *buff;

        struct super_block *sb = get_superblock(filename);

        buff = (char *)kzalloc(OUTPUT_BUFFER_LEN, GFP_KERNEL);

        dump_len = snprintf(buff, OUTPUT_BUFFER_LEN, ">>> start dump \"%s\" dentry\n", filename);
        output(buff, dump_len, 1);
        memset(buff, 0, OUTPUT_BUFFER_LEN);

        list_for_each_entry(dentry, &sb->s_dentry_lru, d_lru) {
                // buf_len = snprintf(buff, FILENAME_LEN, "%s\n", dentry->d_name.name);
                dump_len = dump_dentry_path(dentry, buff, OUTPUT_BUFFER_LEN);
                output(buff, dump_len, 0);

                dentry_count++;
                memset(buff, 0, dump_len);
        }

        //导出dentry计数信息
        dump_and_free_rb();

        dump_len = snprintf(buff, OUTPUT_BUFFER_LEN, "<<< end dump \"%s\" dentry: %lu\n", filename, dentry_count);
        output(buff, dump_len, 0);

        kfree(buff);
        return 0;
}




