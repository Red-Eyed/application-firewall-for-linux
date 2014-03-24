#include "affl_process.h"

#define affl_sys_call "sys_execve"
#define affl_new_sys_call new_sys_execve
#define affl_old_sys_call old_sys_execve
#define affl__NR_call __NR_execve

struct affl_list_process{
char process_name[50];  
int PID;  
};

/*struct affl_list_process_mas[300];
static int affl_cnt_process_mas=0;*/

struct affl_list_process affl_list_process_bl[100];
static int affl_cnt_bl=0;

void affl_bl_print(void)
{
  int i=0;
  printk("\n\n============={print}=================\n");
  for(i=i;i<affl_cnt_bl;i++)
    printk("%s\n", affl_list_process_bl[i].process_name);
  printk("+++++++++++++{print}++++++++++++++++\n\n");  
  
}

void affl_bl_add(char* arg)
{
  strcpy(affl_list_process_bl[affl_cnt_bl].process_name,arg);
  affl_cnt_bl++;
  affl_bl_print();
}

int affl_bl_rm(char* arg)
{
  int i=0;
  if(affl_cnt_bl!=0)
  {
  for(i=i;i<affl_cnt_bl;i++)
    if(!strcmp(arg,affl_list_process_bl[i].process_name))
      break;
  if(i==affl_cnt_bl)  
  {
    affl_bl_print();
    return -1;
  }
  else
  {
    for(i=i;i<affl_cnt_bl-1;i++)
      strcpy(affl_list_process_bl[i].process_name,affl_list_process_bl[i+1].process_name);
      affl_cnt_bl--;
    affl_bl_print();
    return 0;
  }
  }
  return -1;
}

static int affl_bl_cmp(const char* arg)
{
  int i=0;
  char* kk = NULL;
    //printk("\n+++++++arg= %s ++++++++\n",arg);
  for(i=i;i<affl_cnt_bl;i++)
  {
    if((kk=strstr(arg,affl_list_process_bl[i].process_name)))
    {
      return 0;
    }
    //printk("\n+process_name %s +\n",affl_list_process_bl[i].process_name);
    //printk("\n=result %s =\n", kk);
  }   
  
  return 1;   
}

asmlinkage long (*affl_old_sys_call)(const char __user *filename,const char __user *const __user *argv,const char __user *const __user *envp);

asmlinkage long affl_new_sys_call(const char __user *filename,const char __user *const __user *argv,const char __user *const __user *envp)
{
  if(affl_bl_cmp(filename))
  {
    //printk("\n+++++++norm %s++++++++\n",filename);
    return old_sys_execve(filename,argv,envp); 
  }
  else
  {
    printk("\n+++++++lol %s++++++++\n",filename);
    return 0;    
    
  }
}

EXPORT_SYMBOL(affl_new_sys_call);

static void **taddr;

/*static unsigned long hex2adr( char *b ) {
    unsigned long res = 0;
    char *p = b, *d;
    const char *dig = "0123456789abcdef";
    while( ( d = strchr( dig, *p ) ) != NULL ) {
        res = ( res << 4 ) + ( d - dig );
        p++;
    }
    return res;
}
*/

static void* find_sym( const char *sym )
{
  static unsigned long faddr = 0; 
  int symb_fn( void* data, const char* sym, struct module* mod, unsigned long addr ) 
  {
    if( 0 == strcmp( (char*)data, sym ) ) 
    {
      faddr = addr;
      return 1;
    }
    else return 0;
  };
  kallsyms_on_each_symbol( symb_fn, (void*)sym );
  return (void*)faddr;
}

/*int NameToPid(char *arg)
{
  int cnt = -1;
	struct task_struct* iter;
	for_each_process(iter){
		if(strcmp(iter->comm,arg))cnt=iter->pid;break;
	}
	printk("pid: %d comm: %s\n", cnt, iter->comm);
	
  return cnt;
}

char* PidToName(int arg)
{
  char cnt[1];cnt[0]='\0';
	struct task_struct* iter;
	for_each_process(iter){
		if(iter->pid==arg)cnt[0]='o';break;
	}
	printk("pid: %d comm: %s\n", iter->pid, iter->comm);
	if(cnt[0]=='o')
  return iter->comm;
  else
   return cnt; 
}*/


void affl_view(void)
{
  	int cnt = 0;
	struct task_struct* iter;
	for_each_process(iter){
		printk("pid: %d comm: %s\n", iter->pid, iter->comm);
		cnt++;
	}
	printk("processess: %d \n", cnt);
}

int affl_process_load(void)
{
     //affl_view();
  void *waddr;
if ((taddr = find_sym("sys_call_table")) != NULL )
printk("sys_call_table = %p\n", taddr);
else
{
printk("! sys_call_table NO_detekted\n");
return -EINVAL;
}
affl_old_sys_call = (void*) taddr[affl__NR_call];
printk("%d[affl__NR_call] = %p\n", affl__NR_call, affl_old_sys_call);
if ((waddr = find_sym(affl_sys_call)) != NULL )
printk("sys_call = %p\n", waddr);
else
{
printk("! sys_call NO_detekted\n");
return -EINVAL;
}
if (affl_old_sys_call != waddr)
{
printk("NEPONYATNO\n");
return -EINVAL;
}
printk("new sys_call = %p\n", &affl_new_sys_call);
show_cr0();
rw_enable();
taddr[affl__NR_call] = affl_new_sys_call;
show_cr0();
rw_disable();
show_cr0();
return 0;
}

void affl_process_unload(void)
{
printk("sys_call(unload) = %p\n", (void*) taddr[affl__NR_call]);
rw_enable() ;
taddr[affl__NR_call] = affl_old_sys_call;
rw_disable();
return;
  
}