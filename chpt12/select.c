#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

void error_handling(char *message);
const int BUFSIZE = 256;

int main(int argc, char *argv[]) {
  fd_set _read, tmp;
  FD_ZERO(&_read);
  FD_SET(0, &_read);  // 0 is file descriptor of standard input

  int max_file_Decriptor = 1;
  struct timeval timeout;
  char buf[BUFSIZE];
  while (1) {
    tmp = _read;
    /*timeout value = 5+ 0.5 = 5.5*/
    timeout.tv_sec = 5;
    timeout.tv_usec = 500000;
    int ret = select(max_file_Decriptor, &tmp, 0, 0, &timeout);
    if (ret == -1) {
      error_handling("select() error");
    } else if (!ret) {
      puts("time out!");
    } else {
      if (FD_ISSET(0, &tmp)) {
        int str_len = read(0, buf, BUFSIZE - 1);
        buf[str_len] = '\0';
        printf("message from stdin: %s", buf);
      }
    }
  }
  return 0;
}

void error_handling(char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
