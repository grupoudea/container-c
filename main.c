#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mount.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#define CGROUP_FOLDER "/sys/fs/cgroup/mycontainer/"
#define concat(a,b) (a"" b)


static char child_stack[5000];


void setHostName(char* hostName){
  sethostname(hostName,strlen(hostName)+2);
}

int run (const char *name){
  printf("process created with PID: %ld \n", (long)getpid());
  char *_args[] = {(char *)name, (char *)0 };
  execvp(name, _args);
  return EXIT_SUCCESS;
}
void setup_variables() {
  clearenv();// clean all environment variables for the current process 
  setenv("TERM", "xterm-256color", 0);
  setenv("PATH", "/bin/:/sbin/:usr/bin:/usr/sbin:", 0);
}

void setup_root(const char* folder){
  chroot(folder);
  chdir("/");
}

void clone_process(void (*function), int flags){
  //CLONE_NEWPID create new process and make it think it is running solo in the machine, own tree process
  //CLONE_NEWUTS isolate namespaces for new process
  //SIGCHLD tells the process to emit a signal when the process finished
  pid_t pid = clone(function, child_stack+5000, flags , 0);
  printf("clone() = %d\n", pid);
  waitpid(pid, NULL, 0);
}

int runBash(){
  return run("/bin/sh");
}

void write_rule(const char* path,const char* value){
  int fp = open(path, O_WRONLY | O_APPEND );
  write(fp, value, strlen(value));
  close(fp);
}

void limitProcess(){
  
  mkdir(CGROUP_FOLDER, S_IRUSR | S_IWUSR);
  int pid= (int)getpid();
  char strpid[10] ;
  sprintf(strpid,"%d",pid);
  const char* newStr=strpid;
  printf("the pid is : %d \n",pid);
  printf("the pid is : %s \n",newStr);

  write_rule(concat(CGROUP_FOLDER, "pids.max"), "20");// number of process 
  write_rule(concat(CGROUP_FOLDER, "memory.max"), "600000");//add limit for memory 
  write_rule(concat(CGROUP_FOLDER, "cgroup.procs"), newStr);//add pid to cfgroup proc
 
}



int child_fn() {
  
  printf("CHILD :\n");
  printf("PID: %ld Parent:%d\n", (long)getpid(), getppid());
  // limitProcess();
  setHostName("my-container");
  setup_variables(); 
  setup_root("./root");// set root path to the folder root 

  mount("proc", "/proc", "proc", 0, 0);// mount proc 


  pid_t pidb = clone(runBash, child_stack+5000, SIGCHLD , 0);
  waitpid(pidb, NULL, 0);

  umount("/proc");
  return EXIT_SUCCESS;
}


int main() {
  printf("PARENT: \n");
  printf("PID: %ld Parent:%d \n", (long)getpid(), getppid());
  limitProcess();
  clone_process(child_fn,CLONE_NEWPID | CLONE_NEWUTS | SIGCHLD);
  return EXIT_SUCCESS;
}