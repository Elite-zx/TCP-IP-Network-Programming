#include <stdio.h>
#include <string.h>
#include <sys/uio.h>

const int BUFSIZE = 48;

int main(int argc, char* argv[]) {
  struct iovec buf_vec[2];
  char buf1[BUFSIZE];
  char buf2[BUFSIZE];
  memset(buf1, 0, sizeof(buf1));
  memset(buf2, 0, sizeof(buf2));

  buf_vec[0].iov_base = buf1;
  buf_vec[0].iov_len = 2;
  buf_vec[1].iov_base = buf2;
  buf_vec[1].iov_len = BUFSIZE;

  int read_len = readv(0, buf_vec, 2);
  printf("buf1: %s\n", buf1);
  printf("buf2: %s\n", buf2);
  printf("read len: %d\n", read_len);
  return 0;
}
