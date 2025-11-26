//Function prototypes
static int my_open(struct inode *inode, struct file *file );
static int my_close(struct inode *inode, struct file *file);

ssize_t drv_read(struct file *file, char __user *user_buf, 
			size_t count, loff_t *ppos);

ssize_t drv_write(struct file *file, const char __user *user_buf, 				size_t count, loff_t *ppos);

int countt = 0;
