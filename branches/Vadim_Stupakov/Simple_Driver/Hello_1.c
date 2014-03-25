#include <linux/module.h>       	/* Needed by all modules */
#include <linux/kernel.h>       	/* Needed for KERN_INFO */
#include <linux/init.h>        		/* Needed for the macros */

static int hello_start(void)
{
    printk(KERN_INFO "Loading SIMPLE_DRIVER module...\n");
    printk(KERN_INFO "Hello world from the kernel!\n");
    return (0);
}
static void hello_end(void)
{
    printk(KERN_INFO "Goodbye word\n");
    printk(KERN_INFO "Unloading SIMPLE_DRIVER module...\n");
}

module_init(hello_start);
module_exit(hello_end);
