#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 256
#define NAME_SIZE 256
void error_handling(char *message);
void write_routine(int sock, char *message);
void read_routine(int sock, char *message);
void *send_msg(void *msg);
void *recv_msg(void *msg);
char msg[BUF_SIZE];
char name[NAME_SIZE] = "[DEFAULT]";

int main(int argc, char *argv[]) {
  int sock;
  struct sockaddr_in serv_adr;

  if (argc != 4) {
    printf("Usage : %s <IP> <port> <name>\n", argv[0]);
    exit(1);
  }

  sprintf(name, "[%s]", argv[3]);
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

  pthread_t snd_thread_id;
  pthread_t rsv_thread_id;

  pthread_create(&snd_thread_id, NULL, send_msg, &sock);
  pthread_create(&rsv_thread_id, NULL, recv_msg, &sock);
  void *thread_ret;
  pthread_join(snd_thread_id, &thread_ret);
  pthread_join(rsv_thread_id, &thread_ret);
  return 0;
}

void *send_msg(void *arg) {
  int sock = *((int *)arg);
  char msg_with_name[BUF_SIZE + NAME_SIZE + 1];
  while (1) {
    fgets(msg, BUF_SIZE, stdin);
    if (!strcmp(msg, "q\n") || !strcmp(msg, "q\n")) {
      close(sock);
      exit(1);
    }
    sprintf(msg_with_name, "%s: %s", name, msg);
    write(sock, msg_with_name, strlen(msg_with_name));
  }
  return NULL;
}

void *recv_msg(void *arg) {
  int sock = *((int *)arg);
  char msg_with_name[BUF_SIZE + NAME_SIZE + 1];
  while (1) {  // keep reading
    int str_len = read(sock, msg_with_name, NAME_SIZE + BUF_SIZE - 1);
    if (str_len == -1) error_handling("read() error!");
    msg_with_name[str_len] = '\0';
    fputs(msg_with_name, stdout);
  }
  return NULL;
}

void error_handling(char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

