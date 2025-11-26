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
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0xdd6830c7, "sprintf" },
	{ 0x3cd2d66b, "class_create" },
	{ 0x2035aec5, "device_create" },
	{ 0xf78db457, "kernel_kobj" },
	{ 0xe90293c6, "kobject_create_and_add" },
	{ 0xc9286f86, "sysfs_create_file_ns" },
	{ 0x91421947, "kobject_put" },
	{ 0xe2cb05dc, "sysfs_remove_file_ns" },
	{ 0x5d1ebb98, "device_destroy" },
	{ 0x690fb1ed, "class_destroy" },
	{ 0xd272d446, "__fentry__" },
	{ 0xe8213e80, "_printk" },
	{ 0x173ec8da, "sscanf" },
	{ 0xd268ca91, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xd272d446,
	0xdd6830c7,
	0x3cd2d66b,
	0x2035aec5,
	0xf78db457,
	0xe90293c6,
	0xc9286f86,
	0x91421947,
	0xe2cb05dc,
	0x5d1ebb98,
	0x690fb1ed,
	0xd272d446,
	0xe8213e80,
	0x173ec8da,
	0xd268ca91,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"__x86_return_thunk\0"
	"sprintf\0"
	"class_create\0"
	"device_create\0"
	"kernel_kobj\0"
	"kobject_create_and_add\0"
	"sysfs_create_file_ns\0"
	"kobject_put\0"
	"sysfs_remove_file_ns\0"
	"device_destroy\0"
	"class_destroy\0"
	"__fentry__\0"
	"_printk\0"
	"sscanf\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "22E70985B5F2523651ACFF4");
