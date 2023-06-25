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

  if (connect(sock, (struct sockaddr*)&target_serv_addr,
              sizeof(target_serv_addr)) == -1)
    error_handling("connect() error!\n");
  char message[BUF_SIZE];
  while (1) {
    fputs("Input message(Q to quit): ", stdout);
    fgets(message, BUF_SIZE, stdin);

    if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) break;
    write(sock, &message, strlen(message));

    int recv_len = read(sock, message, strlen(message));
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

