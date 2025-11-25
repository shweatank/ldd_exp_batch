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
	{ 0x32feeafc, "mod_timer" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0x67628f51, "msleep" },
	{ 0x5e505530, "kthread_should_stop" },
	{ 0x7851be11, "__SCT__might_resched" },
	{ 0x7a5ffe84, "init_wait_entry" },
	{ 0xd272d446, "schedule" },
	{ 0x0db8d68d, "prepare_to_wait_event" },
	{ 0xc87f4bab, "finish_wait" },
	{ 0xd272d446, "__stack_chk_fail" },
	{ 0x5403c125, "__init_waitqueue_head" },
	{ 0xef959adb, "kthread_create_on_node" },
	{ 0x9a9aee9e, "wake_up_process" },
	{ 0x02f9bbf0, "init_timer_key" },
	{ 0x8697f569, "kthread_stop" },
	{ 0x2352b148, "timer_delete_sync" },
	{ 0xd272d446, "__fentry__" },
	{ 0xe8213e80, "_printk" },
	{ 0x16ab4215, "__wake_up" },
	{ 0x058c185a, "jiffies" },
	{ 0xd268ca91, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0x32feeafc,
	0xd272d446,
	0x67628f51,
	0x5e505530,
	0x7851be11,
	0x7a5ffe84,
	0xd272d446,
	0x0db8d68d,
	0xc87f4bab,
	0xd272d446,
	0x5403c125,
	0xef959adb,
	0x9a9aee9e,
	0x02f9bbf0,
	0x8697f569,
	0x2352b148,
	0xd272d446,
	0xe8213e80,
	0x16ab4215,
	0x058c185a,
	0xd268ca91,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"mod_timer\0"
	"__x86_return_thunk\0"
	"msleep\0"
	"kthread_should_stop\0"
	"__SCT__might_resched\0"
	"init_wait_entry\0"
	"schedule\0"
	"prepare_to_wait_event\0"
	"finish_wait\0"
	"__stack_chk_fail\0"
	"__init_waitqueue_head\0"
	"kthread_create_on_node\0"
	"wake_up_process\0"
	"init_timer_key\0"
	"kthread_stop\0"
	"timer_delete_sync\0"
	"__fentry__\0"
	"_printk\0"
	"__wake_up\0"
	"jiffies\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "6754962DFB4A5F83D728599");
