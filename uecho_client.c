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
  int sock = socket(PF_INET, SOCK_DGRAM, 0);
  struct sockaddr_in target_serv_addr;

  if (argc != 3) {
    printf("Usage: %s <IP> <port> \n", argv[0]);
    exit(1);
  }
  memset(&target_serv_addr, 0, sizeof(target_serv_addr));
  target_serv_addr.sin_family = AF_INET;
  target_serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  target_serv_addr.sin_port = htons(atoi(argv[2]));

  char message[BUF_SIZE];
  while (1) {
    fputs("Input message(Q to quit): ", stdout);
    fgets(message, BUF_SIZE, stdin);

    if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) break;

    /*automatically allocate ip and port to client socket when executing
     * function sendto */
    if (sendto(sock, &message, strlen(message), 0,
               (struct sockaddr*)&target_serv_addr,
               sizeof(target_serv_addr)) == -1)
      error_handling("sendto() error!\n");

    socklen_t serv_adr_sz = sizeof(target_serv_addr);
    int recv_len = recvfrom(sock, &message, strlen(message), 0,
                            (struct sockaddr*)&target_serv_addr, &serv_adr_sz);
    if (recv_len == -1) error_handling("recvfrom() error!\n");
    message[recv_len] = '\0';
    printf("Message from server: %s", message);
  }
  close(sock);
  return 0;
}

void error_handling(const char* message) {
  fputs(message, stderr);
  exit(1);
}
