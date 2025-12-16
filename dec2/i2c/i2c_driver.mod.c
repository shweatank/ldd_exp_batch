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
	{ 0xb90015ae, "i2c_register_driver" },
	{ 0x374b3c83, "sysfs_remove_group" },
	{ 0x3bb3b979, "_dev_info" },
	{ 0x3b6c41ea, "kstrtouint" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0x8dbfeb5a, "i2c_smbus_write_byte_data" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x81718f5f, "i2c_smbus_read_byte_data" },
	{ 0x96848186, "scnprintf" },
	{ 0x92997ed8, "_printk" },
	{ 0x36a78de3, "devm_kmalloc" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0xc5d5e114, "_dev_warn" },
	{ 0xed85cddf, "sysfs_create_group" },
	{ 0xbbd8f39f, "_dev_err" },
	{ 0xcb8314a8, "i2c_del_driver" },
	{ 0x474e54d2, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("i2c:my-i2c-sensor");
MODULE_ALIAS("of:N*T*Cmycompany,my-i2c-oled");
MODULE_ALIAS("of:N*T*Cmycompany,my-i2c-oledC*");

MODULE_INFO(srcversion, "0BFEC50E60900D6CBEEBE80");
