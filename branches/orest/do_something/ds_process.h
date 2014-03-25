#define DS_MAX_BUF_SIZE 1024

struct tty_struct *my_tty = 0;

static char ds_buf[DS_MAX_BUF_SIZE];

static struct task_struct* simple_task = 0;

static void print_string(char *msg) {
	if (my_tty != 0) {
		my_tty->driver->ops->write(my_tty, msg, strlen(msg));
		my_tty->driver->ops->write(my_tty, "\015\012", 2);
	}
}

static int simple_thread(void* arg){
	int cnt = 0;
	struct task_struct* iter;
	for_each_process(iter){
		snprintf(ds_buf, DS_MAX_BUF_SIZE, "pid:%d comm%s", iter->pid, iter->group_leader->comm);
		print_string(ds_buf	);
		cnt++;
	}
	snprintf(ds_buf, DS_MAX_BUF_SIZE, "processess %d ", cnt);
	print_string(ds_buf);
	return (0);
}

int ds_process_init(void)
{
  my_tty = current->signal->tty;
	simple_task = kthread_run(simple_thread, 0, "event simple");
	if(IS_ERR(simple_task)){
		return -1;

	}
  return SUCCESS;
}

void ds_process_exit(void)
{
  print_string("The module process has bean removed");
}