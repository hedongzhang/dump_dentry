/*
 * Date: 2022-03-29 23:45:40
 * Author: hdzhang<hdzhang920@163.com>
 * Description: 
*/

#include "output.h"

void output(char *buff, int len, int new)
{
        ssize_t ret;
        struct file *f;
        mm_segment_t fs;
        loff_t pos = 0;

        if (new)
                f = filp_open(OUTPUT_FILE, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        else
                f = filp_open(OUTPUT_FILE, O_WRONLY | O_APPEND, 0644);
        if (IS_ERR(f)) {
                printk("create or open dump file error\n");
                return;
        }

        fs = get_fs();
        set_fs(KERNEL_DS);
        ret = vfs_write(f, buff, len, &pos);
        filp_close(f, NULL);
        set_fs(fs);

        return;
}
