#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

const int BUFSIZE = 36;

int main(int agrc, char* argv[]) {
  clock_t start_time = clock();
  int fd1 = open("IO_test.txt", O_RDONLY);
  int fd2 = open("cpy1.txt", O_WRONLY | O_TRUNC | O_CREAT);

  int len = 0;
  char buf[BUFSIZE];
  while ((len = read(fd1, buf, BUFSIZE)) > 0) {
    write(fd2, buf, len);
  }

  close(fd1);
  close(fd2);
  clock_t end_time = clock();
  printf("total running time: %f", (double)end_time - start_time);
  return 0;
}
