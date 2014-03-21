#ifndef AFFL_PROCESS_C_
#define AFFL_PROCESS_C_

#include "affl_process.h"

struct affl_list_process
{
    char process_name[50];
    int PID;
};

struct affl_list_process affl_list_process_mas[300];
int affl_cnt_process_mas = 0;

//affl_list_process affl_list_process_bl[100];
//static affl_cnt_bl = 0;

unsigned int affl_handle(const char* input, char* user_buf)
{
    //Command/////////////////////////////////
    const char affl_kill[] = "kill";
    const char affl_view[] = "view";
    const char affl_getInfo[] = "getInfo";
    const char affl_addProc[] = "addProc";
    const char affl_rmProc[] = "rmProc";

    char* proc_name = NULL;

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
        affl_get_proc_name(input, &proc_name);
        printk("affl_Driver: affl_handle(): atribute =  %s\n", proc_name);
        affl_kill_process(proc_name);
    }
    //Handle command getInfo
    else if (strstr(input, affl_getInfo))
    {
        printk("affl_Driver: affl_handle():  command = getInfo \n");
        //Handle atribute
        affl_get_proc_name(input, &proc_name);
        //Add your functio
    }
    //Handle command addProc
    else if (strstr(input, affl_addProc))
    {
        printk("affl_Driver: affl_handle():  command = addProc \n");
        //Handle proc_name
        affl_get_proc_name(input, &proc_name);
        //Add your function
    }
    //Handle command rmProc
    else if (strstr(input, affl_rmProc))
    {
        printk("affl_Driver: affl_handle():  command = rmProc \n");
        //Handle proc_name
        affl_get_proc_name(input, &proc_name);
        //Add your function
    }

    vfree(proc_name);
    printk("affl_Driver: affl_handle(): close\n");
    return (0);
}
//get second param: command@atribute#
void affl_get_proc_name(const char* input, char** proc_name)
{
    char* begin = NULL;
    char* end = NULL;
    if (strstr(input, "@"))
    {
        begin = strstr(input, "@");
        end = strstr(input, "#");
        begin++;
        *proc_name = vmalloc(end - begin);
        memcpy(*proc_name, begin, (size_t) (end - begin));
        printk("affl_handle(): atribute = %s \n", *proc_name);
    }
}

unsigned int affl_view_process(char* user_buf)
{
    char temp[100];
    unsigned int count = 0;
    int i = 0;
    memset(temp, 0, 100);
    printk("affl_Driver: affl_handle(): command =  view\n");
    affl_get_task();
    for (i = 0; i < affl_cnt_process_mas; i++)
    {
        sprintf(temp, " [%d] %s\n", affl_list_process_mas[i].PID,
                affl_list_process_mas[i].process_name);
        copy_to_user(user_buf + count, temp, strlen(temp));
        count += strlen(temp);
        memset(temp, 0, 100);
    }
    printk("count = %d\n", count);
    return (count);
}

void* affl_find_sym(const char *sym)
{
    static unsigned long faddr = 0;
    int symb_fn(void* data, const char* sym, struct module* mod,
            unsigned long addr)
    {
        if (0 == strcmp((char*) data, sym))
        {
            faddr = addr;
            return 1;
        } else
            return 0;
    }

    kallsyms_on_each_symbol(symb_fn, (void*) sym);
    return (void*) faddr;
}

asmlinkage long (*affl_sys_kill)(int pid, int sig);

void affl_kill_process(char* name)
{
    int i = 0;
    affl_get_task();
    for(i = 0; i < affl_cnt_process_mas; i++)
    {
        if (strstr(affl_list_process_mas[i].process_name, name))
        {
            affl_sys_kill(affl_list_process_mas[i].PID, 9);
            break;
        }
        else
        {
            printk("affl_kill_process(): bad pid \n");
        }
    }
}

int affl_from_name_to_pid(char* name)
{
int pid = 0;
struct task_struct *task = NULL;
for_each_process(task)
{
    printk("affl_from_name_to_pid() task = %p\n", task);
    printk("affl_from_name_to_pid() name = %s\n", name);
    printk("affl_from_name_to_pid() task->comm = %s\n", task->comm);
    if(!strcmp(name, task->comm))
    {
        pid = task->pid;
        printk("affl_from_name_to_pid(): pid = %d", pid);
        break;
    }
    else
    {
        printk("affl_from_name_to_pid(): bred pid\n");
    }
}
return pid;
}

int affl_get_task(void)
{
    char path[100];
    char proc_name[100];
    struct file *f = NULL;
    struct task_struct *task = NULL;
    mm_segment_t fs;
    memset(path, 0, 100);
    memset(proc_name, 0, 100);
    affl_cnt_process_mas = 0;

    for_each_process(task)
    {
        if(strlen(task->comm) == 15)
        {
            sprintf(path, "/proc/%d/cmdline", task->pid);
            f = filp_open(path, O_RDONLY, 0);
            if(f == NULL)
            {
                printk(KERN_ALERT "filp_open error!!.\n");
            }
            else
            {
                // Get current segment descriptor
                fs = get_fs();
                // Set segment descriptor associated to kernel space
                set_fs(get_ds());
                // Read the file
                f->f_op->read(f, proc_name, 100, &f->f_pos);
                // Restore segment descriptor
                set_fs(fs);
                //printk("affl_Driver: affl_handle(): proc_name = %s\n", proc_name);
            }
            filp_close(f,NULL);

            if(strlen(proc_name) > 15)
            {
                if(strrchr(proc_name, '/'))
                {
                    char* temp_proc_name = NULL;
                    temp_proc_name = strrchr(proc_name, '/') + 1;
                    //printk("temp_proc_name %s\n", temp_proc_name);
                    //printk("proc_name %s\n", proc_name);

                    strcpy(affl_list_process_mas[affl_cnt_process_mas].process_name, temp_proc_name);
                    affl_list_process_mas[affl_cnt_process_mas].PID = task->pid;
                    affl_cnt_process_mas++;
                }
                else
                {
                    strcpy(affl_list_process_mas[affl_cnt_process_mas].process_name, proc_name);
                    affl_list_process_mas[affl_cnt_process_mas].PID = task->pid;
                    affl_cnt_process_mas++;
                }
            }
            else if(strlen(proc_name) < 15)
            {
                strcpy(affl_list_process_mas[affl_cnt_process_mas].process_name, task->comm);
                affl_list_process_mas[affl_cnt_process_mas].PID = task->pid;
                affl_cnt_process_mas++;
            }
        }
        else if(strlen(task->comm) < 15)
        {
            strcpy(affl_list_process_mas[affl_cnt_process_mas].process_name, task->comm);
            affl_list_process_mas[affl_cnt_process_mas].PID = task->pid;
            affl_cnt_process_mas++;
        }
        memset(path, 0, 100);
        memset(proc_name, 0, 100);
    }
return 0;
}

void affl_init_process(void)
{
    affl_sys_kill = affl_find_sym("sys_kill");
}

void affl_clean_process(void)
{

}
#endif /* AFFL_PROCESS_C_ */
