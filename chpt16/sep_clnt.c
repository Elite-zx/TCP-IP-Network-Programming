#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
const int BUF_SIZE = 256;  // size of type char
int main(int argc, char* argv[]) {
  /* allocate socket*/
  int clnt_sock = socket(PF_INET, SOCK_STREAM, 0);

  if (argc != 3) {
    printf("Usage: %s <IP> <port> \n", argv[0]);
    exit(1);
  }
  /* target server's ip address and port number*/
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(atoi(argv[2]));

  /* connect */
  if (connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) ==
      -1) {
    printf("connect error!\n");
    exit(1);
  }
  printf("Connected!\n");

  /*use standard function*/
  FILE* write_fp = fdopen(clnt_sock, "w");
  FILE* read_fp = fdopen(clnt_sock, "r");
  char message[BUF_SIZE];
  while (1) {
    if (fgets(message, BUF_SIZE, read_fp) == NULL) break;
    fputs(message, stdout);
    fflush(stdout);
  }
  fputs("From client: Thank u! server.\n", write_fp);
  fflush(write_fp);
  fclose(write_fp);
  fclose(read_fp);
  return 0;
}

