#ifndef AFFL_PROCESS_C_
#define AFFL_PROCESS_C_

#include "affl_process.h"

struct affl_list_process
{
	char process_name[50];
	int PID;
};

static void **taddr = NULL;

#define affl_sys_call "sys_execve"
#define affl_new_sys_call new_sys_execve
#define affl_old_sys_call old_sys_execve
#define affl__NR_call __NR_execve

struct affl_list_process affl_list_process_mas[300];
int affl_cnt_process_mas = 0;

struct affl_list_process affl_black_list[100];
static int affl_cnt_bl = 0;

unsigned int affl_handle(const char* input, char* user_buf)
{
	//Command/////////////////////////////////
	const char affl_kill[] = "kill@";
	const char affl_view[] = "view@";
	const char affl_getInfo[] = "getInfo@";
	const char affl_addProc[] = "addProc@";
	const char affl_rmProc[] = "rmProc@";
	const char affl_ex[] = "exist@";
	const char affl_getBL[] = "getBL@";

	char* proc_name = NULL;
	int PID = 0;
	unsigned int count_of_symbols = 0;

	printk("affl_Driver: affl_handle(): open\n");

	//Handle command view
	if (strstr(input, affl_view))
	{
		//affl_view_process(user_buf);
		count_of_symbols = affl_view_process(user_buf);
		affl_get_task();
		printk("affl_Driver: affl_handle(): view\n");
	}

	//Handle command kill
	else if (strstr(input, affl_kill))
	{
		printk("affl_Driver: affl_handle(): command =  kill\n");
		//Handle atribute
		affl_get_proc_name(input, &proc_name);
		affl_get_proc_PID(input, &PID);
		if(!affl_kill_process(proc_name, PID))
		{
			count_of_symbols = 1;
			copy_to_user(user_buf, "0", count_of_symbols);
		}
		else
		{
			count_of_symbols = 2;
			copy_to_user(user_buf, "-1", count_of_symbols);
		}
		printk("PID = %d, proc_name = %s\n", PID, proc_name);
	}
	//Handle command getInfo
	else if (strstr(input, affl_getInfo))
	{
		printk("affl_Driver: affl_handle():  command = getInfo \n");
		//Handle atribute
		affl_get_proc_PID(input, &PID);
		count_of_symbols = affl_get_info_for_process(PID, user_buf);
	}
	//Handle command addProc
	else if (strstr(input, affl_addProc))
	{
		printk("affl_Driver: affl_handle():  command = addProc \n");
		//Handle atribute
		affl_get_proc_name(input, &proc_name);
		if (!affl_bl_add(proc_name))
		{
			count_of_symbols = 1;
			copy_to_user(user_buf, "0", count_of_symbols);
		}
		else
		{
			count_of_symbols = 2;
			copy_to_user(user_buf, "-1", count_of_symbols);
		}
	}
	//Handle command rmProc
	else if (strstr(input, affl_rmProc))
	{
		printk("affl_Driver: affl_handle():  command = rmProc \n");
		//Handle atribute
		affl_get_proc_name(input, &proc_name);
		if (!affl_bl_rm(proc_name))
		{
			count_of_symbols = 1;
			copy_to_user(user_buf, "0", count_of_symbols);
		}
		else
		{
			count_of_symbols = 2;
			copy_to_user(user_buf, "-1", count_of_symbols);
		}
	} //Handle command exist
	else if (strstr(input, affl_getBL))
	{
		count_of_symbols = affl_get_black_list(user_buf);
	}
	else if (strstr(input, affl_ex))
	{
		count_of_symbols = affl_exist(user_buf);
	}
	vfree(proc_name);
	printk("affl_Driver: affl_handle(): close\n");
	return (count_of_symbols);
}

//get second param: command@atribute#
int affl_get_proc_name(const char* input, char** proc_name)
{
	char* begin = NULL;
	char* end = NULL;
	if (strchr(input, '@'))
	{
		begin = strchr(input, '@');
		end = strchr(input, '#');
		begin++;
		*proc_name = vmalloc(end - begin);
		memcpy(*proc_name, begin, (size_t) (end - begin));
		return (0);
	}
	return (-1);
}

int affl_get_proc_PID(const char* input, int* PID)
{
	char* strPID = NULL;
	if ((strPID = strchr(input, '%')))
	{
		kstrtoint((strPID + 1), 10, PID);
		return (0);
	}
	else
	{
		return (-1);
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
asmlinkage long (*affl_sys_readlink)(const char __user *path, char __user *buf,
		int bufsiz);

int affl_get_info_for_process(int pid, char* user_buf)
{
	struct file *f = NULL;
	mm_segment_t fs;
	char path[100];
	char temp[100];
	char cmdline[100];
	unsigned int count = 0;
	int i = 0;
	struct files_struct *current_files = NULL;
	struct fdtable *files_table = NULL;
	struct path files_path;
	char *cwd = NULL;
	char buf[300];
	struct task_struct* task = NULL;

	memset(temp, 0, 100);
	memset(path, 0, 100);
	memset(cmdline, 0, 100);
	memset(buf, 0, 300);

	affl_get_task();

	for_each_process(task)
	{
		if (task->pid == pid)
		{
			//Print PID and name///////////////////////////////////////////////
			for(i = 0; i < affl_cnt_process_mas; i++)
			{
				if(affl_list_process_mas[i].PID == pid)
				{
					sprintf(temp, "PID: %d Name %s\n", affl_list_process_mas[i].PID,
							affl_list_process_mas[i].process_name);
					copy_to_user(user_buf + count, temp, strlen(temp));
					count += strlen(temp);
					break;
				}
			}
			//print commandline////////////////////////////////////////////////
			sprintf(path, "/proc/%d/cmdline", pid);
			f = filp_open(path, O_RDONLY, 0);
			if (f == NULL )
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
				f->f_op->read(f, cmdline, 100, &f->f_pos);
				// Restore segment descriptor
				set_fs(fs);
				//printk("affl_Driver: affl_handle(): proc_name = %s\n", proc_name);
			}
			filp_close(f, NULL );
			sprintf(temp,"Command line: %s\n", cmdline);
			copy_to_user(user_buf + count, temp, strlen(temp));
			count += strlen(temp);

			//print files///////////////////////////////////////////////////////
			sprintf(temp,"Files:\nN  File\n");
			copy_to_user(user_buf + count, temp, strlen(temp));
			count += strlen(temp);
			current_files = task->files;
			files_table = files_fdtable(current_files);
			i = 0;
			while (files_table->fd[i] != NULL )
			{
				files_path = files_table->fd[i]->f_path;
				if(files_table->fd[i] == NULL) break;
				cwd = d_path(&files_path, buf, 300 * sizeof(char));
				sprintf(temp, "%d  %s\n", i, cwd);
				copy_to_user(user_buf + count, temp, strlen(temp));
				count += strlen(temp);
				i++;
			}
		}
	}

	if (count)
	{
		return (count);
	}
	else
	{
		sprintf(temp, "-1");
		copy_to_user(user_buf + count, temp, strlen(temp));
		count += strlen(temp);
		return (count);
	}
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

int affl_kill_process(const char* name, int PID)
{
	int flag = 0;
	int i = 0;
	affl_get_task();
	if (PID < 0)
	{
		for (i = 0; i < affl_cnt_process_mas; i++)
		{
			if (strstr(affl_list_process_mas[i].process_name, name))
			{
				affl_sys_kill(affl_list_process_mas[i].PID, 9);
				flag++;
			}
		}
		if (flag)
		{
			return (0);
		}
		else
		{
			return (-1);
		}
	}
	else if (PID > 0)
	{
		for (i = 0; i < affl_cnt_process_mas; i++)
		{
			if (strstr(affl_list_process_mas[i].process_name, name)
					|| affl_list_process_mas[i].PID == PID)
			{
				affl_sys_kill(affl_list_process_mas[i].PID, 9);
				flag++;
				break;
			}
		}
		if (flag)
		{
			return (0);
		}
		else
		{
			return (-1);
		}
	}
	else
	{
		return (-1);
	}
}

int affl_from_name_to_pid(char* name)
{
	int pid = 0;
	struct task_struct *task = NULL;
	for_each_process(task)
	{
		if(!strcmp(name, task->comm))
		{
			pid = task->pid;
			break;
		}
		else
		{
			printk("affl_from_name_to_pid(): Bed pid\n");
		}
	}
	return (pid);
}

int affl_get_task(void)
{
	char path[100];
	char proc_name[100];
	char cmdlineProc_name[100];
	struct file *f = NULL;
	struct task_struct* task = NULL;
	mm_segment_t fs;
	memset(cmdlineProc_name, 0, 100);
	memset(path, 0, 100);
	memset(proc_name, 0, 100);
	affl_cnt_process_mas = 0;

	for_each_process(task)
	{
		if(strlen(task->comm) == (TASK_COMM_LEN-1))
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
			}
			filp_close(f,NULL);

			if(strlen(proc_name) > (TASK_COMM_LEN-1))
			{
				strcpy(cmdlineProc_name, proc_name);
				if(strrchr(proc_name, '/'))
				{
					strcpy(cmdlineProc_name, strrchr(proc_name, '/') + 1);
				}
				if(strchr(cmdlineProc_name, ' '))
				{
					cmdlineProc_name [strchr(cmdlineProc_name, ' ') - cmdlineProc_name] = 0;
				}
				affl_add_list_process_mass(cmdlineProc_name, task->pid);
			}
			else if(strlen(proc_name) < (TASK_COMM_LEN-1))
			{
				affl_add_list_process_mass(task->comm, task->pid);
			}
		}
		else if(strlen(task->comm) < (TASK_COMM_LEN-1))
		{
			affl_add_list_process_mass(task->comm, task->pid);
		}
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
		printk("%s\n", affl_black_list[i].process_name);
	printk("+++++++++++++{print}++++++++++++++++\n\n");
}

int affl_bl_add(char* arg)
{
	int i = 0;
	if (affl_cnt_bl == 100)
		return -2;
	else
	{
		for (i = i; i < affl_cnt_bl; i++)
			if (!strcmp(arg, affl_black_list[i].process_name))
				break;
		if (i == affl_cnt_bl)
		{
			strcpy(affl_black_list[affl_cnt_bl].process_name, arg);
			affl_cnt_bl++;
			//affl_bl_print();
			affl_kill_process(arg, -1);//if process exist - kill it
			return 0;
		}
		else
		{
			affl_bl_print();
			return -1;
		}
	}
}

int affl_bl_rm(char* arg)
{
	int i = 0;
	if (affl_cnt_bl != 0)
	{
		for (i = i; i < affl_cnt_bl; i++)
			if (!strcmp(arg, affl_black_list[i].process_name))
				break;
		if (i == affl_cnt_bl)
		{
			//affl_bl_print();
			return -1;
		}
		else
		{
			for (i = i; i < affl_cnt_bl - 1; i++)
				strcpy(affl_black_list[i].process_name,
						affl_black_list[i + 1].process_name);
			affl_cnt_bl--;
			//affl_bl_print();
			return 0;
		}
	}
	else
		return -1;
}

int affl_bl_cmp(const char* arg)
{
	int i = 0;
	for (i = i; i < affl_cnt_bl; i++)
	{
		if ((strstr(arg, affl_black_list[i].process_name)))
		{
			return (0);
		}
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
		return (old_sys_execve(filename, argv, envp));
	}
	else
	{
		return (0);
	}
}

EXPORT_SYMBOL(affl_new_sys_call);

int affl_get_black_list(char* user_buf)
{
	char temp[300];
	unsigned int count = 0;
	int i = 0;
	sprintf(temp, "%d\n", affl_cnt_bl);
	count += strlen(temp);
	copy_to_user(user_buf + count, temp, strlen(temp));
	for (i = 0; i < affl_cnt_bl; i++)
	{
		sprintf(temp, "%s\n", affl_black_list[i].process_name);
		copy_to_user(user_buf + count, temp, strlen(temp));
		count += strlen(temp);
	}
	return (count);
}
int affl_exist(char* user_buf)
{
	copy_to_user(user_buf, "0", 1);
	return (1);
}

void affl_add_list_process_mass(const char* proc_name, int PID)
{
	strcpy(affl_list_process_mas[affl_cnt_process_mas].process_name, proc_name);
	affl_list_process_mas[affl_cnt_process_mas].PID = PID;
	affl_cnt_process_mas++;
}

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
