#ifndef DRIVER_H
#define DRIVER_H

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>

#define DEVICE_NAME	"calc_char_dev"

struct data {
	int n1;
	int n2;
	char op;
};

#define BUF_SIZE	20

static int major;
static struct data buffer;
int result;

/*Prototypes*/
static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos);
static ssize_t my_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos);


#endif
