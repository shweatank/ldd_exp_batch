#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
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

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const char ____versions[]
__used __section("__versions") =
	"\x1c\x00\x00\x00\xdc\x59\xa4\xb5"
	"unregister_blkdev\0\0\0"
	"\x10\x00\x00\x00\x97\x82\x9e\x99"
	"vfree\0\0\0"
	"\x10\x00\x00\x00\xba\x0c\x7a\x03"
	"kfree\0\0\0"
	"\x10\x00\x00\x00\x7e\x3a\x2c\x12"
	"_printk\0"
	"\x20\x00\x00\x00\xa1\xc8\x3f\x52"
	"blk_mq_start_request\0\0\0\0"
	"\x10\x00\x00\x00\x38\xdf\xac\x69"
	"memcpy\0\0"
	"\x18\x00\x00\x00\x6c\x1e\x65\x97"
	"vmemmap_base\0\0\0\0"
	"\x1c\x00\x00\x00\x5e\xd7\xd8\x7c"
	"page_offset_base\0\0\0\0"
	"\x1c\x00\x00\x00\xe5\xaf\x67\xf9"
	"blk_mq_end_request\0\0"
	"\x1c\x00\x00\x00\x63\xa5\x03\x4c"
	"random_kmalloc_seed\0"
	"\x18\x00\x00\x00\x19\x08\xda\x08"
	"kmalloc_caches\0\0"
	"\x18\x00\x00\x00\x4c\x48\xc3\xd0"
	"kmalloc_trace\0\0\0"
	"\x10\x00\x00\x00\x8f\x68\xee\xd6"
	"vmalloc\0"
	"\x10\x00\x00\x00\xc5\x8f\x57\xfb"
	"memset\0\0"
	"\x1c\x00\x00\x00\xa7\x27\x0a\x72"
	"__register_blkdev\0\0\0"
	"\x20\x00\x00\x00\x24\xeb\x8b\x3d"
	"blk_mq_alloc_tag_set\0\0\0\0"
	"\x1c\x00\x00\x00\x7c\x65\xf1\x6f"
	"blk_mq_init_queue\0\0\0"
	"\x28\x00\x00\x00\xc4\x62\xa9\x3f"
	"blk_queue_logical_block_size\0\0\0\0"
	"\x1c\x00\x00\x00\x30\x30\x63\x4e"
	"__blk_mq_alloc_disk\0"
	"\x18\x00\x00\x00\xe2\x5b\x56\xb9"
	"set_capacity\0\0\0\0"
	"\x18\x00\x00\x00\x06\x47\xaf\x80"
	"device_add_disk\0"
	"\x18\x00\x00\x00\xbb\x90\x9b\x47"
	"param_ops_int\0\0\0"
	"\x14\x00\x00\x00\xbb\x6d\xfb\xbd"
	"__fentry__\0\0"
	"\x1c\x00\x00\x00\xca\x39\x82\x5b"
	"__x86_return_thunk\0\0"
	"\x14\x00\x00\x00\x74\x47\x58\x64"
	"del_gendisk\0"
	"\x14\x00\x00\x00\x0a\x6c\xa7\x0d"
	"put_disk\0\0\0\0"
	"\x20\x00\x00\x00\x20\xd9\xe4\x90"
	"blk_mq_destroy_queue\0\0\0\0"
	"\x1c\x00\x00\x00\x81\xfb\x94\x5f"
	"blk_mq_free_tag_set\0"
	"\x18\x00\x00\x00\xeb\x7b\x33\xe1"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "9A8C7C4355AA1FBE807998F");
