/*
 * Date: 2022-03-21 23:11:38
 * Author: hdzhang<hdzhang920@163.com>
 * Description: 
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include "get_dentry.h"

static int __init kdemo_init(void) {
    printk(">>> demo driver begin!\n");
    get_superblock("/");
    return 0;
}

static void __exit kdemo_exit(void) {
    printk(">>> demo driver exit!\n");
}

module_init(kdemo_init);
module_exit(kdemo_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("hdzhang920@163.com");
MODULE_VERSION("1.0");
