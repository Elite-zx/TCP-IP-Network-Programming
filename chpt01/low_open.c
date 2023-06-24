#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void error_handling(char *message);

int main() {
  int fd;
  char buf[] = "Let's go!\n";
  // O_CREAT | O_WRONLY | O_TRUNC
  // 是文件打开模式，将创建新文件，并且只能写。如存在 data.txt
  // 文件，则清空文件中的全部数据。
  //这里使用了`|`运算符将多个选项参数组合在一起。这是因为 open
  // 系统调用的选项参数是一个位掩码，每个选项都有一个对应的位标志，可以使用位运算符组合多个选项。
  fd = open("data.txt", O_CREAT | O_WRONLY | O_TRUNC);
  if (fd == -1) error_handling("open() error!");
  printf("file descriptor: %d \n", fd);
  // 向对应 fd 中保存的文件描述符的文件传输 buf 中保存的数据。
  if (write(fd, buf, sizeof(buf)) == -1) error_handling("write() error!");
  close(fd);
  return 0;
}

void error_handling(char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
