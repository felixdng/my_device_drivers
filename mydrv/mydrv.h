#ifndef __MY_DRIVERS_H
#define __MY_DRIVERS_H

#include <linux/kobject.h>

#undef pr_debug
#define pr_debug(fmt, ...) \
printk(KERN_INFO "[%s()-L%d] "fmt, __func__, __LINE__, ##__VA_ARGS__)

#undef pr_info
#define pr_info(fmt, ...) \
printk(KERN_INFO ""fmt, ##__VA_ARGS__)

extern struct kobject *root_node;

int i2c_nodes_add(void);
void i2c_nodes_del(void);

/* commands for device and driver */
#define CMD_ADD "add"
#define CMD_DEL "del"
#define CMD_CLR "clr"
#define IS_CMD(_cmd, _str) \
(!memcmp(_cmd, _str, sizeof(_cmd) - 1))
#define IS_CMD_ADD(_str) IS_CMD(CMD_ADD, _str)
#define IS_CMD_DEL(_str) IS_CMD(CMD_DEL, _str)
#define IS_CMD_CLR(_str) IS_CMD(CMD_CLR, _str)

#endif
