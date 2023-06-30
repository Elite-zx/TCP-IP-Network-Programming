#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

const int BUFSIZE = 256;
void error_handling(const char* message);

int main(int argc, char* argv[]) {
  /*like udp client*/
  int sock = socket(PF_INET, SOCK_DGRAM, 0);
  struct sockaddr_in broadcast_addr;

  if (argc != 3) {
    printf("Usage: %s <BroadcastIP> <Port>\n", argv[0]);
    exit(1);
  }
  /*send data to LAN or specific Area network*/
  memset(&broadcast_addr, 0, sizeof(broadcast_addr));
  broadcast_addr.sin_family = AF_INET;
  broadcast_addr.sin_addr.s_addr = inet_addr(argv[1]);
  broadcast_addr.sin_port = htons(atoi(argv[2]));

  /*socket disable broadcast as default, enable broadcast by set socket option
   * SO_BROADCAST to 1(true)*/
  int enable_brd = 1;
  setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &enable_brd, sizeof(enable_brd));

  FILE* fp = fopen("news.txt", "r");
  if (fp == NULL) error_handling("fopen() error!\n");

  char buf[BUFSIZE];
  if (!feof(fp)) {
    fgets(buf, BUFSIZE, fp);
    sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&broadcast_addr,
           sizeof(broadcast_addr));
    sleep(1);
  }
  fclose(fp);
  close(sock);
  return 0;
}

void error_handling(const char* message) {
  fputs(message, stderr);
  exit(1);
}
