#include <linux/module.h>
#include <linux/kernel.h> /* KERN_INFO */
#include <linux/init.h>   /* Macros */

static int __init hello_2_init(void) {
    printk(KERN_INFO "Hello world!\n");
    return 0;
}

static void __exit hello_2_exit(void) {
    printk(KERN_INFO "Goodbye world!\n");
}

module_init(hello_2_init);
module_exit(hello_2_exit);
