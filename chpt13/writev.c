#include <stdio.h>
#include <sys/uio.h>

int main(int argc, char* argv[]) {
  struct iovec buf_vec[2];
  char buf1[] = "abcdef";
  char buf2[] = "123456";
  buf_vec[0].iov_base = buf1;
  buf_vec[0].iov_len = sizeof(buf1);
  buf_vec[1].iov_base = buf2;
  buf_vec[1].iov_len = sizeof(buf2);

  int str_len = writev(1, buf_vec, 2);
  puts("");  // print output buffer(file) content
  printf("message len: %d", str_len);
  return 0;
}
