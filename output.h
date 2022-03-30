/*
 * Date: 2022-03-29 23:45:33
 * Author: hdzhang<hdzhang920@163.com>
 * Description: 
*/

#ifndef __OUTPUT_
#define __OUTPUT_

#include <linux/fs.h>
#include <linux/uaccess.h>

#define OUTPUT_FILE "/tmp/dump_dentry.out"
#define OUTPUT_BUFFER_LEN PAGE_SIZE

void output(char *buff, int len, int new);

#endif
