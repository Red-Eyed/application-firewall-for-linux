#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <iostream>


static void *affl_thread(void *arg);


int global;

int main(int argc, char **argv)
{
  
  
  global=1;
  int affl_pipe[2];
  //char buf[100];
  char message[100];
  char buf_param[150];
  memset(message, 0, 100);
  
  if (pipe(affl_pipe) == -1)
  {
    printf("ERROR WITH PIPE\n");
    return 0;
  }
  
  sprintf(buf_param, "insmod ../mod_mod.ko mod_param0=%d mod_param1=%d",affl_pipe[0],affl_pipe[1]);
  
  printf("@pipe0 %d \n", affl_pipe[0]);
  printf("@pipe1 %d \n", affl_pipe[1]);
  pthread_t thread;
  int result=pthread_create(&thread, NULL, affl_thread,(void*)affl_pipe);
  usleep(99999);
  system(buf_param);
  char p='r';
  std::cin>>p;
  if(p=='s')global=0;
  
  
  
  close(affl_pipe[0]);
  close(affl_pipe[1]);
  system("rmmod mod_mod");
  return 0;
} 


static void *affl_thread(void *arg)
{ 
 int *affl_pipe = (int*) arg;
char buf[100];
 while(global)
 {
 //printf("pipe0 %d \n", pipe_[0]);
 //printf("pipe1 %d \n", pipe_[1]);
 //printf("global %d \n", global);
 for(int i = 0; (read(affl_pipe[0], buf + i, 1)); i++);
 printf("massege %s \n", buf);
 //usleep(99999);
 }
 return 0;
}