int len_proc,temp_proc;
char *msg_proc;

int read_proc(struct file *filp,char *buf,size_t count,loff_t *offp );

struct file_operations proc_fops = {
    .read= read_proc
};

int ds_proc_init(void)
{
    proc_create("hello_proc",0,NULL,&proc_fops);
    msg_proc=" Hello World_proc ";
    len_proc=strlen(msg_proc);
    temp_proc=len_proc;
    printk(KERN_INFO "1.len=%d",len_proc);

    return SUCCESS;
}

void ds_proc_exit(void)
{
    remove_proc_entry("hello_proc",NULL);

}
int read_proc(struct file *filp,char *buf,size_t count,loff_t *offp ) 
{

    if(count>temp_proc)
    {
        count=temp_proc;
    }
    temp_proc=temp_proc-count;
    copy_to_user(buf,msg_proc, count);
    if(count==0)
        temp_proc=len_proc;

    return count;
}
