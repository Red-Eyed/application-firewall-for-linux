/*
 * affl_init_char_dev.h
 *
 *  Created on: 20 марта 2014
 *      Author: vadimorest
 */

#ifndef AFFL_CHAR_DEV_H_
#define AFFL_CHAR_DEV_H_

#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <asm-generic/uaccess.h>
#include <linux/kthread.h>
#include <linux/moduleparam.h>
#include "affl_process.h"

extern struct file_operations affl_fops;
extern char affl_kernel_buf[255];       // Global variable for the bufer in kernel space
extern int affl_size;                   // Global variable for the bufer size

int affl_open(struct inode *i, struct file *f);
int affl_close(struct inode *i, struct file *f);
ssize_t affl_read(struct file *f, char __user *buf, size_t len, loff_t *off);
ssize_t affl_write(struct file *f, const char __user *buf, size_t len, loff_t *off);

int affl_init_char_dev(const char* file_name, const char* device_name);
void affl_clean_char_dev(void);

void affl_check_file(char* filename);


extern dev_t first;                     // Global variable for the first device number
extern struct cdev c_dev;               // Global variable for the character device structure
extern struct class *cl;                // Global variable for the device class

extern int affl_flag;

#endif /* AFFL_CHAR_DEV_H_ */
