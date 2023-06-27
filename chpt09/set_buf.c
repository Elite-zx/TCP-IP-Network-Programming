#include <asm-generic/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

void error_handling(const char* message);
void getBufSize(int sock);

int main(int argc, char* argv[]) {
  int tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
  int rcv_buf = 2048, snd_buf = 2048;

  getBufSize(tcp_sock);

  if (setsockopt(tcp_sock, SOL_SOCKET, SO_RCVBUF, &rcv_buf, sizeof(rcv_buf)) ==
      -1)
    error_handling("setsockopt() error!\n");

  if (setsockopt(tcp_sock, SOL_SOCKET, SO_SNDBUF, &snd_buf, sizeof(snd_buf)) ==
      -1)
    error_handling("setsockopt() error!\n");

  getBufSize(tcp_sock);
}

void getBufSize(int sock) {
  int rsv_buf, snd_buf;
  socklen_t len = sizeof(rsv_buf);

  if (getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &rsv_buf, &len) == -1)
    error_handling("getsockopt() error!\n");
  printf("getsockopt of tcp_sock in SO_RCVBUF: %d\n", rsv_buf);

  len = sizeof(snd_buf);
  if (getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &snd_buf, &len) == -1)
    error_handling("getsockopt() error!\n");
  printf("getsockopt of tcp_sock in SO_SNDBUF: %d\n", snd_buf);
}

void error_handling(const char* message) {
  fputs(message, stderr);
  exit(1);
}
