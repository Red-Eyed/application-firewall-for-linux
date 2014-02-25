#include <linux/module.h>	
#include <linux/kernel.h>	
#include <linux/init.h>		

static int __init simple_driver_load(void)
{
	printk(KERN_INFO "Simple driver load\n");
	return 0;
}

static void __exit simple_driver_unload(void)
{
	printk(KERN_INFO "Simple driver unload\n");
}

module_init(simple_driver_load);
module_exit(simple_driver_unload);