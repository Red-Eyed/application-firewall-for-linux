
#ifndef AFFL_PROCESS_H_
#define AFFL_PROCESS_H_

#include <linux/cdev.h>
#include <asm-generic/uaccess.h>
#include <linux/kallsyms.h>
#include <linux/fdtable.h>
#include <linux/fs_struct.h>

int affl_init_process(void);
void affl_clean_process(void);

unsigned int affl_handle(const char* input,  char* user_buf);

void* find_sym(const char *sym);

int affl_get_proc_name(const char* input, char** proc_name);
int affl_get_proc_PID(const char* input, int* PID);
int affl_get_task(void);
int affl_get_quantity_tasks(void);
int affl_get_black_list(char* user_buf);
int affl_get_info_for_process(int pid, char* user_buf);

unsigned int affl_view_process(char* user_buf);
int affl_kill_process(const char* name, int PID);

int affl_from_name_to_pid(char* name);

int affl_exist(char* user_buf);
void affl_bl_print(void);
int affl_bl_add(char* arg);
int affl_bl_rm(char* arg);
int affl_bl_cmp(const char* arg);

void affl_add_list_process_mass(const char* proc_name, int PID);

// показать управляющий регистр CR0
#define show_cr0()                        \
{  register unsigned r_eax asm ( "eax" ); \
   asm( "pushl %eax" );                   \
   asm( "movl %cr0, %eax" );              \
   printk( "! CR0 = %x\n", r_eax );       \
   asm( "popl %eax");                     \
}
//asm( "movl %cr0, %eax" );              \	//грузим управляющий регистр cr0 в регистр eax

//код выключения защиты записи:
#define rw_enable()              \
asm( "pushl %eax \n"             \
     "movl %cr0, %eax \n"        \
     "andl $0xfffeffff, %eax \n" \
     "movl %eax, %cr0 \n"        \
     "popl %eax" );

//     "movl %cr0, %eax \n"        \	//грузим управляющий регистр cr0 в регистр eax
//     "andl $0xfffeffff, %eax \n" \	//сбрасываем бит WP, запрещающий запись
//     "movl %eax, %cr0 \n"        \	//обновляем управляющий регистр cr0

//код включения защиты записи:
#define rw_disable()             \
asm( "pushl %eax \n"             \
     "movl %cr0, %eax \n"        \
     "orl $0x00010000, %eax \n"  \
     "movl %eax, %cr0 \n"        \
     "popl %eax" );

//     "movl %cr0, %eax \n"        \	//грузим управляющий регистр cr0 в регистр eax
//     "orl $0x00010000, %eax \n"  \	//устанавливаем бит WP, запрещающий запись
//     "movl %eax, %cr0 \n"        \	//сбрасываем бит WP, запрещающий запись

#endif /* AFFL_PROCESS_H_ */
