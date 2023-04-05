#include <linux/i2c.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/platform_device.h>

#include "mydrv.h"


static int drv1_probe(struct platform_device *pdev)
{
	pr_debug("enter.... %s\n", (to_i2c_client(&pdev->dev))->name);
	return 0;
}
static int drv1_remove(struct platform_device *pdev)
{
	pr_debug("enter.... %s\n", (to_i2c_client(&pdev->dev))->name);
	return 0;
}
static const struct platform_device_id drv1_ids[] = {
	{ .name = "mydrv1-i2c1" },
	{ .name = "mydrv1-i2c2" },
};

static int drv2_probe(struct platform_device *pdev)
{
	pr_debug("enter.... %s\n", (to_i2c_client(&pdev->dev))->name);
	return 0;
}
static int drv2_remove(struct platform_device *pdev)
{
	pr_debug("enter.... %s\n", (to_i2c_client(&pdev->dev))->name);
	return 0;
}
static const struct platform_device_id drv2_ids[] = {
	{ .name = "mydrv2-i2c1" },
	{ .name = "mydrv2-i2c2" },
};
/* i2c drivers */
static struct i2c_driver i2c_drvs[] = {
	{
		.probe = drv1_probe,
		.remove = drv1_remove,
		.id_table = drv1_ids,
		.driver = {
			.name = "i2c_drv-1",
		},
	},
	{
		.probe = drv2_probe,
		.remove = drv2_remove,
		.id_table = drv2_ids,
		.driver = {
			.name = "i2c_drv-2",
		},
	}
};
static const int i2c_drvs_num = sizeof(i2c_drvs) / sizeof(i2c_drvs[0]);
static int i2c_drvs_cnt = 0;

static size_t drv_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	int i;
	pr_info("%d i2c dirvers%c\n", i2c_drvs_cnt, i2c_drvs_cnt > 0 ? ':' : '.');
	for (i = 0; i < i2c_drvs_cnt; ++i)
		pr_info("%s\n", i2c_drvs[i].driver.name);
	return 0;
}
static ssize_t drv_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	if (IS_CMD_ADD(buf) && i2c_drvs_cnt < i2c_drvs_num) {
		if (i2c_add_driver(&i2c_drvs[i2c_drvs_cnt]))
			pr_debug("i2c driver added failed(%s)\n", i2c_drvs[i2c_drvs_cnt].driver.name);
		else
			i2c_drvs_cnt++;
	} else if (IS_CMD_DEL(buf) && i2c_drvs_cnt > 0) {
		i2c_del_driver(&i2c_drvs[i2c_drvs_cnt - 1]);
		i2c_drvs_cnt--;
	}
	return count;
}
static struct kobj_attribute drv_attr = {
	.attr = {
		.name = "driver",
		.mode = 0660,
	},
	.show = drv_show,
	.store = drv_store,
};

static int i2c_mxfer(struct i2c_adapter *adap, struct i2c_msg *msgs, int num)
{
	pr_debug("%d\n", num);
	return 1;
}
static int i2c_sxfer(struct i2c_adapter *adap, u16 addr, unsigned short flags, char read_write,
					 u8 command, int size, union i2c_smbus_data *data)
{
	pr_debug("%d\n", size);
	return 1;
}
static u32 i2c_func(struct i2c_adapter *adapter)
{
	return I2C_FUNC_SMBUS_QUICK | I2C_FUNC_SMBUS_BYTE |
			I2C_FUNC_SMBUS_BYTE_DATA | I2C_FUNC_SMBUS_WORD_DATA |
			I2C_FUNC_SMBUS_PROC_CALL;
}
static struct i2c_algorithm i2c_algo = {
	.master_xfer = i2c_mxfer,
	.smbus_xfer = i2c_sxfer,
	.functionality = i2c_func,
};
static struct i2c_adapter i2c_adpts[] = {
	{
		.owner = THIS_MODULE,
		.name = "i2c_adpt-1",
		.algo = &i2c_algo,
		.class = I2C_CLASS_HWMON | I2C_CLASS_SPD,
	},
	{
		.owner = THIS_MODULE,
		.name = "i2c_adpt-2",
		.algo = &i2c_algo,
		.class = I2C_CLASS_HWMON | I2C_CLASS_SPD,
	},
};
static const int i2c_adpts_num = sizeof(i2c_adpts) / sizeof(i2c_adpts[0]);
static int i2c_adpts_cnt = 0;

static size_t adpt_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	int i;
	pr_info("%d i2c adapters%c\n", i2c_adpts_cnt, i2c_adpts_cnt > 0 ? ':' : '.');
	for (i = 0; i < i2c_adpts_cnt; ++i)
		pr_info("%s\n", i2c_adpts[i].name);
	return 0;
}
static ssize_t adpt_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	if (IS_CMD_ADD(buf) && i2c_adpts_cnt < i2c_adpts_num) {
		if (i2c_add_adapter(&i2c_adpts[i2c_adpts_cnt]))
			pr_debug("i2c adapter added failed(%s)!\n", i2c_adpts[i2c_adpts_cnt].name);
		else
			i2c_adpts_cnt++;
	} else if (IS_CMD_DEL(buf) && i2c_adpts_cnt > 0) {
		i2c_del_adapter(&i2c_adpts[i2c_adpts_cnt - 1]);
		i2c_adpts_cnt--;
	}
	return count;
}
static struct kobj_attribute adpt_attr = {
	.attr = {
		.name = "adapter",
		.mode = 0660,
	},
	.show = &adpt_show,
	.store = &adpt_store,
};

struct _dev_info {
	struct i2c_board_info info;
	struct i2c_client *cli;
};
static struct _dev_info i2c_infos[] = {
	{ { I2C_BOARD_INFO("mydrv1-i2c1", 0x11) }, NULL },
	{ { I2C_BOARD_INFO("mydrv1-i2c2", 0x12) }, NULL },
	{ { I2C_BOARD_INFO("mydrv2-i2c1", 0x13) }, NULL },
	{ { I2C_BOARD_INFO("mydrv2-i2c2", 0x14) }, NULL },
};
static const int i2c_infos_num = sizeof(i2c_infos) / sizeof(i2c_infos[0]);
static int i2c_infos_cnt = 0;

static size_t dev_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	int i;
	pr_info("%d i2c clients%c\n", i2c_infos_cnt, i2c_infos_cnt > 0 ? ':' : '.');
	for (i = 0; i < i2c_infos_cnt; ++i)
		pr_info("%s\n", i2c_infos[i].cli->name);
	return 0;
}
static ssize_t dev_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	if (IS_CMD_ADD(buf) && i2c_infos_cnt < i2c_infos_num){
		if (i2c_adpts_cnt <= 0) {
			pr_debug("i2c adapter not exist!\n");
			return count;
		}
		int nr = 0;
		const char *pn = strstr(buf, "=");
		if (pn)
			nr = simple_strtoul(pn, NULL, 10);
		nr = nr < i2c_adpts_cnt ? nr : 0;
		struct _dev_info *pinfo = &i2c_infos[i2c_infos_cnt];
		pinfo->cli = i2c_new_device(&i2c_adpts[nr], &pinfo->info);
		if (!pinfo->cli) {
			pr_debug("i2c device added failed(%s)!\n", pinfo->info.type);
		} else {
			i2c_infos_cnt++;
		}
	} else if (IS_CMD_DEL(buf) && i2c_infos_cnt > 0) {
		i2c_unregister_device(i2c_infos[i2c_infos_cnt - 1].cli);
		i2c_infos[i2c_infos_cnt - 1].cli = NULL;
		i2c_infos_cnt--;
	} else if (IS_CMD_CLR(buf)) {
		int i;
		for (i = 0; i < i2c_infos_cnt; ++i) {
			i2c_unregister_device(i2c_infos[i].cli);
			i2c_infos[i].cli = NULL;
		}
		i2c_infos_cnt = 0;
	} else {
		pr_info("Support commands:\n"
				"\"add\" or \"add={number}\"\n"
				"\"del\"\n"
				"\"clr\"\n");
	}
	return count;
}
static struct kobj_attribute dev_attr = {
	.attr = {
		.name = "device",
		.mode = 0660,
	},
	.show = &dev_show,
	.store = &dev_store,
};

static struct attribute *i2c_attrs[] = {
	&drv_attr.attr,
	&adpt_attr.attr,
	&dev_attr.attr,
	NULL
};
static struct attribute_group i2c_group = {
	.attrs = i2c_attrs,
};
static struct kobject *i2c_node = NULL;
int i2c_nodes_add(void)
{
	int retval = 0;
	i2c_node = kobject_create_and_add("i2c", root_node);
	if (NULL == i2c_node) {
		pr_debug("kobject created failed!\n");
		return -ENODEV;
	}
	if ((retval = sysfs_create_group(i2c_node, &i2c_group))) {
		pr_debug("group create failed!\n");
		goto err1;
	}
	return 0;
err1:
	kobject_del(i2c_node);
	return retval;
}
void i2c_nodes_del(void)
{
	while (i2c_infos_cnt > 0)
		i2c_unregister_device(i2c_infos[i2c_infos_cnt-- - 1].cli);
	while (i2c_adpts_cnt > 0)
		i2c_del_adapter(&i2c_adpts[i2c_adpts_cnt-- - 1]);
	while (i2c_drvs_cnt > 0)
		i2c_del_driver(&i2c_drvs[i2c_drvs_cnt-- - 1]);

	if (i2c_node)
		kobject_del(i2c_node);
}

