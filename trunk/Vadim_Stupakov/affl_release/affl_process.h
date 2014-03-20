
#ifndef AFFL_PROCESS_H_
#define AFFL_PROCESS_H_

#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <linux/export.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <asm-generic/uaccess.h>
#include <linux/kthread.h>




unsigned int affl_handle(const char* input,  char* user_buf);
void affl_get_atribute(const char* input);

#endif /* AFFL_PROCESS_H_ */
