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
  struct sockaddr_in multicast_addr;

  if (argc != 3) {
    printf("Usage: %s <GroupIP> <Port>\n", argv[0]);
    exit(1);
  }
  memset(&multicast_addr, 0, sizeof(multicast_addr));
  multicast_addr.sin_family = AF_INET;
  multicast_addr.sin_addr.s_addr = inet_addr(argv[1]);
  multicast_addr.sin_port = htons(atoi(argv[2]));

  int TTL_val = 64;
  /* set TTL: Time To Live. Decrement 1 for each router passed*/
  setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &TTL_val, sizeof(TTL_val));
  FILE* fp = fopen("news.txt", "r");
  if (fp == NULL) error_handling("fopen() error!\n");

  char buf[BUFSIZE];
  if (!feof(fp)) {
    fgets(buf, BUFSIZE, fp);
    sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&multicast_addr,
           sizeof(multicast_addr));
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
