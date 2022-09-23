#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

static char child_stack[5000];


int run (const char *name){
  printf("process created with PID: %ld \n", (long)getpid());
  char *_args[] = {(char *)name, (char *)0 };
  execvp(name, _args);
  return EXIT_SUCCESS;
}

int child_fn() {
  clearenv(); // clean all environment variables for the current process 
  printf("Hello I am CHILD :\n");
  printf("PID: %ld Parent:%d\n", (long)getpid(), getppid());
  run("/bin/bash");
  return EXIT_SUCCESS;
}

int main() {
  printf("Hello I am parent : \n");
  printf("PID: %ld Parent:%d \n", (long)getpid(), getppid());
  pid_t pid = clone(child_fn, child_stack+5000, CLONE_NEWPID | CLONE_NEWUTS | SIGCHLD , 0);
  printf("clone() = %d\n", pid);
  waitpid(pid, NULL, 0);
  return EXIT_SUCCESS;
}