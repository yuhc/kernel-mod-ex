#include <linux/module.h>
#include <linux/moduleparam.h> /* Module argument */
#include <linux/kernel.h>     /* KERN_INFO */
#include <linux/init.h>       /* Macros */
#include <linux/stat.h>       /* Permission bits */

#define DRIVER_AUTHOR "Hangchen Yu <hyu@cs.utexas.edu>"
#define DRIVER_DESC   "Practice driver"

static int hello4_data __initdata = 5;

static short int myshort = 1;
static int myint = 2;
static long int mylong = 3;
static char *mystr = "hyu";
static int myarray[2] = {-1, 1};
static int arr_argc = 0;

/*
 * module_param(foo, int, 0000)
 * parameter name, data type, permission bits
 */
module_param(myshort, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(myshort, "A short integer");
module_param(myint, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(myint, "An integer");
module_param(mylong, long, S_IRUSR);
MODULE_PARM_DESC(mylong, "A long integer");
module_param(mystr, charp, 0000);
MODULE_PARM_DESC(mystr, "A character string (pointer)");

/*
 * module_param_array(foo, int, 10, 0000)
 * name, type, &number, permission
 * &number: a pointer to the variable that stores the number of elements
 * of the array initialized by the user at module loading time.
 */
module_param_array(myarray, int, &arr_argc, 0000);

static int __init hello_5_init(void) {
    int i;
    printk(KERN_INFO "Hello world %d!\n", hello4_data);
    printk(KERN_INFO "myshort=%hd\n", myshort);
    printk(KERN_INFO "myint=%d\n", myint);
    printk(KERN_INFO "mylong=%ld\n", mylong);
    printk(KERN_INFO "mystr=%s\n", mystr);
    for (i = 0; i < (sizeof(myarray)/sizeof(int)); ++i)
        printk(KERN_INFO "myarray[%d]=%d\n", i, myarray[i]);
    printk(KERN_INFO "got %d arguments for myarray\n", arr_argc);

    return 0;
}

static void __exit hello_5_exit(void) {
    printk(KERN_INFO "Goodbye world!\n");
}

module_init(hello_5_init);
module_exit(hello_5_exit);

/* Module documentation */
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);

/* /dev/testhello helps automatic configuration of modules in future */
MODULE_SUPPORTED_DEVICE("testhello");
