/*
 * affl_init_char_dev.h
 *
 *  Created on: 20 марта 2014
 *      Author: vadimorest
 */

#ifndef AFFL_CHAR_DEV_H_
#define AFFL_CHAR_DEV_H_

#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <asm-generic/uaccess.h>
#include "affl_process.h"

int affl_open(struct inode *i, struct file *f);
int affl_close(struct inode *i, struct file *f);
ssize_t affl_read(struct file *f, char __user *buf, size_t len, loff_t *off);
ssize_t affl_write(struct file *f, const char __user *buf, size_t len, loff_t *off);

int affl_init_char_dev(const char* file_name, const char* device_name);
void affl_clean_char_dev(void);

#endif /* AFFL_CHAR_DEV_H_ */
