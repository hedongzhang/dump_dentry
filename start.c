/*
 * Date: 2022-03-21 23:11:38
 * Author: hdzhang<hdzhang920@163.com>
 * Description: 
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include "dump_dentry.h"
#include "rb_dentry.h"

#define DUMP_DIRECTORY "/"

static int __init kdemo_init(void)
{
    printk(">>> ddump driver begin!\n");
    // rb_init_dentry();
    dump_dentry(DUMP_DIRECTORY);
    // analyze_dentry(DUMP_DIRECTORY);
    return 0;
}

static void __exit kdemo_exit(void)
{
    // rb_free_dentry();
    printk(">>> ddump driver exit!\n");
}

module_init(kdemo_init);
module_exit(kdemo_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("hdzhang920@163.com");
MODULE_VERSION("1.0");