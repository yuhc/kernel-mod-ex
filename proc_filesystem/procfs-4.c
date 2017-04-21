/*
 * Use seq_file to manage /proc file.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/seq_file.h>

#define min_v(a,b) (a>b?b:a)

#define PROCFS_NAME     "hello_world"
#define PROCFS_MAX_SIZE 1024

struct task_struct;

int procfs_open(struct inode *, struct file *);
int procfs_release(struct inode *, struct file *);

static void *seq_start(struct seq_file *m, loff_t *pos);
static void seq_stop(struct seq_file *m, void *v);
static void *seq_next(struct seq_file *m, void *v, loff_t *pos);
static int seq_show(struct seq_file *m, void *v);

struct proc_dir_entry *Proc_File;
static char procfs_buffer[PROCFS_MAX_SIZE];
static size_t procfs_buffer_size = 0;

const struct file_operations proc_fops = {
    .owner = THIS_MODULE,
    .open = procfs_open,
    .release = procfs_release,
    .read = seq_read,
    .llseek = seq_lseek,
};

const struct seq_operations seq_ops = {
    .start = seq_start,
    .next = seq_next,
    .stop = seq_stop,
    .show = seq_show
};

int procfs_open(struct inode *inode, struct file *filp) {
    try_module_get(THIS_MODULE);
    return seq_open(filp, &seq_ops);
}

int procfs_release(struct inode *inode, struct file *filp) {
    int ret = seq_release(inode, filp); 
    module_put(THIS_MODULE);
    return ret;
}

/**
 * Called at the beginning of a sequence, i.e. /proc is read the first
 * time; after seq_stop.
 */
static void *seq_start(struct seq_file *m, loff_t *pos) {
    static unsigned long counter = 0;
    if (*pos == 0) {
        /* begin the sequence */
        return &counter;
    }
    else {
        /* end of a sequence */
        *pos = 0;
        return NULL;
    }

}

static void seq_stop(struct seq_file *m, void *v) {
    /* Nothing to do */
}

/**
 * Called after the beginning of a sequence, until the end of it.
 */
static void *seq_next(struct seq_file *m, void *v, loff_t *pos) {
    unsigned long *tmp_v = (unsigned long *)v;
    (*tmp_v)++;
    (*pos)++;
    return NULL;
}

static int seq_show(struct seq_file *m, void *v) {
    loff_t *spos = (loff_t *)v;
    seq_printf(m, "%Ld\n", *spos);
    return 0;
}

int init_module(void) {
    Proc_File = proc_create(PROCFS_NAME, 0644, NULL, &proc_fops);
    if (Proc_File == NULL) {
        return -ENOMEM;
    }

    sprintf(procfs_buffer, "Hello World!\n");
    procfs_buffer_size = strlen(procfs_buffer) + 1;

    printk(KERN_INFO "/proc/%s created\n", PROCFS_NAME);
    return 0;
}

void cleanup_module(void) {
    proc_remove(Proc_File);
    printk(KERN_INFO "/proc/%s removed\n", PROCFS_NAME);
}
