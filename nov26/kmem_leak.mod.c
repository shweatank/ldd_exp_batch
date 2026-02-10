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
	{ 0x122c3a7e, "_printk" },
	{ 0x73900c40, "kmalloc_caches" },
	{ 0xe1478d23, "__kmalloc_cache_noprof" },
	{ 0x95534a9f, "__asan_report_load8_noabort" },
	{ 0x98ec031b, "__asan_unregister_globals" },
	{ 0xf5869226, "__asan_register_globals" },
	{ 0xd08ee9ff, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "0B5DA6362AFB7DB02817E72");
