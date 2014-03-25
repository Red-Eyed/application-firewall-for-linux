static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);


#define DEVICE_NAME "char_test_dev"	
#define BUF_LEN 80	

static int major;	
static int Device_Open = 0;
static char msg[BUF_LEN];	
static char *msg_ptr;

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};


int ds_char_test_dev_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0)
    {
        printk(KERN_INFO "Registering char_test_device failed with %d\n", major);
        return major;
    }
    printk(KERN_INFO "I was assigned major number %d. To talk to\n", major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'sudo mknod /dev/%s c %d 0'.\n", DEVICE_NAME, major);
    printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
    printk(KERN_INFO "the device file.\n");
    printk(KERN_INFO "Remove the device file and module whell done.\n");

    return SUCCESS;
}

void ds_char_test_dev_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
}

static int device_open(struct inode *inode, struct file *file)
{
    static int counter = 0;

    if (Device_Open)
        return -EBUSY;

    Device_Open++;
    sprintf(msg, "I already told you %d times Hello world!\n", counter++);
    msg_ptr = msg;
    try_module_get(THIS_MODULE);

    return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
    Device_Open--;
    module_put(THIS_MODULE);

    return 0;
}

static ssize_t device_read(struct file *filp,char *buffer,size_t length,loff_t * offset)
{
    int bytes_read = 0;
    if (*msg_ptr == 0)
        return 0;
    while (length && *msg_ptr) {
        put_user(*(msg_ptr++), buffer++);
        length--;
        bytes_read++;
    }

    return bytes_read;
}

static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
    printk(KERN_INFO "Sorry, this operation isn't supported.\n");
    return -EINVAL;
}
