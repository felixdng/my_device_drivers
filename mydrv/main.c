#include <linux/init.h>
#include <linux/module.h>
#include <linux/kobject.h>

#include "mydrv.h"


struct kobject *root_node = NULL;
static int _root_node_add(void)
{
	root_node = kobject_create_and_add("mydrv", NULL);
	if (NULL == root_node) {
		pr_debug("kojbect created failed!\n");
		return -ENODEV;
	}
	return 0;
}
static void _root_node_del(void)
{
	if (root_node)
		kobject_del(root_node);
}

static __init int mydrv_init(void)
{
	int retval = 0;
	pr_debug("enter...\n");

	if ((retval = _root_node_add()))
		return retval;

	if ((retval = i2c_nodes_add()))
		goto err1;

	return 0;
err1:
	_root_node_del();
	return retval;
}

static __exit void mydrv_exit(void)
{
	pr_debug("leave...\n");
	
	i2c_nodes_del();
	_root_node_del();
}

module_init(mydrv_init);
module_exit(mydrv_exit);
MODULE_LICENSE("GPL");
