/*
 * Date: 2022-03-21 23:11:38
 * Author: hdzhang<hdzhang920@163.com>
 * Description: 
*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

#include "dump_dentry.h"
#include "rb_dentry.h"

#define DEF_DUMP_DIRECTORY "/"
#define DEF_DUMP_MATCH_KEY "tmp"

static char *dump_directory = DEF_DUMP_DIRECTORY;
module_param(dump_directory, charp, S_IRUGO);

static int dump_match_on = 0;
module_param(dump_match_on, int, S_IRUGO);

static char *dump_match_key = DEF_DUMP_MATCH_KEY;
module_param(dump_match_key, charp, S_IRUGO);

static int __init kdemo_init(void)
{
    printk(">>> ddump driver begin!\n");

    if(!dump_match_on) {
        rb_init_dentry();
        analyze_dentry(dump_directory);
    } else {
        dump_specific_dentry(dump_directory, dump_match_key);
    }

    return 0;
}

static void __exit kdemo_exit(void)
{
    if(!dump_match_on) {
        rb_free_dentry();
    }
    printk(">>> ddump driver exit!\n");
}

module_init(kdemo_init);
module_exit(kdemo_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("hdzhang920@163.com");
MODULE_VERSION("1.0");