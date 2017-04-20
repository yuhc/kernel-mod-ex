#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/uaccess.h>    /* Put_user */

#include "chardev.h"

/*
 * Called when a process tries to open the device file.
 * e.g. cat /dev/char_device
 */
static int device_open(struct inode *inode, struct file *file) {
    static int counter = 0;
    if (Device_Open)
        return -EBUSY;
    Device_Open++;

    sprintf(msg, "I already told u %d times Hello world!\n", counter++);
    msg_ptr = msg;
    try_module_get(THIS_MODULE);
    printk(KERN_INFO "[chardev] Device opened\n");

    return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file) {
    Device_Open--;
    msg_ptr = msg;
    module_put(THIS_MODULE);
    printk(KERN_INFO "[chardev] Device released\n");

    return SUCCESS;
}

static ssize_t device_read(struct file *filp, char __user *buffer,
        size_t length, loff_t *offset) {
    int bytes_read = 0; // number of bytes actually written to buffer
    if (*msg_ptr == 0)
        return 0;

    while (length && *msg_ptr) {
        /*
         * Buffer is in user data segment, not kernel segment, so
         * "*" assignment won't work.
         */
        put_user(*(msg_ptr++), buffer++);
        length--;
        bytes_read++;
    }

    return bytes_read;
}

/*
 * Called when a process writes to dev file
 * e.g. echo "xxx" > /dev/char_device
 */
static ssize_t device_write(struct file *filp, const char __user *buffer,
        size_t length, loff_t *offset) {
    printk(KERN_ALERT "Unsupported write operation.\n");
    return -EINVAL;
}

static int __init init_char_dev(void) {
    Major = register_chrdev(0, DEVICE_NAME, &fops);
    if (Major < 0) {
        printk(KERN_ALERT "Registering char device failed: %d\n", Major);
        return Major;
    }

    /*
     * mknod char_device c <Major> <Minor>
     * cat /dev/char_device
     * echo "xxx" > /dev/char_device
     * rm /dev/char_device
     */
    printk(KERN_INFO "[chardev] Assigned major number %d\n", Major);
    printk(KERN_INFO "[chardev] To talk to the drive, create dev file by\n");
    printk(KERN_INFO "[chardev]   'mknod /dev/%s c %d 0'\n", DEVICE_NAME, Major);
    printk(KERN_INFO "[chardev] Remove dev file and module when done\n");

    return SUCCESS;
}

static void __exit exit_char_dev(void) {
    unregister_chrdev(Major, DEVICE_NAME);
}

module_init(init_char_dev);
module_exit(exit_char_dev);
