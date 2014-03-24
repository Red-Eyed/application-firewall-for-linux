
#ifndef AFFL_PROCESS_H_
#define AFFL_PROCESS_H_

#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/vmalloc.h>
#include <linux/export.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <asm-generic/uaccess.h>
#include <linux/kthread.h>
#include <linux/fs.h>
#include <linux/kallsyms.h>

void affl_init_process(void);
void affl_clean_process(void);

unsigned int affl_handle(const char* input,  char* user_buf);
void affl_get_proc_name(const char* input, char** atribute);
void affl_get_proc_PID(const char* input, int* PID);

unsigned int affl_view_process(char* user_buf);

void* affl_find_sym(const char *sym);

void affl_kill_process(const char* name, int PID);

int affl_from_name_to_pid(char* name);

int affl_get_task(void);

#endif /* AFFL_PROCESS_H_ */
