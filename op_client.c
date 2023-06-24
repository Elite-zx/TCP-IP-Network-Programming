#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
const int BUF_SIZE = 256;  // size of type char
const int OPSZ = 4;
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
  /* write and read */
  int opnd_cnt;
  char opmsg[BUF_SIZE];  // char type because of function read
  fputs("Operand count: ", stdout);
  scanf("%d", &opnd_cnt);
  opmsg[0] = (char)opnd_cnt;
  for (int i = 0; i < opnd_cnt; ++i) {
    printf("Operand %d: ", i + 1);
    scanf("%d", (int*)&opmsg[i * OPSZ + 1]);
  }
  fgetc(stdin);  // remove carriage return
  fputs("Operator: ", stdout);
  scanf("%c", &opmsg[opnd_cnt * OPSZ + 1]);
  write(clnt_sock, opmsg, opnd_cnt * OPSZ + 2);
  int result;
  read(clnt_sock, &result, OPSZ);
  printf("Operation result: %d \n", result);

  /*close socket */
  close(clnt_sock);
  return 0;
}
