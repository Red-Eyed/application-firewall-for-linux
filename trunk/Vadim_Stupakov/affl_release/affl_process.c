
#ifndef AFFL_PROCESS_C_
#define AFFL_PROCESS_C_

#include "affl_process.h"

/////////////////////////////////////////
char* atribute = NULL;
char* begin = NULL;
char* end = NULL;
static char temp[100] = {'\0'};
/////////////////////////////////////////

unsigned int affl_handle(const char* input, char* user_buf)
{
    //input = "command@atribute#" or "command"

    //Declare variable////////////////////////

    //Command/////////////////////////////////
    const char affl_kill[] = "kill";
    const char affl_view[] = "view";
    const char affl_getInfo[] = "getInfo";
    const char affl_addProc[] = "addProc";
    const char affl_rmProc[] = "rmProc";
    unsigned int count = 0;
    char proc_name[100];
    struct file *f;
    printk("affl_Driver: affl_handle(): open\n");
    //Handle command view
    if (strstr(input, affl_view))
    {
    	struct task_struct *task;
    		for_each_process(task)
    		{
    			sprintf(temp, "/etc/proc/%d", task->pid);
    			f = filp_open(str"/etc/proc", O_RDONLY, 0);
    			if(f == NULL)
    			        printk(KERN_ALERT "filp_open error!!.\n");
    			    else{
    			        // Read the file
    			        f->f_op->read(f, proc_name, 100, &f->f_pos);
    			    }
    			//http://stackoverflow.com/questions/20753307/opening-a-file-from-userspace-from-a-linux-kernel-module
    			filp_close(f,NULL);
    			printk("=========================================================\n");
    			printk("%s [%d]\n", task->cmdline, task->pid);
    			sprintf(temp, "%d %s\n", task->pid, proc_name);
    			copy_to_user(user_buf+count, temp, strlen(temp));
    			printk("temp = %s", temp);
    			//printk("user_buf = %s", user_buf);
    			printk("=========================================================\n");
    			count += strlen(temp);
    		}
        printk("affl_Driver: affl_handle(): view\n");
        printk("affl_Driver: affl_handle(): count = %d\n", count);
        printk("affl_Driver: affl_handle(): buf = %p\n", (void*)user_buf);
        return count;
    }
    //Handle command kill
    else if (strstr(input, affl_kill))
    {
        printk("affl_Driver: affl_handle(): command =  kill\n");
        //Handle atribute
        affl_get_atribute(input);
        //Add your functio
    }
    //Handle command getInfo
    else if (strstr(input, affl_getInfo))
    {
        printk("affl_Driver: affl_handle():  command = getInfo \n");
        //Handle atribute
        affl_get_atribute(input);
        //Add your functio
    }
    //Handle command addProc
    else if (strstr(input, affl_addProc))
    {
        printk("affl_Driver: affl_handle():  command = addProc \n");
        //Handle atribute
        affl_get_atribute(input);
        //Add your function
    }
    //Handle command rmProc
    else if (strstr(input, affl_rmProc))
    {
        printk("affl_Driver: affl_handle():  command = rmProc \n");
        //Handle atribute
        affl_get_atribute(input);
        //Add your function
    }
    vfree(atribute);
    printk("affl_Driver: affl_handle(): close\n");
    return 0;
}

void affl_get_atribute(const char* input)
{
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

#endif /* AFFL_PROCESS_C_ */
