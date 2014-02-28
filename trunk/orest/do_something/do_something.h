static int SUCCESS=0;
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include<linux/init.h>
#include <linux/stat.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <asm-generic/uaccess.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include "ds_param.h"
#include "info.h"
#include "char_test_dev.h"
#include "ds_proc.h"