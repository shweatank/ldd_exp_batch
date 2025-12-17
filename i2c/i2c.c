// SPDX-License-Identifier: GPL-2.0
/*
 * my_i2c_sensor.c - Simple I2C client driver example for Raspberry Pi 4B
 *
 * Binds to a device with:
 *   compatible = "techdhaba,my-i2c-sensor";
 *   reg = <0x48>;
 *
 * Sysfs attributes (under /sys/bus/i2c/devices/1-0048/):
 *   reg (rw) : register address (0-255)
 *   val (rw) : read/write 1-byte value from/to current "reg"
 *
 * This assumes the underlying I2C controller (i2c-bcm2835) is already enabled.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
#include <linux/device.h>

struct my_i2c_data {
	struct i2c_client *client;
	struct mutex lock;   /* protects reg/val transactions */
	u8 current_reg;
};

static ssize_t reg_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct my_i2c_data *data = i2c_get_clientdata(client);
	u8 reg;

	mutex_lock(&data->lock);
	reg = data->current_reg;
	mutex_unlock(&data->lock);

	return scnprintf(buf, PAGE_SIZE, "0x%02x\n", reg);
}

static ssize_t reg_store(struct device *dev,
			 struct device_attribute *attr,
			 const char *buf, size_t count)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct my_i2c_data *data = i2c_get_clientdata(client);
	unsigned int reg;
	int ret;

	ret = kstrtouint(buf, 0, &reg);
	if (ret)
		return ret;

	if (reg > 0xFF)
		return -EINVAL;

	mutex_lock(&data->lock);
	data->current_reg = (u8)reg;
	mutex_unlock(&data->lock);

	return count;
}

static ssize_t val_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct my_i2c_data *data = i2c_get_clientdata(client);
	s32 ret;
	u8 value;

	mutex_lock(&data->lock);
	ret = i2c_smbus_read_byte_data(client, data->current_reg);
	mutex_unlock(&data->lock);

	if (ret < 0)
		return ret;

	value = (u8)ret;
	return scnprintf(buf, PAGE_SIZE, "0x%02x\n", value);
}

static ssize_t val_store(struct device *dev,
			 struct device_attribute *attr,
			 const char *buf, size_t count)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct my_i2c_data *data = i2c_get_clientdata(client);
	unsigned int value;
	int ret;

	ret = kstrtouint(buf, 0, &value);
	if (ret)
		return ret;

	if (value > 0xFF)
		return -EINVAL;

	mutex_lock(&data->lock);
	ret = i2c_smbus_write_byte_data(client, data->current_reg,
					(u8)value);
	mutex_unlock(&data->lock);

	if (ret < 0)
		return ret;

	return count;
}

/* Create two sysfs attributes: reg and val */
static DEVICE_ATTR_RW(reg);
static DEVICE_ATTR_RW(val);

static struct attribute *my_i2c_attrs[] = {
	&dev_attr_reg.attr,
	&dev_attr_val.attr,
	NULL,
};

static const struct attribute_group my_i2c_attr_group = {
	.attrs = my_i2c_attrs,
};

static int my_i2c_probe(struct i2c_client *client)
{
	struct my_i2c_data *data;
	int ret;

	dev_info(&client->dev, "my_i2c_sensor probe: addr=0x%02x on adapter %s\n",
		 client->addr, dev_name(&client->adapter->dev));

	data = devm_kzalloc(&client->dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->client = client;
	data->current_reg = 0x00;
	mutex_init(&data->lock);

	i2c_set_clientdata(client, data);

	/* Optional: quick check - try reading 1 byte from reg 0 */
	ret = i2c_smbus_read_byte_data(client, data->current_reg);
	if (ret < 0) {
		dev_warn(&client->dev,
			 "device didn't respond at reg 0x00 (ret=%d), continuing anyway\n",
			 ret);
	}

	/* Create sysfs group */
	ret = sysfs_create_group(&client->dev.kobj, &my_i2c_attr_group);
	if (ret) {
		dev_err(&client->dev, "failed to create sysfs group\n");
		return ret;
	}

	dev_info(&client->dev, "my_i2c_sensor driver bound successfully\n");
	return 0;
}

static void my_i2c_remove(struct i2c_client *client)
{
	sysfs_remove_group(&client->dev.kobj, &my_i2c_attr_group);
	dev_info(&client->dev, "my_i2c_sensor removed\n");
}

static const struct of_device_id my_i2c_of_match[] = {
	{ .compatible = "techdhaba,my-i2c-sensor" },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, my_i2c_of_match);

static const struct i2c_device_id my_i2c_id[] = {
	{ "my_i2c_sensor", 0 },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(i2c, my_i2c_id);

static struct i2c_driver my_i2c_driver = {
	.driver = {
		.name           = "my_i2c_sensor",
		.of_match_table = my_i2c_of_match,
	},
	.probe  = my_i2c_probe,   /* modern kernels */
	.remove     = my_i2c_remove,
	.id_table   = my_i2c_id,
};

module_i2c_driver(my_i2c_driver);

MODULE_AUTHOR("techdhaba");
MODULE_DESCRIPTION("Example I2C client driver for Raspberry Pi 4B");
MODULE_LICENSE("GPL");
