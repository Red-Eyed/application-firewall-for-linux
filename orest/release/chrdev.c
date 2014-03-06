#include "chrdev.h"

#define GPIO_NUMBER    149     //User LED 0. GPIO number 149. Page 71 of BB-xM Sys Ref Manual.

static dev_t MajMin;         // Global variable for the MajMin device number
static struct cdev affl_chr_dev;     // Global variable for the character device structure
static struct class *affl_class;     // Global variable for the device class

static int major;


static ssize_t affl_comand_read( struct file* F, char *buf, size_t count, loff_t *f_pos )
{
  char buffer[10];
  
  int temp = gpio_get_value(GPIO_NUMBER);
  
  sprintf( buffer, "%1d" , temp );
  
  count = sizeof( buffer );
  
  if( copy_to_user( buf, buffer, count ) )
  {
    return -EFAULT;
  }
  
  if( *f_pos == 0 )
  {
    *f_pos += 1;
    return 1;
  }
  else
  {
    return 0;
  }
  
}

static ssize_t affl_comand_write( struct file* F, const char *buf, size_t count, loff_t *f_pos )
{
  
  printk(KERN_INFO "Executing WRITE.\n");
  
  switch( buf[0] )
  {
    case '0':
      gpio_set_value(GPIO_NUMBER, 0);
      break;
      
    case '1':
      gpio_set_value(GPIO_NUMBER, 1);
      break;
      
    default:
      printk("Wrong option.\n");
      break;
  }
  return count;
}

static int affl_comand_open( struct inode *inode, struct file *file )
{
  return 0;
}

static int affl_comand_close( struct inode *inode, struct file *file )
{
  return 0;
}

static struct file_operations affl_fop =
{
  .owner        = THIS_MODULE,
  .open         = affl_comand_open,
  .read         = affl_comand_read,
  .write        = affl_comand_write,
  .release      = affl_comand_close,
};

int affl_chrdev_load(void)
{
  major = alloc_chrdev_region( &MajMin, 0, 1, "affl_chrdev" );
  
  if( 0 > major )
  {
    printk( KERN_INFO "Device Registration failed\n" );
    return -1;
  }
  else
  {
    printk( KERN_INFO "Major number is: %d\n",major );
    //return 0;
  }
  
  if ( (affl_class = class_create( THIS_MODULE, "chrdev" ) ) == NULL )
  {
    printk( KERN_INFO "Class creation failed\n" );
    unregister_chrdev_region( MajMin, 1 );
    return -1;
  }
  
  if( device_create( affl_class, NULL, MajMin, NULL, "affl_chrdev" ) == NULL )
  {
    printk( KERN_INFO "Device creation failed\n" );
    class_destroy(affl_class);
    unregister_chrdev_region( MajMin, 1 );
    return -1;
  }
  
  cdev_init( &affl_chr_dev, &affl_fop );
  
  if( cdev_add( &affl_chr_dev, MajMin, 1 ) == -1)
  {
    printk( KERN_INFO "Device addition failed\n" );
    device_destroy( affl_class, MajMin );
    class_destroy( affl_class );
    unregister_chrdev_region( MajMin, 1 );
    return -1;
  }
  
  return 0;
}

void affl_chrdev_unload(void)
{
  cdev_del( &affl_chr_dev );
  //device_destroy( affl_class, MajMin );
  //class_destroy( affl_class );
  unregister_chrdev_region( MajMin, 1 );
  
  printk(KERN_INFO "Device unregistered\n");
}