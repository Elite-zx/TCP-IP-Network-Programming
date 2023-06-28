#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
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

  /*handle request: create another socket and connect to client */
  struct sockaddr_in clnt_addr;
  // function accept need a LValue
  socklen_t clnt_addr_size = sizeof(clnt_addr);
  char buf[BUFSIZE];
  int pipe_handle[2];

  pipe(pipe_handle);

  pid_t pid = fork();

  /*child process which write content from client to txt file*/
  if (!pid) {
    FILE* pf = fopen("echomsg.txt", "wt");
    if (pf == NULL) error_handling("fopen() error!\n");
    close(serv_sock);
    for (int i = 0; i < 3; ++i) {
      int len = read(pipe_handle[0], buf, BUFSIZE);
      printf("i can write! len : %d\n", len);
      int num_written = fwrite((void*)buf, 1, len, pf);
      if (num_written < len && ferror(pf)) {
        perror("fwrite failed");
      }
    }
    fclose(pf);
    return 0;
  }

  while (1) {
    int clnt_sock =
        accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1)
      continue;
    else
      puts("new client connected!");
    pid_t pid = fork();
    if (pid == -1) {
      close(clnt_sock);
      continue;
    }
    if (!pid) {
      close(serv_sock);
      int str_len = 0;
      while ((str_len = read(clnt_sock, buf, BUFSIZE)) != 0) {
        write(clnt_sock, buf, str_len);
        write(pipe_handle[1], buf, str_len);
      }
      close(clnt_sock);
      puts("client disconnected!");
      return 0;
    } else {
      close(clnt_sock);
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
