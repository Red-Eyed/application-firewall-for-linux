#ifndef AFFL_PROCESS_C_
#define AFFL_PROCESS_C_

#include "affl_process.h"

struct affl_list_process
{
	char process_name[50];
	int PID;
};

static void **taddr;



#define affl_sys_call "sys_execve"
#define affl_new_sys_call new_sys_execve
#define affl_old_sys_call old_sys_execve
#define affl__NR_call __NR_execve

struct affl_list_process affl_list_process_mas[300];
int affl_cnt_process_mas = 0;

struct affl_list_process affl_list_process_bl[100];
static int affl_cnt_bl = 0;

unsigned int affl_handle(const char* input, char* user_buf)
{
	//Command/////////////////////////////////
	const char affl_kill[] = "kill@";
	const char affl_view[] = "view@";
	const char affl_getInfo[] = "getInfo@";
	const char affl_addProc[] = "addProc@";
	const char affl_rmProc[] = "rmProc@";


	char* proc_name = NULL;
	int PID = 0;

	printk("affl_Driver: affl_handle(): open\n");

	//Handle command view
	if (strstr(input, affl_view))
	{
		//affl_view_process(user_buf);
		unsigned int count = 0;
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
		affl_get_proc_PID(input, &PID);
		printk("affl_Driver: affl_handle(): atribute =  %s\n", proc_name);
		affl_kill_process(proc_name, PID);
	}
	//Handle command getInfo
	else if (strstr(input, affl_getInfo))
	{
		printk("affl_Driver: affl_handle():  command = getInfo \n");
		//Handle atribute
		affl_get_proc_name(input, &proc_name);
		PID = affl_from_name_to_pid(proc_name);
		affl_get_info_for_process(PID);
	}
	//Handle command addProc
	else if (strstr(input, affl_addProc))
	{
		printk("affl_Driver: affl_handle():  command = addProc \n");
		//Handle atribute
		affl_get_proc_name(input, &proc_name);
		affl_bl_add(proc_name);
	}
	//Handle command rmProc
	else if (strstr(input, affl_rmProc))
	{
		printk("affl_Driver: affl_handle():  command = rmProc \n");
		//Handle atribute
		affl_get_proc_name(input, &proc_name);
		affl_bl_rm(proc_name);
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
        printk("affl_get_proc_name(): proc_name = %s \n", *proc_name);
    }
}

void affl_get_proc_PID(const char* input, int* PID)
{
    char* strPID = NULL;
    if ((strPID = strstr(input, "%")))
    {
        kstrtoint((strPID+1), 10, PID);
        printk("affl_get_proc_PID(): PID = %d\n", *PID);
        printk("affl_get_proc_PID(): strPID = %s\n", strPID);
    }
    else
    {
        *PID = 0;
    }
}


unsigned int affl_view_process(char* user_buf)
{
	char temp[100];
	unsigned int count = 0;
	int i = 0;

	printk("affl_Driver: affl_handle(): command =  view\n");
	affl_get_task();
	i = affl_get_quantity_tasks();
	sprintf(temp, "%d\n", i);
	copy_to_user(user_buf + count, temp, strlen(temp));
	count += strlen(temp);
	for (i = 0; i < affl_cnt_process_mas; i++)
	{
		sprintf(temp, "%d %s\n", affl_list_process_mas[i].PID,
				affl_list_process_mas[i].process_name);
		copy_to_user(user_buf + count, temp, strlen(temp));
		count += strlen(temp);
	}
	printk("count = %d\n", count);
	return (count);
}
asmlinkage long (*affl_sys_readlink)(const char __user *path,
				char __user *buf, int bufsiz);

void affl_get_info_for_process(int pid)
{
	/*char *tmp;
	char *pathname;
	struct file *file;
	struct path path;
	struct files_struct *files;
	spin_lock(&files->file_lock);
	file = fcheck_files(files, fd);
	if (!file) {
	    spin_unlock(&files->file_lock);
	    return -ENOENT;
	}

	path = file->f_path;
	path_get(&file->f_path);
	spin_unlock(&files->file_lock);

	tmp = (char *)__get_free_page(GFP_TEMPORARY);

	if (!tmp) {
	    path_put(&path);
	    return -ENOMEM;
	}

	pathname = d_path(&path, tmp, PAGE_SIZE);
	path_put(&path);

	if (IS_ERR(pathname)) {
	    free_page((unsigned long)tmp);
	    return PTR_ERR(pathname);
	}

	 //do something here with pathname
	printk("path = %s/n", pathname);

	free_page((unsigned long)tmp);*/
}

void* find_sym(const char *sym)
{
	static unsigned long faddr = 0;
	int symb_fn(void* data, const char* sym, struct module* mod,
			unsigned long addr)
	{
		if (0 == strcmp((char*) data, sym))
		{
			faddr = addr;
			return (1);
		}
		else
			return (0);
	}

	kallsyms_on_each_symbol(symb_fn, (void*) sym);
	return ((void*) faddr);
}

asmlinkage long (*affl_sys_kill)(int pid, int sig);

void affl_kill_process(const char* name, int PID)
{
    int i = 0;
    affl_get_task();
    if(PID < 0)
    {
        for (i = 0; i < affl_cnt_process_mas; i++)
        {
            if (strstr(affl_list_process_mas[i].process_name, name))
            {
                affl_sys_kill(affl_list_process_mas[i].PID, 9);
            }
        }
    }
    else if (PID > 0)
    {
        for (i = 0; i < affl_cnt_process_mas; i++)
        {
            if (strstr(affl_list_process_mas[i].process_name, name) &&
                    affl_list_process_mas[i].PID == PID)
            {
                affl_sys_kill(affl_list_process_mas[i].PID, 9);
                break;
            }
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
			printk("affl_from_name_to_pid(): pid = %d\n", pid);
			break;
		}
		else
		{
			printk("affl_from_name_to_pid(): bred pid\n");
		}
	}
	printk("affl_from_name_to_pid(): end\n");
	return (pid);
}

int affl_get_task(void)
{
	char path[100];
	char proc_name[100];
	struct file *f = NULL;
	struct task_struct *task = NULL;
	mm_segment_t fs;
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
		//printk("%d %s\n", task->pid, task->comm);
	}
	return (0);
}

int affl_get_quantity_tasks(void)
{
	struct task_struct *task = NULL;
	int quantity = 0;
	for_each_process(task)
	{
		quantity++;
	}
	return quantity;
}

void affl_bl_print(void)
{
	int i = 0;
	printk("\n\n============={print}=================\n");
	for (i = i; i < affl_cnt_bl; i++)
		printk("%s\n", affl_list_process_bl[i].process_name);
	printk("+++++++++++++{print}++++++++++++++++\n\n");
}

void affl_bl_add(char* arg)
{
	strcpy(affl_list_process_bl[affl_cnt_bl].process_name, arg);
	affl_cnt_bl++;
	affl_bl_print();
}

int affl_bl_rm(char* arg)
{
	int i = 0;
	if (affl_cnt_bl != 0)
	{
		for (i = i; i < affl_cnt_bl; i++)
			if (!strcmp(arg, affl_list_process_bl[i].process_name))
				break;
		if (i == affl_cnt_bl)
		{
			affl_bl_print();
			return (-1);
		}
		else
		{
			for (i = i; i < affl_cnt_bl - 1; i++)
				strcpy(affl_list_process_bl[i].process_name,
						affl_list_process_bl[i + 1].process_name);
			affl_cnt_bl--;
			affl_bl_print();
			return (0);
		}
	}
	return (-1);
}

int affl_bl_cmp(const char* arg)
{
	int i = 0;
	char* kk = NULL;
	//printk("\n+++++++arg= %s ++++++++\n",arg);
	for (i = i; i < affl_cnt_bl; i++)
	{
		if ((kk = strstr(arg, affl_list_process_bl[i].process_name)))
		{
			return (0);
		}
		//printk("\n+process_name %s +\n",affl_list_process_bl[i].process_name);
		//printk("\n=result %s =\n", kk);
	}

	return (1);
}

asmlinkage long (*affl_old_sys_call)(const char __user *filename,
	const char __user * const __user *argv,
	const char __user * const __user *envp);

asmlinkage long affl_new_sys_call(const char __user *filename,
	const char __user * const __user *argv,
	const char __user * const __user *envp)
{
	if (affl_bl_cmp(filename))
	{
		//printk("\n+++++++norm %s++++++++\n",filename);
		return (old_sys_execve(filename, argv, envp));
	}
	else
	{
		printk("\n+++++++lol %s++++++++\n", filename);
		return (0);

	}
}

EXPORT_SYMBOL(affl_new_sys_call);

int affl_init_process(void)
{
	void *waddr;

	affl_sys_kill = find_sym("sys_kill");
	affl_sys_readlink = find_sym("sys_readlink");


	//init chek run process
	if ((taddr = find_sym("sys_call_table")) != NULL )
	{
		printk("sys_call_table = %p\n", taddr);
	}
	else
	{
		printk("! sys_call_table NO_detekted\n");
		return (-EINVAL);
	}
	affl_old_sys_call = (void*) taddr[affl__NR_call];
	printk("%d[affl__NR_call] = %p\n", affl__NR_call, affl_old_sys_call);
	if ((waddr = find_sym(affl_sys_call)) != NULL )
	{
		printk("sys_call = %p\n", waddr);
	}
	else
	{
		printk("! sys_call NO_detekted\n");
		return (-EINVAL);
	}
	if (affl_old_sys_call != waddr)
	{
		printk("NEPONYATNO\n");
		return (-EINVAL);
	}
	printk("new sys_call = %p\n", &affl_new_sys_call);
	show_cr0();
	rw_enable();
	taddr[affl__NR_call] = affl_new_sys_call;
	show_cr0();
	rw_disable();
	show_cr0();
	/////////////////////////////////////////////////////////////////////
	return (0);
}
void affl_clean_process(void)
{
	printk("sys_call(unload) = %p\n", (void*) taddr[affl__NR_call]);
	rw_enable();
	taddr[affl__NR_call] = affl_old_sys_call;
	rw_disable();
	return;
}
#endif /* AFFL_PROCESS_C_ */
