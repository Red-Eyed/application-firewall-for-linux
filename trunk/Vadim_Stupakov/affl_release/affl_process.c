#ifndef AFFL_PROCESS_C_
#define AFFL_PROCESS_C_

#include "affl_process.h"

unsigned int affl_handle(const char* input, char* user_buf)
{
    //Command/////////////////////////////////
    const char affl_kill[] = "kill";
    const char affl_view[] = "view";
    const char affl_getInfo[] = "getInfo";
    const char affl_addProc[] = "addProc";
    const char affl_rmProc[] = "rmProc";

    char* atribute = NULL;

    printk("affl_Driver: affl_handle(): open\n");

    //Handle command view
    if (strstr(input, affl_view))
    {
        unsigned long count = 0;
        count = affl_view_process(user_buf);
        printk("affl_Driver: affl_handle(): view\n");
        return (count);
    }
    //Handle command kill
    else if (strstr(input, affl_kill))
    {
        printk("affl_Driver: affl_handle(): command =  kill\n");
        //Handle atribute
        affl_get_atribute(input, atribute);
        //Add your functio
    }
    //Handle command getInfo
    else if (strstr(input, affl_getInfo))
    {
        printk("affl_Driver: affl_handle():  command = getInfo \n");
        //Handle atribute
        affl_get_atribute(input, atribute);
        //Add your functio
    }
    //Handle command addProc
    else if (strstr(input, affl_addProc))
    {
        printk("affl_Driver: affl_handle():  command = addProc \n");
        //Handle atribute
        affl_get_atribute(input, atribute);
        //Add your function
    }
    //Handle command rmProc
    else if (strstr(input, affl_rmProc))
    {
        printk("affl_Driver: affl_handle():  command = rmProc \n");
        //Handle atribute
        affl_get_atribute(input, atribute);
        //Add your function
    }
    vfree(atribute);
    printk("affl_Driver: affl_handle(): close\n");
    return (0);
}

void affl_get_atribute(const char* input, char* atribute)
{
    char* begin = NULL;
    char* end = NULL;
    if (strstr(input, "@"))
    {
        begin = strstr(input, "@");
        end = strstr(input, "#");
        begin++;
        atribute = vmalloc(end - begin);
        memcpy(atribute, begin, (size_t) (end - begin));
        printk("affl_handle(): atribute = %s \n", atribute);
    }
}

unsigned int affl_view_process(char* user_buf)
{
    char temp[100] =
        { '\0' };
    unsigned int count = 0;
//        char proc_name[100];
//        struct file *f = NULL;
    struct task_struct *task = NULL;
//        mm_segment_t fs;

    printk("affl_Driver: affl_handle(): command =  view\n");
    for_each_process(task)
    {
        sprintf(temp, "/proc/%d/cmdline", task->pid);
        //printk("affl_Driver: affl_handle(): temp = %s\n", temp);
    //            f = filp_open(temp, O_RDONLY, 0);
    //            if(f == NULL)
    //            printk(KERN_ALERT "filp_open error!!.\n");
    //            else
    //            {
    //                // Get current segment descriptor
    //                fs = get_fs();
    //                // Set segment descriptor associated to kernel space
    //                set_fs(get_ds());
    //                // Read the file
    //                f->f_op->read(f, proc_name, 100, &f->f_pos);
    //                // Restore segment descriptor
    //                set_fs(fs);
    //                //printk("affl_Driver: affl_handle(): proc_name = %s\n", proc_name);
    //            }
    //            //http://stackoverflow.com/questions/20753307/opening-a-file-from-userspace-from-a-linux-kernel-module
    //            filp_close(f,NULL);
        //printk("=========================================================\n");
        printk("%s [%d]\n", task->comm, task->pid);
        sprintf(temp, "%d %s\n", task->pid, task->comm);
        copy_to_user(user_buf+count, temp, strlen(temp));
        //printk("temp = %s", temp);
        //printk("user_buf = %s", user_buf);
        //printk("=========================================================\n");
        count += strlen(temp);
        //memset(proc_name,' ',100);
    }
    return (count);
}

#endif /* AFFL_PROCESS_C_ */
