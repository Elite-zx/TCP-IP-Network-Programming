#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  pid_t pid = fork();

  if (pid == 0) {
    return 3;
  } else {
    int status;
    if (!waitpid(pid, &status, WNOHANG)) {
      fputs("no stuck if no child end!\n", stdout);
    }
    if (WIFEXITED(status))
      printf("First child process end and return value %d!\n",
             WEXITSTATUS(status));  // print 3
  }
  return 0;
}
