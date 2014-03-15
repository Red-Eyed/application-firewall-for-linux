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

static int affl_size = 0;
static char c[20];
static dev_t first; // Global variable for the first device number
static struct cdev c_dev; // Global variable for the character device structure
static struct class *cl; // Global variable for the device class

//Declarations
static void
affl_handle(const char* input);
static int
affl_open(struct inode *i, struct file *f);
static int
affl_close(struct inode *i, struct file *f);
static ssize_t
affl_read(struct file *f, char __user *buf, size_t len, loff_t *off);
static ssize_t
affl_write(struct file *f, const char __user *buf, size_t len, loff_t *off);
//

static int affl_open(struct inode *i, struct file *f)
{
	printk(KERN_INFO "Driver: affl_open()\n");
	return (0);
}
static int affl_close(struct inode *i, struct file *f)
{
	printk(KERN_INFO "Driver: affl_close()\n");
	return (0);
}
static ssize_t affl_read(struct file *f, char __user *buf, size_t len,
		loff_t *off)
{
	static int a = 1;
	printk(KERN_INFO "Driver: affl_read()\n");
	if (copy_to_user(buf, c, affl_size) != 0)
	{
		printk(KERN_INFO "affl_read(): Error\n");
		return (-EFAULT);
	}
	printk(KERN_INFO "affl_read(): affl_size = %i\n", affl_size);
	printk(KERN_INFO "affl_read(): len = %i\n", (int) len);

	affl_handle(c);

	if (a) //Bad
	{
		a = 0;
		return (affl_size);
	}
	else
	{
		a = 1;
		affl_size = 0;
		return (affl_size);
	}

}
static ssize_t affl_write(struct file *f, const char __user *buf, size_t len,
		loff_t *off)
{
	affl_size = len;
	printk(KERN_INFO "Driver: affl_write()\n");
	if (copy_from_user(c, buf, affl_size) != 0)
	{
		printk(KERN_INFO "affl_write(): Error\n");
		return (-EFAULT);
	}
	printk(KERN_INFO "affl_write(): len = %i\n", (int) len);
	return (affl_size);
}
static struct file_operations affl_fops =
{ .owner = THIS_MODULE, .open = affl_open, .release = affl_close, .read =
		affl_read, .write = affl_write };

int init_module(void) /* Constructor */
{
	printk(KERN_INFO "Namaskar: ofcd registered\n");
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
	printk(KERN_INFO "Alvida: ofcd unregistered\n");
}

static void affl_handle(const char* input)
{
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
	int i = 0;
	/////////////////////////////////////////

	printk("affl_handle(): open\n");
	//Handle command view
	if (strstr(input, affl_view))
	{
		printk("affl_handle(): view\n");
	}
	//Handle command kill
	else if (strstr(input, affl_kill))
	{
		printk("affl_handle(): kill\n");
		//Handle atribute
		if (strstr(input, "@"))
		{
			begin = strstr(input, "@");
			end = strstr(input, "#");
			begin++;
			atribute = vmalloc(end - begin);
			for (begin; begin != end; begin++)
			{
				atribute[i] = *begin;
				i++;
			}
			printk("affl_handle(): atribute = %s \n", atribute);
			vfree(atribute);
			i = 0;
			//Add your function
		}
	}
	//Handle command getInfo
	else if (strstr(input, affl_getInfo))
	{
		printk("affl_handle(): getInfo \n");
		//Handle atribute
		if (strstr(input, "@"))
		{
			begin = strstr(input, "@");
			end = strstr(input, "#");
			begin++;
			atribute = vmalloc(end - begin);
			for (begin; begin != end; begin++)
			{
				atribute[i] = *begin;
				i++;
			}
			printk("affl_handle(): atribute = %s \n", atribute);
			vfree(atribute);
			i = 0;
			//Add your function
		}
	}
	//Handle command addProc
	else if (strstr(input, affl_addProc))
	{
		printk("affl_handle(): addProc \n");
		//Handle atribute
		if (strstr(input, "@"))
		{
			begin = strstr(input, "@");
			end = strstr(input, "#");
			begin++;
			atribute = vmalloc(end - begin);
			for (begin; begin != end; begin++)
			{
				atribute[i] = *begin;
				i++;
			}
			printk("affl_handle(): atribute = %s \n", atribute);
			vfree(atribute);
			i = 0;
			//Add your function
		}
	}
	//Handle command rmProc
	else if (strstr(input, affl_rmProc))
	{
		printk("affl_handle(): rmProc \n");
		//Handle atribute
		if (strstr(input, "@"))
		{
			begin = strstr(input, "@");
			end = strstr(input, "#");
			begin++;
			atribute = vmalloc(end - begin);
			for (begin; begin != end; begin++)
			{
				atribute[i] = *begin;
				i++;
			}
			printk("affl_handle(): atribute = %s \n", atribute);
			vfree(atribute);
			i = 0;
			//Add your function
		}
	}
	printk("affl_handle(): close\n");
}
