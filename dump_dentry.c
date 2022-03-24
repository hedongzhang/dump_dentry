/*
 * Date: 2022-03-21 23:14:45
 * Author: hdzhang<hdzhang920@163.com>
 * Description: 
*/

#include "dump_dentry.h"

#define OUT_FILE "/tmp/dump_dentry"
#define OUTPUT_BUFFER_LEN 4096

struct super_block *get_superblock(const char *filename) {
        struct file *file;
        struct super_block *sb;

        file = filp_open(filename, O_RDONLY, 0);
        sb = file->f_path.mnt->mnt_sb;
        filp_close(file, current->files);
        printk("get \"%s\" superblock\n", file->f_path.dentry->d_iname);
        return sb;
}

int dump_dentry(struct super_block *sb) {
        struct dentry *dentry;
        size_t dump_len = 0;
        size_t dentry_count = 0;
        char *buf;

        buf = (char *)kmalloc(OUTPUT_BUFFER_LEN, GFP_KERNEL);
        memset(buf, 0, OUTPUT_BUFFER_LEN);

        list_for_each_entry(dentry, &sb->s_dentry_lru, d_lru)
        {
                // buf_len = snprintf(buf, FILENAME_LEN, "%s\n", dentry->d_name.name);
                dump_len = dump_dentry_path(dentry, buf, OUTPUT_BUFFER_LEN);
                output(buf, dump_len);

                dentry_count++;
                memset(buf, 0, OUTPUT_BUFFER_LEN);
        }

        dump_len = snprintf(buf, OUTPUT_BUFFER_LEN, "dump %lu dentry\n", dentry_count);
        output(buf, dump_len);

        kfree(buf);
        printk("dump %lu dentry\n", dentry_count);
        return 0;
}

size_t dump_dentry_path(struct dentry *dentry, char *buf, int len) {
        size_t free_len = len - 1;
        size_t dentry_len = 0;

        while (free_len)
        {
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

void output(char *buf, int len) {
        ssize_t ret;
        struct file *f;
        mm_segment_t fs;
        loff_t pos;

        f = filp_open(OUT_FILE, O_CREAT | O_RDWR | O_APPEND, 0644);
        if (IS_ERR(f))
        {
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