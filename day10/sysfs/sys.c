#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kdev_t.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/device.h>

#define SYS_FS "sysfs_entry"

static dev_t dev;
volatile int sysfs_value;

static struct class    *class_creat;
static struct device   *device_creat;
static struct kobject  *kobj;

static struct cdev      sys_cdev;

//Function prototypes -- start
//sysfs has two apis associated with it -- _show()  - for reading
//					   _store() - for writing
//

static ssize_t sysfs_show(struct kobject *dev,struct kobj_attribute *attr,
				char *buf);
static ssize_t sysfs_store(struct kobject *dev,struct kobj_attribute *attr,
				const char *buf, size_t count);

//Function prototypes -- end

static ssize_t sysfs_show(struct kobject *dev,struct kobj_attribute *attr,
				char *buf){
	pr_info("SYSFS_READ\n");
	return sprintf(buf, "%d", sysfs_value);

}

static ssize_t sysfs_store(struct kobject *dev,struct kobj_attribute *attr,
				const char *buf, size_t count){
	pr_info("SYSFS_WRITE\n");
	sscanf(buf, "%d", &sysfs_value);
	return count;
}

struct kobj_attribute kobj_arr = __ATTR(sysfs_value, 0660, sysfs_show, sysfs_store);

struct file_operations fops = {
	.owner = THIS_MODULE
};

static int __init sys_init(void){
	
	int ret = 0;

	ret = alloc_chrdev_region(&dev, 0, 1, SYS_FS);	
	if(ret){
		pr_err("registering character device failed\n");
		return -1;
	}
	
	cdev_init(&sys_cdev, &fops);
	sys_cdev.owner = THIS_MODULE;

	ret = cdev_add(&sys_cdev, dev, 1);
	if(ret){
		pr_err("cdev_add failed\n");
		return -1;
	}

	class_creat = class_create(SYS_FS);
	if(IS_ERR(class_creat)){
		pr_err("Failed to create class: %s\n",SYS_FS);
		return -1;
	}

	device_creat = device_create(class_creat,NULL,dev,NULL,SYS_FS);
	if(IS_ERR(device_creat)){
		pr_err("Failed to create device\n");
		return -1;
	}

	kobj = kobject_create_and_add(SYS_FS, kernel_kobj);
	ret = sysfs_create_file(kobj, &kobj_arr.attr);
	printk(KERN_EMERG "ret val : %d\n",ret);
	if(ret){
		pr_err("Sysfs file creation failed\n");
		goto sys_f;
		return -1;

	}
sys_f:
	kobject_put(kobj);
	sysfs_remove_file(kobj, &kobj_arr.attr);
	device_destroy(class_creat, dev);
	class_destroy(class_creat);

	return 0;
} 

static void __exit sys_exit(void){
	
	kobject_put(kobj);
	sysfs_remove_file(kobj, &kobj_arr.attr);
	device_destroy(class_creat, dev);
	class_destroy(class_creat);
}

module_init(sys_init);
module_exit(sys_exit);

MODULE_LICENSE("GPL");
