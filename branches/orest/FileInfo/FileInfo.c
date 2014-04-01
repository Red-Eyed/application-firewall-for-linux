#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/moduleparam.h>	
#include <linux/unistd.h>	
#include <linux/sched.h>
#include <asm-generic/uaccess.h>
#include <linux/fs.h>
#include <linux/kallsyms.h>
#include <linux/kthread.h>
#include <linux/string.h>
#include <linux/fdtable.h>
#include <linux/rcupdate.h>
#include <linux/fs_struct.h>
#include <linux/dcache.h>
#include <linux/slab.h>

void view_proc_file_info(void)
{
  struct files_struct *current_files; 
  struct fdtable *files_table;
  int i=0,cnt=0;
  struct path files_path;
  char *cwd;
  char buf[300];
  
  struct task_struct* iter;
  for_each_process(iter){
    if(iter->pid==3018){
    printk("===========pid: %d======= ++++++++++++comm: %s++++++\n", iter->pid, iter->comm);
    current_files = iter->files;	
    files_table = files_fdtable(current_files);
    i=0;
    while(files_table->fd[i] != NULL) { 


      files_path = files_table->fd[i]->f_path;
      cwd = d_path(&files_path,buf,300*sizeof(char));
      printk(KERN_ALERT "file #%d:  %s\n", i,cwd);
      i++;
    }

    cnt++;}
  }
  printk("processess: %d \n", cnt);
}

static int __init FileInfo_load(void)
{
  printk(KERN_INFO "FileInfo mod load\n");
  view_proc_file_info();
  return 0;
}

static void __exit FileInfo_unload(void)
{
  printk(KERN_INFO "FileInfo mod unload\n");
}


module_init(FileInfo_load);
module_exit(FileInfo_unload);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Orest");
MODULE_DESCRIPTION("FileInfo");
