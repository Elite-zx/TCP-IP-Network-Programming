#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

const int BUFSIZE = 40;
const int EPOLL_SIZE = 24;
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

  /*epoll*/
  /*apply space for file descriptor*/
  int epoll_fd = epoll_create(EPOLL_SIZE);

  /*register file descriptor*/
  struct epoll_event register_event;
  register_event.events = EPOLLIN;
  register_event.data.fd = serv_sock;
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serv_sock, &register_event);

  /*define event to receive result*/
  struct epoll_event* result_event;
  result_event = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);
  /* start listening multiple sockets! */
  while (1) {
    int event_cnt = epoll_wait(epoll_fd, result_event, EPOLL_SIZE, -1);
    if (event_cnt == -1) error_handling("epoll_wait() error!");

    puts("return epoll_wait!");
    for (int i = 0; i < event_cnt; ++i) {
      if (result_event[i].data.fd == serv_sock) {
        struct sockaddr_in clnt_addr;
        socklen_t clnt_addr_size = sizeof(clnt_addr);
        int clnt_sock =
            accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1) continue;

        /*non block I/O*/
        int flag = fcntl(clnt_sock, F_GETFL);
        fcntl(clnt_sock, F_SETFL, flag | O_NONBLOCK);

        /* Edge Trigger*/
        register_event.events = EPOLLIN | EPOLLET;
        register_event.data.fd = clnt_sock;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clnt_sock, &register_event);
        printf("new client %d connected!\n", clnt_sock);
      } else {
        while (1) {
          int str_len = read(result_event[i].data.fd, buf, BUFSIZE);
          if (str_len == 0)  // EOF
          {
            epoll_ctl(epoll_fd, EPOLL_CTL_DEL, result_event[i].data.fd, NULL);
            close(result_event[i].data.fd);
            printf("close client %d!\n", result_event[i].data.fd);
          } else if (str_len < 0 && errno == EAGAIN) {
            break;
          } else {
            write(result_event[i].data.fd, buf, str_len);
          }
        }
      }
    }
  }
  close(serv_sock);
  close(epoll_fd);
  free(result_event);
  return 0;
};

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
