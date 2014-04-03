#include "affl_char_dev.h"

int affl_flag = 0;


struct cdev affl_c_dev;
dev_t affl_first;
struct class *affl_cl;

struct file_operations affl_fops =
{
    .owner = THIS_MODULE,
    .open = affl_open,
    .release = affl_close,
    .read =	affl_read,
    .write = affl_write
};

char affl_kernel_buf[255];
int affl_size = 0;

int affl_open(struct inode *i, struct file *f)
{
	//printk(KERN_INFO "affl_Driver: affl_open()\n");
	return (0);
}
int affl_close(struct inode *i, struct file *f)
{
	//printk(KERN_INFO "affl_Driver: affl_close()\n");
	return (0);
}
ssize_t affl_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
	unsigned int size = 0;
	if (affl_flag == 0)
	{
		return (0);
	}
	else
	{
		//printk(KERN_INFO "affl_Driver: affl_read()\n");
		if (copy_to_user(buf, affl_kernel_buf, affl_size) != 0)
		{
			//printk(KERN_INFO "affl_read(): Error\n");
			return (0);
		}

		//Hanle command
		size = affl_handle(affl_kernel_buf, buf);
		len -= size;
		//printk(KERN_INFO "affl_Driver: affl_read(): buf = %p\n", (void*)buf);
		//printk(KERN_INFO "affl_Driver: affl_read(): affl_size = %i\n", affl_size);

		affl_flag = 0;
		return (size);
	}
}
ssize_t affl_write(struct file *f, const char __user *buf, size_t len,
		loff_t *off)
{
	affl_flag = 1;
	affl_size = len;
	//printk(KERN_INFO "affl_Driver: affl_write()\n");
	if (copy_from_user(affl_kernel_buf, buf, affl_size) != 0)
	{
		printk(KERN_INFO "affl_write(): Error\n");
		return (0);
	}

	//printk(KERN_INFO "affl_Driver: affl_write(): len = %i\n", (int) len);
	return (affl_size);
}
asmlinkage long (*affl_sys_removexattr)(const char __user *path,
				const char __user *name);

void affl_check_file(char* filename)
{
	struct file *f = NULL;
	char* path = "/dev/";
	char* path_name = NULL;
	sprintf(path_name, "%s%s", path, filename);
	///printk("affl_check_file(): path_name = %s", path_name);
	f = filp_open(path_name, O_RDONLY, 0);
	if (f == NULL )
	{
		printk(KERN_ALERT "filp_open error!!.\n");
	}
	else
	{
		printk("affl_check_file(): delete\n");
		filp_close(f, NULL );
		affl_sys_removexattr(path, filename);
	}
}

int affl_init_char_dev(const char* file_name, const char* device_name)
{
	if (alloc_chrdev_region(&affl_first, 0, 1, file_name) < 0)
	{
		return (-1);
	}
	if ((affl_cl = class_create(THIS_MODULE, device_name)) == NULL )
	{
		unregister_chrdev_region(affl_first, 1);
		return (-1);
	}
	if (device_create(affl_cl, NULL, affl_first, NULL, file_name) == NULL )
	{
		class_destroy(affl_cl);
		unregister_chrdev_region(affl_first, 1);
		return (-1);
	}
	cdev_init(&affl_c_dev, &affl_fops);
	if (cdev_add(&affl_c_dev, affl_first, 1) == -1)
	{
		device_destroy(affl_cl, affl_first);
		class_destroy(affl_cl);
		unregister_chrdev_region(affl_first, 1);
		return (-1);
	}
	return (0);
}

void affl_clean_char_dev(void)
{
	cdev_del(&affl_c_dev);
	device_destroy(affl_cl, affl_first);
	class_destroy(affl_cl);
	unregister_chrdev_region(affl_first, 1);
}
