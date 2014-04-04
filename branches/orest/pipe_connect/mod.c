#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/unistd.h>
#include <asm-generic/uaccess.h>
#include <linux/kallsyms.h>

void* find_sym(const char *sym)
{
	static unsigned long faddr = 0;
	int symb_fn(void* data, const char* sym, struct module* mod,
			unsigned long addr)
	{
		if (0 == strcmp((char*) data, sym))
		{
			faddr = addr;
			return (1);
		}
		else
			return (0);
	}

	kallsyms_on_each_symbol(symb_fn, (void*) sym);
	return ((void*) faddr);
}

asmlinkage long (*affl_sys_write)(unsigned int fd, const char __user *buf,size_t count);
asmlinkage long (*affl_sys_close)(unsigned int fd);

static int mod_param0=5;
module_param(mod_param0, int, 0000);
MODULE_PARM_DESC(mod_param0, "pipe");

static int mod_param1=5;
module_param(mod_param1, int, 0000);
MODULE_PARM_DESC(mod_param1, "pipe");

static int __init mod_load(void)
{
    int i=0;
    char buf[100];
    int affl_pipe[2];
    affl_sys_write=find_sym("sys_write");
    affl_sys_close=find_sym("sys_close");
    affl_pipe[0]=mod_param0;
    affl_pipe[1]=mod_param1;
    printk(KERN_INFO "mod load %d %d\n",affl_pipe[0],affl_pipe[1]);
    //close(affl_pipe[0]);  unused read

    sprintf(buf, "Hi");
    printk("send %s\n", buf);
    for(i = 0; i < strlen(buf); i++)
      affl_sys_write(affl_pipe[1], buf + i, 1);
    affl_sys_close(affl_pipe[1]);
    affl_sys_close(affl_pipe[0]);
    return 0;
}

static void __exit mod_unload(void)
{

    printk(KERN_INFO "mod unload\n");
}

module_init(mod_load);
module_exit(mod_unload);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Orest");
MODULE_DESCRIPTION("pipe");
 
