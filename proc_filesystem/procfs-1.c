#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#define procfs_name "hello_world"

ssize_t procfs_read(struct file *, char __user *, size_t, loff_t *);

struct proc_dir_entry *Proc_File;
static char msg[80];
static char *msg_ptr;

const struct file_operations proc_fops = {
    .owner = THIS_MODULE,
    .read = procfs_read
};

ssize_t procfs_read(struct file *filp, char __user *buffer, size_t length,
        loff_t *offset) {
    int ret;

    printk(KERN_INFO "procfs_read (/proc/%s) called\n", procfs_name);
    if (*offset > 0) {
        /* read has been finished */
        ret = 0;
    }
    else {
        /* fill the buffer */
        ret = 0;
        msg_ptr = msg;
        while (*msg_ptr && ret < length) {
            put_user(*(msg_ptr++), buffer++);
            ret++;
        }
    }

    *offset += ret;
    return ret;
}

int init_module(void) {
    Proc_File = proc_create(procfs_name, 0644, NULL, &proc_fops);
    if (Proc_File == NULL) {
        return -ENOMEM;
    }
    sprintf(msg, "Hello World!\n");

    printk(KERN_INFO "/proc/%s created\n", procfs_name);
    return 0;
}

void cleanup_module(void) {
    // remove_proc_entry(procfs_name, Proc_File);
    proc_remove(Proc_File);
    printk(KERN_INFO "/proc/%s removed\n", procfs_name);
}
