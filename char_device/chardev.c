#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/uaccess.h>    /* Put_user */

#include "chardev.h"

int __init init_char_dev(void) {
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

void __exit exit_char_dev(void) {
    int ret = unregister_chrdev(Major, DEVICE_NAME);
    if (ret < 0)
        printk(KERN_ALERT "Error in unregister_chrdev: %d\n", ret);
}

module_init(init_char_dev);
module_exit(exit_char_dev);
