#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>

int value_int;
int value_array[4];
char *name_str;
int cb_value = 0;

// integer param
module_param(value_int, int, S_IRUSR | S_IWUSR);

// string param
module_param(name_str, charp, S_IRUSR | S_IWUSR);

// array param
module_param_array(value_array, int, NULL, S_IRUSR | S_IWUSR);

static int __init start_init(void)
{
    int i;

    printk(KERN_INFO "value_int = %d\n", value_int);
    printk(KERN_INFO "cb_value = %d\n", cb_value);
    printk(KERN_INFO "name_str = %s\n", name_str);

    for (i = 0; i < (sizeof(value_array) / sizeof(int)); i++) {
        printk(KERN_INFO "value_array[%d] = %d\n", i, value_array[i]);
    }

    printk(KERN_INFO "kernel module inserted\n");
    return 0;
}

static void __exit end_init(void)
{
    printk(KERN_INFO "kernel module removed\n");
}

module_init(start_init);
module_exit(end_init);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pawan");
MODULE_DESCRIPTION("Module param example");

