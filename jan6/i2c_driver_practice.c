// -- char device -------
 
static ssize_t oled_write(struct file *filep, const char __usr *buf,
                             size_t count, loff_t *f_pos)
{
    char kbuf[128];
    int len = min(count, sizeof())
}

static ssize_t oled_read(struct file *filp, char __user *buf,
                            size_t count, loff_t *f_pos)
{
    const char *msg = "OLED ready\n";
    int len = strlen(msg);
    if(*f_pos >= len)
        return 0;
    if(count > len - *f_pos)
          count = len - *f_pos;
     if(copy_to_user(buf,msg + *f_pos,count))
            return -EFAULT;

      *f_pos += count;
       return count;
}

static const struct file_operations oled_fops = {
     .owner = THIS_MODULE,
     .write = oled_write,
     .read  = oled_read,
};

// ---- i2c driver -------
static int oled_probe(struct i2c_client *client)
{
    int ret;
     oled_device = kzalloc(sizeof(*oled_device), GFP_KERNEL);
     if(!oled_device)
           return -ENOMEM;

     oled_device->client = client;

     ret = ssd1306_send_cmds(clinet, ssd1306_init_cmds,
                             sizeof(ssd1306_init_cmds));
     ssd1306_clear_display(client);
    
     ret = alloc_chrdev_region(&oled_device->devt, 0, 1, DEVICE_NAME);     
     cdev_init(&oled_device->cdev, &oled_fops);
     oled_device->cdev.owner = THIS_MODULE;
     ret = cdev_add(&oled_device->cdev, oled_device->dev_t,1);

     oled_device->class = class_create(DEVICE_NAME);

     device_create(oled_device->class, NULL, oled_device->devt,NULL, DEVICE_NAME);

  dev_info(&client->dev, "SSD1306 OLED initialised\n");
   return 0;
}

static void oled_remove(struct i2c_client *client)
{
   device_destroy(oled_device->class, oled_device->devt);
   class_destroy(oled_device->class);
   cdev_del(&oled_device->cdev);
   unregister_chrdev_region(oled_device->devt, 1);
   kfree(oled_device);
   dev_info(&client->dev, "SSD1306 OLED removed\n");
}

static const struct of_device_id oled_dt_ids[] = {
           { .compatible = "mycompany,my-i2c-oled" },
           { }
};
MODULE_DEVICE_TABLE(of, oled_dt_ids);

static const struct i2c_device_id oled_id[] = {
      {"oled_char", 0 },
      { }
};
MODULE_DEVICE_TABLE(i2c, oled_id);

static struct i2c_driver oled_driver = {
        .driver = {
            .name = "oled_driver",
            .of_match_table = oled_dt_ids,
        },
       .probe = oled_probe,
       .remove = oled_remove,
       .id_table = oled_id,
};

module_i2c_driver(oled_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ssd1306 oled char device driver");
