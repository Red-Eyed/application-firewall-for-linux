#include "affl_char_dev.h"

int affl_flag = 1;

struct cdev affl_c_dev;
dev_t affl_first;
struct class *affl_cl;
int affl_Device_Open = 0;

struct file_operations affl_fops =
{ .owner = THIS_MODULE, .open = affl_open, .release = affl_close, .read =
		affl_read, .write = affl_write };

char affl_kernel_buf[255];
int affl_size = 0;

int affl_open(struct inode *i, struct file *f)
{
	if (affl_Device_Open)
		return (-EBUSY);
	affl_Device_Open++;
	try_module_get(THIS_MODULE );
	return (0);
}
int affl_close(struct inode *i, struct file *f)
{
	affl_Device_Open--;
	module_put(THIS_MODULE );
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

		printk(KERN_INFO "affl_Driver: affl_read()\n");
		if (copy_to_user(buf, affl_kernel_buf, affl_size) != 0)
		{
			printk(KERN_INFO "affl_read(): Error\n");
			return (0);
		}
		//Hanle command
		size = affl_handle(affl_kernel_buf, buf);
		len -= size;

		affl_flag = 0;
		return (size);
	}

	return (0);
}
ssize_t affl_write(struct file *f, const char __user *buf, size_t len,
		loff_t *off)
{
	affl_flag = 1;
	affl_size = len;
	printk(KERN_INFO "affl_Driver: affl_write()\n");
	if (copy_from_user(affl_kernel_buf, buf, affl_size) != 0)
	{
		printk(KERN_INFO "affl_write(): Error\n");
		return (0);
	}
	printk(KERN_INFO "affl_Driver: affl_write(): len = %i\n", (int) len);
	return (affl_size);

	return (0);
}
asmlinkage long (*affl_sys_unlink)(const char __user *pathname);

void affl_check_file(const char* filename)
{
	printk("affl_check_file(): begin\n");
	if((affl_sys_unlink = find_sym("sys_unlink")))
	{
		struct file *f = NULL;
		char path[100];
		char path_name[100];
		char temp_filename[100];
		mm_segment_t fs;
		memset(path, 0, 100);
		memset(path_name, 0, 100);
		memset(temp_filename, 0, 100);
		strcpy(temp_filename, filename);
		strcpy(path, "/dev/");
		sprintf(path_name, "%s%s", path, temp_filename);
		printk("\taffl_sys_unlink = %p\n", affl_sys_unlink);
		f = filp_open(path_name, O_RDONLY, 0);
		if (f == NULL )
		{
			printk("\filp_open error!!.\n");
		}
		else
		{
			printk("\taffl_check_file(): file open success\n");
			filp_close(f, NULL );
			fs = get_fs();
			set_fs(get_ds());
			if(!affl_sys_unlink(path_name))
			{
				printk("\taffl_check_file(): delete\n");
			}
			set_fs(fs);

		}
	}
	else
	{
		printk("\tcannot find symbol\n");
	}
	printk("affl_check_file(): end\n");
}

int affl_init_char_dev(const char* file_name, const char* device_name)
{
	printk("affl_init_char_dev(): begin\n");
	affl_check_file(file_name);
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
	printk("affl_init_char_dev(): end\n");
	return (0);
}

void affl_clean_char_dev(void)
{
	printk("affl_clean_char_dev(): begin\n");
	cdev_del(&affl_c_dev);
	device_destroy(affl_cl, affl_first);
	class_destroy(affl_cl);
	unregister_chrdev_region(affl_first, 1);
	printk("affl_clean_char_dev(): end\n");
}
