#include <linux/module.h>  // Core header for modules
#include <linux/kernel.h>  // Kernel functions like printk
#include <linux/kthread.h> // For creating kernel threads
#include <linux/delay.h>   // For using msleep

static struct task_struct *my_thread1;
static struct task_struct *my_thread2;

static int count = 0;

static int thread_function(void *data)
{
	int thread_data = *(int *)data;
	count++;

	while (!kthread_should_stop()) {
		printk(KERN_INFO "Kernel thread says: Hello from thread%d count = %d\n", thread_data, count);
		msleep(1000); // Sleep for 1 second
	}
	printk(KERN_INFO "Kernel thread: Thread is stopping\n");
	return 0;
}


static int my_init(void)
{

	static int data1 = 1;
	static int data2 = 2;


	printk(KERN_INFO "Creating kernel thread\n");
	/*
	//Old fashioned just reference
	my_thread1 = kthread_create(thread_function, &data1, "my_kernel_thread1");
	wake_up_process(my_thread1);
	*/
	my_thread1 = kthread_run(thread_function, &data1, "my_kernel_thread1");
	my_thread2 = kthread_run(thread_function, &data2, "my_kernel_thread2");


	if (my_thread1 && my_thread2) {
		printk(KERN_INFO "Kernel threads created successfully\n");
	} else {
		printk(KERN_ERR "Failed to create kernel threads\n");
		return -ENOMEM;
	}

	return 0;
}


static void my_exit(void)
{
	printk(KERN_INFO "Stopping kernel thread\n");
	if (my_thread1) {
		kthread_stop(my_thread1);
	}
	if(my_thread2) {
		kthread_stop(my_thread2);
		printk(KERN_INFO "Kernel thread stopped\n");
	}
}


module_init(my_init);
module_exit(my_exit);


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple kernel thread example");

