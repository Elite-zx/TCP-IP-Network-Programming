#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  int fd = 1;  // standard output
  int cpy_fd1, cpy_fd2;
  char str1[] = "Hello!\n";
  char str2[] = "I am copying file descriptor!\n";

  cpy_fd1 = dup(fd);
  cpy_fd2 = dup2(fd, 6);

  write(cpy_fd1, str1, sizeof(str1));
  write(cpy_fd2, str2, sizeof(str2));
  close(cpy_fd1);
  close(cpy_fd2);
  write(fd, str1, sizeof(str1));
  close(fd);
  write(fd, str2, sizeof(str2));  // no output
  return 0;
}
