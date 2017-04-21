/*
 * Add inode_operations for permission checking.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <asm/current.h>
#include <linux/cred.h>

#define min_v(a,b) (a>b?b:a)

#define PROCFS_NAME     "hello_world"
#define PROCFS_MAX_SIZE 1024

struct task_struct;

ssize_t procfs_read(struct file *, char __user *, size_t, loff_t *);
ssize_t procfs_write(struct file *, const char __user *, size_t, loff_t *);
int procfs_open(struct inode *, struct file *);
int procfs_release(struct inode *, struct file *);
int module_permission(struct inode *, int);

struct proc_dir_entry *Proc_File;
static char procfs_buffer[PROCFS_MAX_SIZE];
static size_t procfs_buffer_size = 0;

const struct file_operations proc_fops = {
    .owner = THIS_MODULE,
    .open = procfs_open,
    .release = procfs_release,
    .read = procfs_read,
    .write = procfs_write
};

const struct inode_operations proc_iops = {
    .permission = module_permission
};

int module_permission(struct inode *inode, int op) {
    /*
     * Allow everybody to read, but only root to write
     */
    printk(KERN_INFO "procfs permission checking\n");
    if (op == 4 || (op == 2 && current_euid().val == 0))
        return 0;
    else
        return -EACCES;
}

int procfs_open(struct inode *inode, struct file *filp) {
    try_module_get(THIS_MODULE);
    return 0;
}

int procfs_release(struct inode *inode, struct file *filp) {
    module_put(THIS_MODULE);
    return 0;
}

ssize_t procfs_read(struct file *filp, char __user *buffer, size_t length,
        loff_t *offset) {
    int ret;

    printk(KERN_INFO "procfs_read (/proc/%s) called\n", PROCFS_NAME);
    if (*offset > 0) {
        /* read has been finished */
        ret = 0;
    }
    else {
        /* fill the buffer */
        ret = min_v(length, procfs_buffer_size);
        copy_to_user(buffer, procfs_buffer, ret);
//      ret = 0;
//      msg_ptr = msg;
//      while (*msg_ptr && ret < length) {
//          put_user(*(msg_ptr++), buffer++);
//          ret++;
//      }
    }

    *offset += ret;
    return ret;
}

ssize_t procfs_write(struct file *filp, const char __user *buffer,
        size_t length, loff_t *offset) {
    procfs_buffer_size = min_v(length, PROCFS_MAX_SIZE);
    if (copy_from_user(procfs_buffer, buffer, procfs_buffer_size))
        return -EFAULT;

    return procfs_buffer_size;
}

int init_module(void) {
    Proc_File = proc_create(PROCFS_NAME, 0644, NULL, &proc_fops);
    if (Proc_File == NULL) {
        return -ENOMEM;
    }
    /*
     * General purpose of /proc filesystem is to give kernel and its
     * modules ability to easily create files with "content" generated
     * on the fly and directories for group these files. One may do that
     * by using proc_create() and friends.
     *
     * As for inodes and dentries, they are part of filesystem's
     * internals: it is better to not modify them and their operations.
     *
     * Actually, file_operations are powerful by themselves. If you find
     * mode parameter for proc_create() insufficient to reflect access
     * rights, you may check access in .open() file operation.
     */
    // Proc_File->proc_iops = proc_iops;
    sprintf(procfs_buffer, "Hello World!\n");
    procfs_buffer_size = strlen(procfs_buffer) + 1;

    printk(KERN_INFO "/proc/%s created\n", PROCFS_NAME);
    return 0;
}

void cleanup_module(void) {
    // remove_proc_entry(procfs_name, Proc_File);
    proc_remove(Proc_File);
    printk(KERN_INFO "/proc/%s removed\n", PROCFS_NAME);
}
