/*
 * Date: 2022-03-21 23:14:45
 * Author: hdzhang<hdzhang920@163.com>
 * Description: 
*/

#include "dump_dentry.h"

#define OUT_FILE "/tmp/output"
#define FILENAME_LEN 1024
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
        char buf[OUTPUT_BUFFER_LEN] = "";
        size_t buf_len = 0;
        size_t dentry_count = 0;

        if (!list_empty(&sb->s_dentry_lru)) {
                list_for_each_entry(dentry, &sb->s_dentry_lru, d_lru) {
                        buf_len = snprintf(buf, FILENAME_LEN, "%s\n", dentry->d_name.name);
                        // buf_len = dentry_path_ext(dentry, buf, FILENAME_LEN);

                        output(buf, buf_len);

                        dentry_count++;
                        memset(buf, 0, OUTPUT_BUFFER_LEN);
                }
        }

        buf_len = sprintf(buf, "dentry_count:%lu\n", dentry_count);
        output(buf, buf_len);

        printk("dump %lu dentry\n", dentry_count);
        return 0;
}

char *dentry_path_ext(struct dentry *dentry, char *buf, int buflen)
{
// 	char *p = NULL;
// 	char *retval;

// 	if (d_unlinked(dentry)) {
// 		p = buf + buflen;
// 		if (prepend(&p, &buflen, "//deleted", 10) != 0)
// 			goto Elong;
// 		buflen++;
// 	}
// 	retval = __dentry_path(dentry, buf, buflen);
// 	if (!IS_ERR(retval) && p)
// 		*p = '/';	/* restore '/' overriden with '\0' */
// 	return retval;
// Elong:
// 	return ERR_PTR(-ENAMETOOLONG);
}

void output(char *buf, int len) {
        ssize_t ret;
        struct file *f;
        mm_segment_t fs;
        loff_t pos;

        // f = filp_open(OUT_FILE, O_RDWR | O_CREAT | O_APPEND, 0644);
        // sb = file->f_path.mnt->mnt_sb;
        
        // f->f_op->write(f, buf, offset, &fp->f_pos);

        // filp_close(f, current->files);

        f = filp_open(OUT_FILE, O_RDWR | O_CREAT | O_APPEND, 0644);
        if (IS_ERR(f)) {
                printk("create outout file error\n");
                return;
        }
        fs = get_fs();
        set_fs(KERNEL_DS);

        // pos = f->f_pos;
        ret = vfs_write(f, buf, len, &pos);
        // f->f_pos = pos;
        filp_close(f, NULL);
        set_fs(fs);
}