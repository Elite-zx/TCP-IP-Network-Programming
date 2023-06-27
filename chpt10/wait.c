#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  pid_t pid = fork();

  if (pid == 0)  // first child process
  {
    return 3;
  } else {
    printf("First child process pid: %d\n", pid);

    pid = fork();
    if (pid == 0) {
      exit(7);
    } else {
      printf("Second child process pid: %d\n", pid);

      int status;
      wait(&status);
      if (WIFEXITED(status))
        printf("First child process end and return value %d!\n",
               WEXITSTATUS(status));  // print 3

      wait(&status);
      if (WIFEXITED(status))
        printf("Second child process end and return value %d!\n",
               WEXITSTATUS(status));  // print 7
    }
  }
  return 0;
}
