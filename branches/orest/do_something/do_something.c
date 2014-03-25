#include "do_something.h"

static int __init do_something_load(void)
{
    ds_param_init();
    SUCCESS=ds_char_test_dev_init();
    SUCCESS=ds_proc_init();
    //SUCCESS=ds_process_init();
    printk(KERN_INFO "do_something load\n");
    return SUCCESS;
}

static void __exit do_something_unload(void)
{
    ds_char_test_dev_exit();
    ds_proc_exit();
    //ds_process_exit();
    printk(KERN_INFO "do_something unload\n");
}

module_init(do_something_load);
module_exit(do_something_unload);
