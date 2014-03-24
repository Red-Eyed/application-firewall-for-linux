#ifndef AFFL_PROCESS_H
#define AFFL_PROCESS_H

#include <linux/moduleparam.h>	
#include <linux/unistd.h>	
#include <linux/sched.h>
#include <asm-generic/uaccess.h>
#include <linux/fs.h>
#include <linux/kallsyms.h>
#include <linux/kthread.h>
#include <linux/string.h>

// показать управляющий регистр CR0
#define show_cr0()                        \
{  register unsigned r_eax asm ( "eax" ); \
   asm( "pushl %eax" );                   \
   asm( "movl %cr0, %eax" );              \
   printk( "! CR0 = %x\n", r_eax );       \
   asm( "popl %eax");                     \
}

//код выключения защиты записи:
#define rw_enable()              \
asm( "pushl %eax \n"             \
     "movl %cr0, %eax \n"        \
     "andl $0xfffeffff, %eax \n" \
     "movl %eax, %cr0 \n"        \
     "popl %eax" );

//код включения защиты записи:
#define rw_disable()             \
asm( "pushl %eax \n"             \
     "movl %cr0, %eax \n"        \
     "orl $0x00010000, %eax \n"  \
     "movl %eax, %cr0 \n"        \
     "popl %eax" );


     




int affl_process_load(void);
void affl_process_unload(void);

extern void affl_create_file(int major);


#endif