#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

const int BUF_SIZE = 256;
void error_handling(char *message);
void write_routine(int sock, char *message);
void read_routine(int sock, char *message);

int main(int argc, char *argv[]) {
  int sock;
  char message[BUF_SIZE];
  struct sockaddr_in serv_adr;

  if (argc != 3) {
    printf("Usage : %s <IP> <port>\n", argv[0]);
    exit(1);
  }

  sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1) error_handling("socket() error");

  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_adr.sin_port = htons(atoi(argv[2]));

  if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
    error_handling("connect() error!");
  else
    puts("Connected...........");

  pid_t pid = fork();
  if (!pid) {
    write_routine(sock, message);  // child process is responsible for write
  } else {
    read_routine(sock, message);  // parent process is responsible for read
  }

  close(sock);  // close twice
  return 0;
}

void write_routine(int sock, char *message) {
  while (1) {
    fgets(message, BUF_SIZE, stdin);
    if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
      shutdown(sock, SHUT_WR);  // pass EOF
      return;
    }
    write(sock, message, strlen(message));
  }
}

void read_routine(int sock, char *message) {
  while (1) {
    int str_len = read(sock, message, BUF_SIZE - 1);
    if (str_len == 0) return;  // message is empty which means no input
    message[str_len] = '\0';   // string end character
    printf("Message from server: %s", message);
  }
}

void error_handling(char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
