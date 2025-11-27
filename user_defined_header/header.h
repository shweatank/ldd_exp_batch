#ifndef HEADER_H
#define HEADER_H

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/types.h>

#define DEVICE_NAME "simple_char_dev"
#define BUFFER_SIZE 256

extern int major;
extern char buffer[BUFFER_SIZE];
extern int buffer_size;

static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos);
static ssize_t my_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos);

extern struct file_operations fops;

static int __init simple_init(void);
static void __exit simple_exit(void);

#endif