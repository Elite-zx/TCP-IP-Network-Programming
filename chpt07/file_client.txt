#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

const int BUF_SIZE = 256;
void error_handling(char *message);

int main(int argc, char *argv[]) {
  int serv_sock = socket(PF_INET, SOCK_STREAM, 0);

  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    exit(1);
  }
  /*initialize ip address and port number*/
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));

  /*bind ip address and port to socket*/
  if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    error_handling("bind() error! \n");

  /* start listening, size of waiting list is 5 */
  listen(serv_sock, 5);
  printf("Server start!\n");
  /*handle request: create another socket and connect to client */
  struct sockaddr_in clnt_addr;
  // function accept need a LValue
  socklen_t clnt_addr_size = sizeof(clnt_addr);
  char buf[BUF_SIZE];
  FILE *fp = fopen("file_server.c", "rb");
  if (fp == NULL) error_handling("fopen() error!\n");
  int clnd_sock;
  if ((clnd_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr,
                          &clnt_addr_size)) == -1)
    error_handling("accept() error!\n");
  int read_cnt;
  while (1) {
    read_cnt = fread((void *)buf, 1, BUF_SIZE, fp);
    /*file has been read over*/
    if (read_cnt < BUF_SIZE) {
      write(clnd_sock, buf, read_cnt);
      break;
    }
    /*file has not been read over */
    write(clnd_sock, buf, BUF_SIZE);
  }

  /* half close, keep input(read) stream*/
  shutdown(clnd_sock, SHUT_WR);
  read(clnd_sock, buf, BUF_SIZE);
  printf("Message from client: %s \n", buf);

  /*close both stream*/
  close(clnd_sock);
  close(serv_sock);
  fclose(fp);
  return 0;
}

void error_handling(char *message) {
  fputs(message, stderr);
  exit(1);
}
