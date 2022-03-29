/*
 * Date: 2022-03-21 23:14:45
 * Author: hdzhang<hdzhang920@163.com>
 * Description: 
*/

#include "dump_dentry.h"

#define OUT_FILE "/tmp/dump_dentry.out"
#define OUTPUT_BUFFER_LEN 4096

struct super_block *get_superblock(const char *filename)
{
        struct file *file;
        struct super_block *sb;

        file = filp_open(filename, O_RDONLY, 0);
        sb = file->f_path.mnt->mnt_sb;
        filp_close(file, current->files);
        return sb;
}

size_t dump_dentry_path(struct dentry *dentry, char *buf, int len)
{
        size_t free_len = len - 1;
        size_t dentry_len = 0;

        while (free_len) {
                dentry_len = snprintf(buf, free_len, "%s/", dentry->d_iname);
                // dentry_len = snprintf(buf, dentry->d_name.len+1, "%s/", dentry->d_name.name);
                if (dentry == dentry->d_parent)
                        break;

                buf += dentry_len;
                free_len -= dentry_len;
                dentry = dentry->d_parent;
        }
        
        sprintf(buf, "\n");
        return len - free_len;
}

void output(char *buf, int len, int new)
{
        ssize_t ret;
        struct file *f;
        mm_segment_t fs;
        loff_t pos = 0;

        if (new)
                f = filp_open(OUT_FILE, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        else
                f = filp_open(OUT_FILE, O_WRONLY | O_APPEND, 0644);
        if (IS_ERR(f)) {
                printk("create or open dump file error\n");
                return;
        }

        fs = get_fs();
        set_fs(KERNEL_DS);
        ret = vfs_write(f, buf, len, &pos);
        filp_close(f, NULL);
        set_fs(fs);

        return;
}

int dump_dentry(const char *filename)
{
        struct dentry *dentry;
        size_t dump_len = 0;
        size_t dentry_count = 0;
        char *buf;

        struct super_block *sb = get_superblock(filename);

        buf = (char *)kzalloc(OUTPUT_BUFFER_LEN, GFP_KERNEL);

        dump_len = snprintf(buf, OUTPUT_BUFFER_LEN, ">>> start dump \"%s\" dentry\n", filename);
        output(buf, dump_len, 1);
        memset(buf, 0, OUTPUT_BUFFER_LEN);

        list_for_each_entry(dentry, &sb->s_dentry_lru, d_lru) {
                // buf_len = snprintf(buf, FILENAME_LEN, "%s\n", dentry->d_name.name);
                dump_len = dump_dentry_path(dentry, buf, OUTPUT_BUFFER_LEN);
                output(buf, dump_len, 0);

                dentry_count++;
                memset(buf, 0, dump_len);
        }

        dump_len = snprintf(buf, OUTPUT_BUFFER_LEN, "<<< end dump \"%s\" dentry: %lu\n", filename, dentry_count);
        output(buf, dump_len, 0);

        kfree(buf);
        return 0;
}




