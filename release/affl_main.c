#include "affl_main.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vadim & Orest");
MODULE_DESCRIPTION("Application furewall dor linux");


int init_module(void) /* Constructor */
{
	printk(KERN_INFO "affl_Driver: registered\n");

	affl_init_process();
	//affl_check_file("affl_comm");
	affl_init_char_dev("affl_comm", "affl_device");
	return (0);
}
void cleanup_module(void) /* Destructor */
{
	affl_clean_process();
	affl_clean_char_dev();
	printk(KERN_INFO "affl_Driver: unregistered\n");
}
