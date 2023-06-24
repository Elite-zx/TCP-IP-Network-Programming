#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
const int BUF_SIZE = 256;
const int OPSZ = 4;

void error_handling(const char* message);
int caculate(int opnum, int* operand, char operator);

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
  socklen_t clnt_addr_size = sizeof(clnt_addr);
  int opnd_cnt = 0;
  char opinfo[BUF_SIZE];
  int result = 0;
  for (int i = 0; i < 5; ++i) {
    int clnt_sock =
        accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    printf("Handle connection!\n");
    // the file pointer will moved by one byte
    read(clnt_sock, &opnd_cnt, 1);

    /* handle one connection at a time */
    int read_len = 0, read_cnt = 0;
    while (read_len < opnd_cnt * OPSZ + 1) {
      // count by bytes;
      read_cnt = read(clnt_sock, &opinfo[read_len], BUF_SIZE - 1);
      if (read_cnt == -1) error_handling("read() error!\n");
      read_len += read_cnt;
    }
    result = caculate(opnd_cnt, (int*)opinfo, opinfo[read_len - 1]);
    write(clnt_sock, (char*)&result, sizeof(result));
    close(clnt_sock);
  }
  close(serv_sock);
  return 0;
}

void error_handling(const char* message) {
  fputs(message, stderr);
  exit(1);
}

int caculate(int opnum, int* operand, char operator) {
  int result = operand[0];
  switch (operator) {
    case '+':
      for (int i = 1; i < opnum; ++i) result += operand[i];
      break;
    case '-':
      for (int i = 1; i < opnum; ++i) result -= operand[i];
      break;
    case '*':
      for (int i = 1; i < opnum; ++i) result *= operand[i];
      break;
    default:
      error_handling("illegal operator!\n");
  }
  return result;
}
