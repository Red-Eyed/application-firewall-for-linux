#include "load.h"


static int __init affl_mod_load(void)
{
    affl_chrdev_load();
    printk(KERN_INFO "affl mod load\n");
    return AFFL_SUCCESS;
}

static void __exit affl_mod_unload(void)
{
    affl_chrdev_unload();
    printk(KERN_INFO "affl mod unload\n");
}


module_init(affl_mod_load);
module_exit(affl_mod_unload);