#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

void error_handling(const char* message);

int main(int argc, char* argv[]) {
  int tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
  int udp_sock = socket(PF_INET, SOCK_DGRAM, 0);
  printf("SO_TYPE---SOCK_STREAM: %d\n", SOCK_STREAM);
  printf("SO_TYPE---SOCK_DGRAM: %d\n", SOCK_DGRAM);

  int optval;
  socklen_t optlen = sizeof(optval);
  if (getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, &optval, &optlen) == -1)
    error_handling("getsockopt() error!");
  printf("getsockopt of tcp_sock in SO_TYPE: %d\n", optval);
  if (getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, &optval, &optlen) == -1)
    error_handling("getsockopt() error!");
  printf("getsockopt of udp_sock in SO_TYPE: %d\n", optval);
  close(tcp_sock);
  close(udp_sock);
  return 0;
}

void error_handling(const char* message) {
  fputs(message, stderr);
  exit(1);
}
