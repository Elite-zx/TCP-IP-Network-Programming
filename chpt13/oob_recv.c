#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

const int BUFSIZE = 256;
int serv_sock, clnt_sock;
void error_handling(char *message);
void urg_handling(int sig);

int main(int argc, char *argv[]) {
  struct sockaddr_in serv_adr, clnt_adr;
  socklen_t clnt_adr_sz;

  if (argc != 2) {
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }

  serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  if (serv_sock == -1) error_handling("socket() error");

  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_adr.sin_port = htons(atoi(argv[1]));

  if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
    error_handling("bind() error");

  if (listen(serv_sock, 5) == -1) error_handling("listen() error");

  clnt_adr_sz = sizeof(clnt_adr);
  clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
  if (clnt_sock == -1) error_handling("accept() error");

  /*designate which process's urg_handling function to be invoke*/
  fcntl(clnt_sock, F_SETOWN, getpid());
  struct sigaction act;
  act.sa_handler = urg_handling;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);  // set 0
  sigaction(SIGURG, &act, 0);

  int str_len;
  char buf[BUFSIZE];
  while ((str_len = recv(clnt_sock, buf, BUFSIZE - 1, 0)) != 0) {
    buf[str_len] = '\0';
    printf("normal message: %s\n", buf);
  }
  close(clnt_sock);
  close(serv_sock);
  return 0;
}

void error_handling(char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

/*handle SIGURG signal*/
void urg_handling(int sig) {
  char buf[BUFSIZE];
  int str_len = recv(clnt_sock, buf, BUFSIZE - 1, MSG_OOB);
  buf[str_len] = '\0';
  printf("urgency message: %s\n", buf);
}
