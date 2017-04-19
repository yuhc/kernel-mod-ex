#include <linux/module.h>
#include <linux/kernel.h> /* KERN_INFO */

int init_module(void) {
    printk(KERN_INFO "Hello world!\n");
    return 0;
}

void cleanup_module(void) {
    printk(KERN_INFO "Goodbye world!\n");
}
