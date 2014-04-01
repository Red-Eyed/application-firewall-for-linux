#include "affl_main.h"


static int __init affl_mod_load(void)
{
    printk(KERN_INFO "affl mod load%d\n",RARI);
    affl_process_load();
    affl_chrdev_load();
    return AFFL_SUCCESS;
}

static void __exit affl_mod_unload(void)
{
    affl_chrdev_unload();
    affl_process_unload();
    printk(KERN_INFO "affl mod unload\n");
}


module_init(affl_mod_load);
module_exit(affl_mod_unload);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Orest");
MODULE_DESCRIPTION("affl_release");