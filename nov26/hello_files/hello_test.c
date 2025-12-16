#include "hello_test.h"


/*static int __init hello_init(void)
  {
      int ret = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops);
         if(ret < 0){
            pr_err("failed to register char device\n");
            return ret;
          }
    pr_info("ioctl_demo = module loaded , major %d\n",MAJOR_NUM);*/
	pr_info("hello world\n");
    return 0;
  }
  
  static void __exit hello_exit(void)
  {
  //   unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
     pr_info("hello_world exit\n");
   }
  
 
  module_init(hello_init);
  module_exit(hello_exit);
  
  MODULE_LICENSE("GPL");
  MODULE_AUTHOR("pavan");
  MODULE_DESCRIPTION("simple hello world Example");

