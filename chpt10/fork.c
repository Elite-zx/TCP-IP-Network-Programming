#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int gVal = 10;
int main(int argc, char* argv[]) {
  int lVal = 20;
  pid_t pid;

  pid = fork();
  if (pid == 0) {
    printf("I am Son with gVal %d lVal %d !\n", gVal, lVal);
    gVal += 2;
    lVal += 2;
  } else {
    wait(NULL);
    printf("I am father with gVal %d lVal %d !\n", gVal, lVal);
    gVal -= 2;
    lVal -= 2;
  }
  return 0;
}
