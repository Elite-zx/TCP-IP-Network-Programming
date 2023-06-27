#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void childproc_handler(int sig) {
  int status;
  pid_t pid = waitpid(-1, &status, WNOHANG);
  if (WIFEXITED(status)) {
    printf("Remove child process pid %d and return value %d!\n", pid,
           WEXITSTATUS(status));  // print 3
  }
}

int main(int argc, char* argv[]) {
  struct sigaction act;
  act.sa_handler = childproc_handler;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);  // set 0
  sigaction(SIGCHLD, &act, 0);

  pid_t pid = fork();
  if (!pid) {
    puts("Hi! I am child process one");
    return 1;
  } else {
    printf("First child process with pid %d\n", pid);
    pid = fork();
    if (!pid) {
      puts("Hi! I am child process two");
      sleep(2);
      exit(2);
    } else {
      printf("Second child process with pid %d\n", pid);
      puts("waiting...(child process end)");
      for (int i = 0; i < 3; i++) sleep(5);  // actually 2 seconds
    }
  }
  puts("parent process exit!\n");
  return 0;
}

