#include "affl_chrdev.h"

static dev_t MajMin;         
static struct cdev affl_chr_dev;     
static struct class *affl_class;
//static affl_size_buffer=500;
static int affl_len_buffer=0; 
static char affl_buffer[/*affl_size_buffer*/500];

static int major;
static int counterr = 0;

static ssize_t affl_comand_read( struct file* F, char *buf, size_t len, loff_t *f_pos )
{
  
  //sprintf( buffer, "%1d" , temp );  
  int i=0;

  printk(KERN_INFO "\nread\n");
    if(affl_len_buffer==0)
    return 0/*-EFAULT*/;
    

    printk("\n!!!!read %d !!!!\n",counterr);
  counterr++;    
  for(i=0;affl_len_buffer>i;i++,len--)
  {
    put_user(affl_buffer[i],buf+i);
  }
  i--;
  put_user('\n',buf+i);
  len--;
  i++;
  put_user('c',buf+i);
  len--;
  i++;
  
  affl_len_buffer=0;
  printk(KERN_INFO "i=%i",i);
  return i/*0len*/;
}

static ssize_t affl_comand_write( struct file* F, const char *buf, size_t len, loff_t *f_pos )
{  
    
  int i=0;
  printk(KERN_INFO "\nwrite\n");
  for(i=0;i<len && i<500/*affl_size_buffer*/;i++)
  {
    get_user(affl_buffer[i],buf+i);
  }
  affl_len_buffer=i;
  return i;
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
  printk( KERN_INFO "Major number is: %d\n",MAJOR(MajMin));
  
  return 0;
}

void affl_chrdev_unload(void)
{
  cdev_del( &affl_chr_dev );
  device_destroy( affl_class, MajMin );
  class_destroy( affl_class );
  unregister_chrdev_region( MajMin, 1 );
  
  printk(KERN_INFO "Device unregisteredlol\n");
}