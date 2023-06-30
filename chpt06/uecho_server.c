#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

const int BUF_SIZE = 100;
void error_handling(const char* message);

int main(int argc, char* argv[]) {
  int serv_sock = socket(PF_INET, SOCK_DGRAM, 0);

  if (argc != 2) {
    printf("Usage: %s <port> \n", argv[0]);
    exit(1);
  }


  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));

  if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    error_handling("bind() error!\n");

  struct sockaddr_in clnt_addr;
  socklen_t clnt_addr_sz = sizeof(clnt_addr);
  char message[BUF_SIZE];
  while (1) {
    int recv_len = recvfrom(serv_sock, &message, BUF_SIZE, 0,
                            (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
    if (recv_len == -1) error_handling("recvfrom() error!\n");
    sendto(serv_sock, &message, recv_len, 0, (struct sockaddr*)&clnt_addr,
           clnt_addr_sz);
  }
  close(serv_sock);
  return 0;
}

void error_handling(const char* message) {
  fputs(message, stderr);
  exit(1);
}
