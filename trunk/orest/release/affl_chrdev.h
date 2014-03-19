#ifndef AFFL_CHRDEV_H
#define AFFL_CHRDEV_H

#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <asm-generic/uaccess.h>


int affl_chrdev_load(void);
void affl_chrdev_unload(void);



#endif