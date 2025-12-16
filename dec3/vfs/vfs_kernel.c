/*
 * techfs.c - Minimal example VFS filesystem
 *
 * Features:
 *   - In-memory, nodev filesystem
 *   - Single root directory "/"
 *   - Single regular file "/hello" that returns a static string
 *
 * Build:
 *   make -C /lib/modules/$(uname -r)/build M=$PWD modules
 *
 * Usage:
 *   sudo insmod techfs.ko
 *   sudo mkdir -p /mnt/techfs
 *   sudo mount -t techfs none /mnt/techfs
 *   ls -l /mnt/techfs
 *   cat /mnt/techfs/hello
 *
 * Cleanup:
 *   sudo umount /mnt/techfs
 *   sudo rmmod techfs
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/pagemap.h>    /* for simple_dir operations */
#include <linux/mount.h>
#include <linux/slab.h>
#include <linux/string.h>

#define TECHFS_MAGIC 0x74656368  /* "tech" in hex */

static const char techfs_hello_str[] = "Hello from TechFS!\n";

/* ---------- Forward declarations ---------- */

static struct dentry *techfs_mount(struct file_system_type *fs_type,
                                   int flags, const char *dev_name,
                                   void *data);
static int techfs_fill_super(struct super_block *sb, void *data, int silent);

/* ---------- File operations for /hello ---------- */

static ssize_t techfs_hello_read(struct file *filp,
                                 char __user *buf,
                                 size_t len,
                                 loff_t *ppos)
{
    return simple_read_from_buffer(buf, len, ppos,
                                   techfs_hello_str,
                                   strlen(techfs_hello_str));
}

static const struct file_operations techfs_hello_fops = {
    .owner   = THIS_MODULE,
    .read    = techfs_hello_read,
    .llseek  = default_llseek,
    .open    = simple_open,
};

/* ---------- Inode operations for regular files ---------- */

static const struct inode_operations techfs_file_inode_ops = {
    /* nothing special, regular file with simple fops */
};

/* ---------- Lookup + iterate for root directory ---------- */

/*
 * Our filesystem has exactly one file: "hello"
 *
 * - readdir: always shows: ".", "..", "hello"
 * - lookup: creates an inode for "hello" on demand
 */

static int techfs_readdir(struct file *file, struct dir_context *ctx)
{
    /* Emit "." and ".." using helper; then our single file. */
    if (!dir_emit_dots(file, ctx))
        return 0;

    /* We want to emit only once when ctx->pos reaches 2 */
    if (ctx->pos == 2) {
        if (!dir_emit(ctx, "hello", 5, 1, DT_REG))
            return 0;
        ctx->pos++;
    }

    return 0;
}

static const struct file_operations techfs_dir_fops = {
    .owner          = THIS_MODULE,
    .iterate_shared = techfs_readdir,
    .llseek         = default_llseek,
};

static struct inode *techfs_make_inode(struct super_block *sb, umode_t mode)
{
    struct inode *inode;

    inode = new_inode(sb);
    if (!inode)
        return NULL;

    inode->i_ino = get_next_ino();
    inode_init_owner(&init_user_ns, inode, NULL, mode);
    inode->i_atime = inode->i_mtime = inode->i_ctime = current_time(inode);

    return inode;
}

/* Lookup is called when VFS wants child "name" under a directory dentry */
static struct dentry *techfs_lookup(struct inode *dir,
                                    struct dentry *dentry,
                                    unsigned int flags)
{
    struct inode *inode = NULL;

    /* We only support one file named "hello" under the root */
    if (strcmp(dentry->d_name.name, "hello") == 0) {
        inode = techfs_make_inode(dir->i_sb, S_IFREG | 0444);
        if (!inode)
            return ERR_PTR(-ENOMEM);

        inode->i_fop = &techfs_hello_fops;
        inode->i_op  = &techfs_file_inode_ops;
    } else {
        /* Any other name: no entry */
        return ERR_PTR(-ENOENT);
    }

    d_add(dentry, inode);
    return NULL;
}

static const struct inode_operations techfs_dir_inode_ops = {
    .lookup = techfs_lookup,
};

/* ---------- Superblock operations ---------- */

static const struct super_operations techfs_super_ops = {
    .statfs     = simple_statfs,
    .drop_inode = generic_delete_inode,
};

/* Fill superblock when filesystem is mounted */
static int techfs_fill_super(struct super_block *sb, void *data, int silent)
{
    struct inode *root_inode;
    struct dentry *root_dentry;

    sb->s_magic = TECHFS_MAGIC;
    sb->s_op    = &techfs_super_ops;

    /* Create root inode */
    root_inode = techfs_make_inode(sb, S_IFDIR | 0755);
    if (!root_inode)
        return -ENOMEM;

    root_inode->i_op  = &techfs_dir_inode_ops;
    root_inode->i_fop = &techfs_dir_fops;

    root_dentry = d_make_root(root_inode);
    if (!root_dentry)
        return -ENOMEM;

    sb->s_root = root_dentry;
    return 0;
}

/* Mount entry point: nodev filesystem (no backing device) */
static struct dentry *techfs_mount(struct file_system_type *fs_type,
                                   int flags, const char *dev_name,
                                   void *data)
{
    return mount_nodev(fs_type, flags, data, techfs_fill_super);
}

/* ---------- File system type registration ---------- */

static struct file_system_type techfs_fs_type = {
    .owner   = THIS_MODULE,
    .name    = "techfs",
    .mount   = techfs_mount,
    .kill_sb = kill_anon_super,   /* for nodev fs */
};

static int __init techfs_init(void)
{
    int ret;

    pr_info("techfs: registering filesystem\n");
    ret = register_filesystem(&techfs_fs_type);
    if (ret) {
        pr_err("techfs: register_filesystem failed (%d)\n", ret);
        return ret;
    }

    return 0;
}

static void __exit techfs_exit(void)
{
    int ret;

    pr_info("techfs: unregistering filesystem\n");
    ret = unregister_filesystem(&techfs_fs_type);
    if (ret)
        pr_err("techfs: unregister_filesystem failed (%d)\n", ret);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Minimal example VFS filesystem (techfs)");
MODULE_VERSION("1.0");

module_init(techfs_init);
module_exit(techfs_exit);
