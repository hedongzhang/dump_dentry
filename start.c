#include <linux/module.h>
#include <linux/kernel.h>    //包含了常用的内核函数
#include <linux/init.h>      //包含了宏__init和宏__exit


static int __init kdemo_init(void) {
    printk(">>> demo driver begin!\n");
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
