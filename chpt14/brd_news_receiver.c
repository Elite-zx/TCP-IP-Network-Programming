include<arpa / inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

    const int BUFSIZE = 256;
void error_handling(const char* message);

int main(int argc, char* argv[]) {
  int receiver_sock = socket(PF_INET, SOCK_DGRAM, 0);

  if (argc != 2) {
    printf("Usage: %s <Port> \n", argv[0]);
    exit(1);
  }

  /*receive data from LAN */
  struct sockaddr_in receiver_addr;
  memset(&receiver_addr, 0, sizeof(receiver_addr));
  receiver_addr.sin_family = AF_INET;
  receiver_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  receiver_addr.sin_port = htons(atoi(argv[1]));

  if (bind(receiver_sock, (struct sockaddr*)&receiver_addr,
           sizeof(receiver_addr)) == -1)
    error_handling("bind() error!\n");

  char buf[BUFSIZE];

  /* parameters NULL and 0 here*/
  int str_len = recvfrom(receiver_sock, buf, BUFSIZE - 1, 0, NULL, 0);
  buf[str_len] = '\0';
  fputs(buf, stdout);

  close(receiver_sock);
  return 0;
}

void error_handling(const char* message) {
  fputs(message, stderr);
  exit(1);
}

