#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  pid_t pid = fork();
  if (pid == 0) {
    printf("Hi, I am a child process\n");
  } else {
    printf("Child process pid: %d\n", pid);
    sleep(30);
  }
  if (pid == 0)
    printf("End child process!\n");
  else
    printf("End parent process!\n");
  return 0;
}
