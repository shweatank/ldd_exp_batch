#include <linux/module.h>  // Core header for LKM
#include <linux/kernel.h>  // Contains types, macros, functions for the kernel
#include <linux/init.h>    // Used for init and exit macros
#include <linux/mutex.h>   // For mutex functionality

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple mutex example");

// Define a mutex
static DEFINE_MUTEX(my_mutex);


// Function to be executed when the module is loaded (initialized)
static int __init mutex_example_init(void)
{
	printk(KERN_INFO "Mutex Example: Initializing module\n");

	// Acquire the mutex
	printk(KERN_INFO "Mutex Example: Attempting to acquire mutex\n");
	mutex_lock(&my_mutex);
	printk(KERN_INFO "Mutex Example: Mutex acquired\n");

	// Simulate some work being done while holding the mutex
	printk(KERN_INFO "Mutex Example: Doing some work while holding the mutex\n");
	// In a real scenario, this could be accessing a shared resource

	// Release the mutex
	printk(KERN_INFO "Mutex Example: Releasing mutex\n");
	mutex_unlock(&my_mutex);
	printk(KERN_INFO "Mutex Example: Mutex released\n");

	return 0;  
}


// Function to be executed when the module is unloaded (exited)
static void __exit mutex_example_exit(void)
{
	printk(KERN_INFO "Mutex Example: Exiting module\n");
}


// Register init and exit functions
module_init(mutex_example_init);
module_exit(mutex_example_exit);

