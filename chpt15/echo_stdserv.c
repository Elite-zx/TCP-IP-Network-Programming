#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
const int BUF_SIZE = 256;

void error_handling(const char* message);

int main(int argc, char* argv[]) {
  /* allocate socket*/
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
  if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    error_handling("bind() error! \n");

  /* start listening, size of waiting list is 5 */
  listen(serv_sock, 5);
  printf("Server start!\n");

  /*handle request: create another socket and connect to client */
  struct sockaddr_in clnt_addr;
  // function accept need a LValue
  socklen_t clnt_adr_sz = sizeof(clnt_addr);
  char message[BUF_SIZE];

  for (int i = 0; i < 5; ++i) {
    int clnt_sock =
        accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_adr_sz);
    if (clnt_sock == -1)
      error_handling("accept() error");
    else
      printf("Connect client %d \n", i + 1);

    FILE* write_fp = fdopen(clnt_sock, "w");
    FILE* read_fp = fdopen(clnt_sock, "r");
    /* standard I/O function*/
    while (!feof(read_fp)) {
      fgets(message, BUF_SIZE, read_fp);
      fputs(message, write_fp);
      fflush(write_fp);
    }
    fclose(write_fp);
    fclose(read_fp);
  }
  close(serv_sock);
  return 0;
}

void error_handling(const char* message) {
  fputs(message, stderr);
  exit(1);
}
