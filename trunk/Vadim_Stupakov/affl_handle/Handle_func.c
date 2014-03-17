#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <asm-generic/uaccess.h>
#include <linux/string.h>
#include <linux/vmalloc.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vadim ");
MODULE_DESCRIPTION("Handler module");

//Declarations

static int affl_size = 0;				// Global variable for the bufer size
static char affl_kernel_buf[255];		// Global variable for the bufer in kernel space
static dev_t first; 					// Global variable for the first device number
static struct cdev c_dev;  				// Global variable for the character device structure
static struct class *cl; 				// Global variable for the device class

static void affl_handle(const char* input);
static int affl_open(struct inode *i, struct file *f);
static int affl_close(struct inode *i, struct file *f);
static ssize_t affl_read(struct file *f, char __user *buf, size_t len, loff_t *off);
static ssize_t affl_write(struct file *f, const char __user *buf, size_t len, loff_t *off);

//Declarations END

static int affl_open(struct inode *i, struct file *f)
{
	printk(KERN_INFO "affl_Driver: affl_open()\n");
	return (0);
}
static int affl_close(struct inode *i, struct file *f)
{
	printk(KERN_INFO "affl_Driver: affl_close()\n");
	return (0);
}
static ssize_t affl_read(struct file *f, char __user *buf, size_t len,
		loff_t *off)
{
	printk(KERN_INFO "affl_Driver: affl_read()\n");
	if (copy_to_user(buf, affl_kernel_buf, affl_size) != 0)
	{
		printk(KERN_INFO "affl_read(): Error\n");
		return (-EFAULT);
	}

	//Hanle command
	affl_handle(affl_kernel_buf);

	printk(KERN_INFO "affl_Driver: affl_read(): affl_size = %i\n", affl_size);

	return (0);
}
static ssize_t affl_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
	affl_size = len;
	printk(KERN_INFO "affl_Driver: affl_write()\n");
	if (copy_from_user(affl_kernel_buf, buf, affl_size) != 0)
	{
		printk(KERN_INFO "affl_write(): Error\n");
		return (-EFAULT);
	}
	printk(KERN_INFO "affl_Driver: affl_write(): len = %i\n", (int) len);
	return (affl_size);
}
static struct file_operations affl_fops =
{
	.owner = THIS_MODULE,
	.open = affl_open,
	.release = affl_close,
	.read =	affl_read,
	.write = affl_write
};

int init_module(void) /* Constructor */
{
	printk(KERN_INFO "affl_Driver: registered\n");
	if (alloc_chrdev_region(&first, 0, 1, "affl") < 0)
	{
		return (-1);
	}
	if ((cl = class_create(THIS_MODULE, "affl_chardev")) == NULL )
	{
		unregister_chrdev_region(first, 1);
		return (-1);
	}
	if (device_create(cl, NULL, first, NULL, "affl_comm") == NULL )
	{
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		return (-1);
	}
	cdev_init(&c_dev, &affl_fops);
	if (cdev_add(&c_dev, first, 1) == -1)
	{
		device_destroy(cl, first);
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		return (-1);
	}
	return (0);
}
void cleanup_module(void) /* Destructor */
{
	cdev_del(&c_dev);
	device_destroy(cl, first);
	class_destroy(cl);
	unregister_chrdev_region(first, 1);
	printk(KERN_INFO "affl_Driver: unregistered\n");
}

static void affl_handle(const char* input)
{
	//input = "command@atribute#" or "command"

	//Declare variable////////////////////////

	//Command/////////////////////////////////
	const char affl_kill[] = "kill";
	const char affl_view[] = "view";
	const char affl_getInfo[] = "getInfo";
	const char affl_addProc[] = "addProc";
	const char affl_rmProc[] = "rmProc";
	/////////////////////////////////////////
	char* atribute;
	char* begin;
	char* end;
	/////////////////////////////////////////

	printk("affl_Driver: affl_handle(): open\n");
	//Handle command view
	if (strstr(input, affl_view))
	{
		printk("affl_Driver: affl_handle(): view\n");
	}
	//Handle command kill
	else if (strstr(input, affl_kill))
	{
		printk("affl_Driver: affl_handle(): command =  kill\n");
		//Handle atribute
		if (strstr(input, "@"))
		{
			begin = strstr(input, "@");
			end = strstr(input, "#");
			begin++;
			atribute = vmalloc(end - begin);
			memcpy(atribute, begin, (size_t) (end - begin));
			printk("affl_Driver: affl_handle(): atribute = %s \n", atribute);
			vfree(atribute);
			//Add your function
		}
	}
	//Handle command getInfo
	else if (strstr(input, affl_getInfo))
	{
		printk("affl_Driver: affl_handle():  command = getInfo \n");
		//Handle atribute
		if (strstr(input, "@"))
		{
			begin = strstr(input, "@");
			end = strstr(input, "#");
			begin++;
			atribute = vmalloc(end - begin);
			memcpy(atribute, begin, (size_t) (end - begin));
			printk("affl_handle(): atribute = %s \n", atribute);
			vfree(atribute);
			//Add your function
		}
	}
	//Handle command addProc
	else if (strstr(input, affl_addProc))
	{
		printk("affl_Driver: affl_handle():  command = addProc \n");
		//Handle atribute
		if (strstr(input, "@"))
		{
			begin = strstr(input, "@");
			end = strstr(input, "#");
			begin++;
			atribute = vmalloc(end - begin);
			memcpy(atribute, begin, (size_t) (end - begin));
			printk("affl_handle(): atribute = %s \n", atribute);
			vfree(atribute);
			//Add your function
		}
	}
	//Handle command rmProc
	else if (strstr(input, affl_rmProc))
	{
		printk("affl_Driver: affl_handle():  command = rmProc \n");
		//Handle atribute
		if (strstr(input, "@"))
		{
			begin = strstr(input, "@");
			end = strstr(input, "#");
			begin++;
			atribute = vmalloc(end - begin);
			memcpy(atribute, begin, (size_t) (end - begin));
			printk("affl_handle(): atribute = %s \n", atribute);
			vfree(atribute);
			//Add your function
		}
	}
	printk("affl_Driver: affl_handle(): close\n");
}
