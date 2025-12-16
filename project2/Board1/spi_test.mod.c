#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x2c992cbf, "__spi_register_driver" },
	{ 0x92997ed8, "_printk" },
	{ 0x8b970f46, "device_destroy" },
	{ 0x6775d5d3, "class_destroy" },
	{ 0x27271c6b, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x92893115, "driver_unregister" },
	{ 0xdcb764ad, "memset" },
	{ 0x4e660045, "spi_sync" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xb00e4280, "gpiod_set_value_cansleep" },
	{ 0xf9a482f9, "msleep" },
	{ 0xc3055d20, "usleep_range_state" },
	{ 0x36a78de3, "devm_kmalloc" },
	{ 0x240ed2c9, "device_property_read_u32_array" },
	{ 0x3bb3b979, "_dev_info" },
	{ 0x660806e0, "spi_setup" },
	{ 0xbbd8f39f, "_dev_err" },
	{ 0x3250fd9c, "devm_gpiod_get" },
	{ 0x991fb4bf, "gpiod_set_value" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xa01f13a6, "cdev_init" },
	{ 0x3a6d85d3, "cdev_add" },
	{ 0x59c02473, "class_create" },
	{ 0x2c9a4c10, "device_create" },
	{ 0x52c5c991, "__kmalloc_noprof" },
	{ 0x37a0cba, "kfree" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0x474e54d2, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Cilitek,ili9225");
MODULE_ALIAS("of:N*T*Cilitek,ili9225C*");

MODULE_INFO(srcversion, "EAAF0D375E640E4AA54947B");
