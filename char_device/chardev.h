#ifndef __CHAR_DEV_H__
#define __CHAR_DEV_H__

#include <linux/fs.h>

static int __init init_char_dev(void);
static void __exit exit_char_dev(void);

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char __user *, size_t, loff_t *);

#define SUCCESS     0
#define DEVICE_NAME "char_device" /* appear in /proc/devices */
#define BUF_LEN     80

/* Global variables */
static int Major;
static int Device_Open = 0;

static char msg[BUF_LEN];
static char *msg_ptr;

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

#endif
