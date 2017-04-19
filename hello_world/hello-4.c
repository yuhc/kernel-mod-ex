#include <linux/module.h>
#include <linux/kernel.h> /* KERN_INFO */
#include <linux/init.h>   /* Macros */

#define DRIVER_AUTHOR "Hangchen Yu <hyu@cs.utexas.edu>"
#define DRIVER_DESC   "Practice driver"

static int hello4_data __initdata = 4;

static int __init hello_4_init(void) {
    printk(KERN_INFO "Hello world %d!\n", hello4_data);
    return 0;
}

static void __exit hello_4_exit(void) {
    printk(KERN_INFO "Goodbye world!\n");
}

module_init(hello_4_init);
module_exit(hello_4_exit);

/* Module documentation */
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);

/* /dev/testhello helps automatic configuration of modules in future */
MODULE_SUPPORTED_DEVICE("testhello");
