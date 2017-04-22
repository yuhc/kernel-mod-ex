#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#include "chardev.h"

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char __user *, size_t, loff_t *);
static long device_ioctl(struct file *, unsigned int, unsigned long);

static int Device_Open = 0;
static char Message[BUFFER_LEN];
static char *Message_Ptr;

struct file_operations fops = {
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write,
    .unlocked_ioctl = device_ioctl
};

static int device_open(struct inode *inode, struct file *filp) {
    printk(KERN_INFO "Device opened\n");

    try_module_get(THIS_MODULE);
    if (Device_Open)
        return -EBUSY;
    Device_Open++;

    Message_Ptr = Message;
    return SUCCESS;
}

static int device_release(struct inode *inode, struct file *filp) {
    Device_Open--;
    module_put(THIS_MODULE);
    printk(KERN_INFO "Device released\n");
    return SUCCESS;
}

static ssize_t device_read(struct file *filp, char __user *buf,
        size_t len, loff_t *offset) {
    int bytes_read = 0;

    if (*Message_Ptr == 0)
        return 0;

    while (len && *Message_Ptr) {
        put_user(*(Message_Ptr++), buf++);
        len--;
        bytes_read++;
    }
    printk(KERN_INFO "Read %d bytes, %ld space left\n", bytes_read, len);

    return bytes_read;
}

static ssize_t device_write(struct file *filp, const char __user *buf,
        size_t len, loff_t *offset) {
    int i;
    for (i = 0; i < len && i < BUFFER_LEN; ++i)
        get_user(Message[i], buf + i);
    Message_Ptr = Message;

    return i;
}

/*
 * Function is called whenever a process tries to do an IOCTL on the
 * device file. If the IOCTL is r/w, the IOCTL call returns the output
 * of this function.
 */
static long device_ioctl(struct file *filp, unsigned int ioctl_num,
        unsigned long ioctl_param) {
    int i;
    char *tmp;
    char ch;

    /* Switch according to the IOCTL called */
    switch(ioctl_num) {
        case IOCTL_SET_MSG:
            /* find the length of message */
            tmp = (char *)ioctl_param;
            get_user(ch, tmp);
            for (i = 0; i < BUFFER_LEN && ch; ++i, ++tmp)
                get_user(ch, tmp);
            device_write(filp, (char *)ioctl_param, i, 0);
            break;

        case IOCTL_GET_MSG:
            i = device_read(filp, (char *)ioctl_param, 99, 0);
            put_user('\0', (char *)(ioctl_param + i));
            break;

        case IOCTL_GET_NTH_BYTE:
            return Message[ioctl_param];
            break;
    }

    return SUCCESS;
}

int init_module(void) {
    int ret;
    ret = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops);
    if (ret < 0) {
        printk(KERN_ALERT "Registering char device failed: %d\n", ret);
        return ret;
    }

    printk(KERN_INFO "Device major number: %d\n", MAJOR_NUM);
    printk(KERN_INFO "Create device file by 'mknod %s c %d 0'\n",
            DEVICE_FILE_NAME, MAJOR_NUM);

    return 0;
}

void cleanup_module(void) {
    unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
}
