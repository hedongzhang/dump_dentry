/*
 * Date: 2022-03-21 23:14:45
 * Author: hdzhang<hdzhang920@163.com>
 * Description: 
*/

#include "dump_dentry.h"

#define OUT_FILE "/tmp/output"

struct super_block *get_superblock(const char *filename) {
        
        struct file *file = filp_open(filename, O_RDONLY, 0);
        struct path *path = &file->f_path;
        struct super_block *sb = path->mnt->mnt_sb;

        printk("open file:%s\n", path->dentry->d_iname);
        filp_close(file, current->files);

        return sb;
}

int dump_dentry(struct super_block *sb, char *buf, size_t len) {
        struct dentry *dentry;
        size_t dentry_count = 0;

        if (!list_empty(&sb->s_dentry_lru)) {
                list_for_each_entry(dentry, &sb->s_dentry_lru, d_lru) {
                        // spin_lock(&dentry->d_lock);
                        // if (dentry->d_flags & DCACHE_REFERENCED) {
                        dentry_count++;
                        // }
                        // spin_unlock(&dentry->d_lock);
                        // memset(path, 0, 2048); 

                        // cp = dentry_path_stp(dentry, path, 1024); //解析每个dentry的文件路径，参考char *dentry_path(struct dentry *dentry, char *buf, int buflen)的实现
                        // if (!IS_ERR(cp)) {
                        //     if(strlen(cp))
                        //         //offset += snprintf(buf+offset, PAGE_SIZE-offset, "%s", cp);
                        //         ;
                        //     else
                        //         cp = path+1024;                        
                        // }  
                        
                        // pp = path+1024;
                        // pp += sprintf(path+1024, "%d ", dcache_referenced_nr);
                        
                        // if (offset + (pp-cp) + 3 >= PAGE_SIZE) {//\r\n space
                        //      fp->f_op->write(fp, buf, offset, &fp->f_pos);     //将文件路径写log
                        //      offset = 0;
                        //      memset(buf,0,PAGE_SIZE);
                        // }

                        // offset += snprintf(buf+offset, PAGE_SIZE-offset, "%s %s\r\n", cp, path+1024);
                }
        }

        sprintf(buf, "dentry_count:%lu\n", dentry_count);

        return 0;
}

void output(char *buf, int len) {
        struct file *f;
        mm_segment_t fs;
        loff_t pos = 0;

        f = filp_open(OUT_FILE, O_RDWR | O_CREAT, 0644);
        if (IS_ERR(f)) {
                printk("create outout file error\n");
                return;
        }
        fs = get_fs();
        set_fs(KERNEL_DS);

        vfs_write(f, buf, len, &pos);
        filp_close(f, NULL);
        set_fs(fs);
}

int dump(const char *filename) {
        struct super_block *sb;
        char buf[64] = "";
        size_t len = 64;

        sb = get_superblock(filename);
        dump_dentry(sb, buf, len);
        output(buf, len);
        return 0;
}
