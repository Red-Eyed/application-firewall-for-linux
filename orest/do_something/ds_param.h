static int ds_mod_param=5;
module_param(ds_mod_param, int, 0000);
MODULE_PARM_DESC(ds_mod_param, "An integer");

void ds_param_init(void)
{
  printk(KERN_INFO "do_something load\n");
  printk(KERN_INFO "ds_mod_param=%d\n",ds_mod_param);
}