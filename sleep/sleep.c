#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>

#define PROCFS_NAME "sleep"
#define MSG_LEN     80

static char Message[MSG_LEN];
static int Already_Open = 0;

static struct proc_dir_entry *Proc_File;
DECLARE_WAIT_QUEUE_HEAD(WaitQ);

ssize_t module_output(struct file *filp, char __user *buf,
        size_t len, loff_t *offset) {
    static int finished = 0;
    int i;
    char message[MSG_LEN + 30];

    if (finished) {
        finished = 0;
        return 0;
    }

    sprintf(message, "Last input: %s\n", Message);
    for (i = 0; i < len && message[i]; i++)
        put_user(message[i], buf + i);
    finished = 1;
    return i;
}

ssize_t module_input(struct file *filp, const char __user *buf,
        size_t len, loff_t *offset) {
    int i;
    for (i = 0; i < MSG_LEN - 1 && i < len; i++)
        get_user(Message[i], buf + i);
    Message[i] = '\0';
    return i;
}

int module_open(struct inode *inode, struct file *filp) {
    if ((filp->f_flags & O_NONBLOCK) && Already_Open)
        return -EAGAIN;

    try_module_get(THIS_MODULE);
    while (Already_Open) {
        int i, is_sig = 0;
        wait_event_interruptible(WaitQ, !Already_Open);

        /* Signals are contained in two words (64 bits) */
        for (i = 0; i < _NSIG_WORDS && !is_sig; i++)
            is_sig = current->pending.signal.sig[i]
                & ~current->blocked.sig[i];
        if (is_sig) {
            module_put(THIS_MODULE);
            return -EINTR;
        }
    }

    Already_Open = 1; /* only one process enters here */
    return 0;
}

int module_release(struct inode *inode, struct file *filp) {
    Already_Open = 0;
    wake_up(&WaitQ);
    module_put(THIS_MODULE);
    return 0;
}

static struct file_operations procfs_fops = {
    .owner = THIS_MODULE,
    .read = module_output,
    .write = module_input,
    .open = module_open,
    .release = module_release
};

int init_module(void) {
    Proc_File = proc_create(PROCFS_NAME, 0644, NULL, &procfs_fops);
    if (Proc_File == NULL)
        return -ENOMEM;

    printk(KERN_INFO "/proc/%s created\n", PROCFS_NAME);
    return 0;
}

void cleanup_module(void) {
    proc_remove(Proc_File);
    printk(KERN_INFO "/proc/%s removed\n", PROCFS_NAME);
}
