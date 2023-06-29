#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

const int BUFSIZE = 256;
void error_handling(const char* message);
void childproc_handler(int sig);

int main(int argc, char* argv[]) {
  /* allocate socket*/
  int serv_sock = socket(PF_INET, SOCK_STREAM, 0);

  /*remove time_wait state*/
  int optVal = 1;
  socklen_t optLen = sizeof(optVal);
  setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &optVal, optLen);

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

  /* avoid zombie process*/
  struct sigaction act;
  act.sa_handler = childproc_handler;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);  // set 0
  sigaction(SIGCHLD, &act, 0);

  char buf[BUFSIZE];
  /*I/O Multiplexing*/
  fd_set _read, cp_read;
  FD_ZERO(&_read);
  FD_SET(serv_sock, &_read);
  int fd_max = serv_sock;
  struct timeval timeout;

  /* single server process, no fork */
  while (1) {
    cp_read = _read;
    timeout.tv_sec = 5;
    timeout.tv_usec = 500000;  // 5.5s
    int ret = select(fd_max + 1, &cp_read, 0, 0, &timeout);
    if (ret == -1) error_handling("select() error!");
    if (!ret)  // timeout
      continue;
    else {
      /* polling */
      for (int i = 0; i < fd_max + 1; ++i) {
        if (FD_ISSET(i, &cp_read)) {
          if (i == serv_sock)  //  Welcoming socket
          {
            /*handle request: create another socket and connect to client */
            struct sockaddr_in clnt_addr;
            socklen_t clnt_addr_size = sizeof(clnt_addr);
            int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr,
                                   &clnt_addr_size);
            if (clnt_sock == -1) continue;
            FD_SET(clnt_sock, &_read);  // add new file descripter to listen
            fd_max = fd_max < clnt_sock ? clnt_sock : fd_max;
            printf("new client %d connected!\n", clnt_sock);
          } else {
            int str_len = read(i, buf, BUFSIZE);
            if (str_len == 0)  // EOF
            {
              FD_CLR(i, &_read);
              close(i);
              printf("close client %d!\n", i);
            } else {
              write(i, buf, str_len);
            }
          }
        }
      }
    }
  }
  close(serv_sock);
  return 0;
}

void childproc_handler(int sig) {
  int status;
  pid_t pid = waitpid(-1, &status, WNOHANG);
  if (WIFEXITED(status)) {
    printf("Remove child process pid %d\n", WEXITSTATUS(status));
  }
}
void error_handling(const char* message) {
  fputs(message, stderr);
  exit(1);
}
