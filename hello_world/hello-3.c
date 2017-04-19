#include <linux/module.h>
#include <linux/kernel.h> /* KERN_INFO */
#include <linux/init.h>   /* Macros */

static int hello3_data __initdata = 3;

static int __init hello_3_init(void) {
    printk(KERN_INFO "Hello world %d!\n", hello3_data);
    return 0;
}

static void __exit hello_3_exit(void) {
    printk(KERN_INFO "Goodbye world!\n");
    // Cause a section mismatch for build; segfault for exit
    // printk(KERN_INFO "Goodbye world %d!\n", hello3_data);
}

module_init(hello_3_init);
module_exit(hello_3_exit);
