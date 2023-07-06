#include <arpa/inet.h>
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_CLNT 100
const int BUFSIZE = 256;

int clnt_cnt = 0;
int clnt_socks_arr[MAX_CLNT];
pthread_mutex_t mutex;
void error_handling(const char* message);
void send_msg_to_clnt(char* msg, int len);
void* handle_clnt(void* arg);

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
  puts("Server start!");

  /*thread*/
  pthread_t t_id;
  /*mutex*/
  pthread_mutex_init(&mutex, NULL);

  struct sockaddr_in clnt_addr;
  socklen_t clnt_addr_size = sizeof(clnt_addr);
  while (1) {
    int clnt_sock =
        accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    pthread_mutex_lock(&mutex);
    clnt_socks_arr[clnt_cnt++] = clnt_sock;
    pthread_mutex_unlock(&mutex);

    pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
    pthread_detach(t_id);
    printf("Connected client IP: %s \n", inet_ntoa(clnt_addr.sin_addr));
  }
  close(serv_sock);
  return 0;
}

void* handle_clnt(void* arg) {
  int clnt_sock = *((int*)arg);
  char msg[BUFSIZE];
  int str_len;

  while ((str_len = read(clnt_sock, msg, BUFSIZE)) != 0)
    send_msg_to_clnt(msg, str_len);

  // remove current connection from server
  pthread_mutex_lock(&mutex);
  for (int i; i < clnt_cnt; ++i) {
    if (clnt_socks_arr[i] == clnt_sock) {
      while (i++ < clnt_cnt - 1)  // tilt array ,remove current client
        clnt_socks_arr[i] = clnt_socks_arr[i + 1];
      break;
    }
  }
  --clnt_cnt;
  pthread_mutex_unlock(&mutex);
  close(clnt_sock);
  return NULL;
}

/*send msg to all client*/
void send_msg_to_clnt(char* msg, int len) {
  pthread_mutex_lock(&mutex);
  for (int i = 0; i < clnt_cnt; ++i) write(clnt_socks_arr[i], msg, len);
  pthread_mutex_unlock(&mutex);
}

void error_handling(const char* message) {
  fputs(message, stderr);
  exit(1);
}
