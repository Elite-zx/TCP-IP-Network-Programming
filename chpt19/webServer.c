#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFSIZE 256
#define LITTLE_BUF 100

void error_handling(char* msg);
void* request_handler(void* arg);
void send_data(FILE* fp, char* cont_type, char* file_name);
void send_error(FILE* fp);
char* Get_Content_Type(char* path_to_file);

int main(int argc, char* argv[]) {
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
  listen(serv_sock, 20);
  puts("WebServer start!");

  struct sockaddr_in clnt_addr;
  socklen_t clnt_addr_size = sizeof(clnt_addr);
  pthread_t t_id;
  int cnt = 0;
  while (1) {
    int clnt_sock =
        accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    printf("Connection request from client %d\n", cnt++);
    /*create a thread handle a connection*/
    pthread_create(&t_id, NULL, request_handler, &clnt_sock);
    pthread_detach(t_id);
  }
  close(serv_sock);
  return 0;
}

void* request_handler(void* arg) {
  int clnt_sock = *((int*)arg);

  /*I/O division*/
  FILE* write_fp = fdopen(clnt_sock, "w");
  FILE* read_fp = fdopen(dup(clnt_sock), "r");

  /*parse request line*/
  char req_line[LITTLE_BUF];
  char method[20];
  char file_name[20];
  char cont_type[20];

  /*get request line from request message*/
  fgets(req_line, LITTLE_BUF, read_fp);

  /*check if it is HTTP protocol*/
  if (strstr(req_line, "HTTP/") == NULL) {
    send_error(write_fp);
    fclose(write_fp);
    fclose(read_fp);
    return NULL;
  }
  /* function strtok is used to extract different part which is divided by '\'
   * in request line*/
  strcpy(method, strtok(req_line, " /"));
  strcpy(file_name, strtok(NULL, " /"));
  strcpy(cont_type, Get_Content_Type(file_name));
  /*check if it is GET method*/
  if (strcmp(method, "GET") != 0) {
    send_error(write_fp);
    fclose(write_fp);
    fclose(read_fp);
    return NULL;
  }
  fclose(read_fp);
  send_data(write_fp, cont_type, file_name);
  return NULL;
}

/*determine the value of Content-Type in HTTP message header*/
char* Get_Content_Type(char* path_to_file) {
  char extension[LITTLE_BUF];

  /* aviod path_to_file modified by function strtok*/
  char file_name[LITTLE_BUF];
  strcpy(file_name, path_to_file);

  /*get file extension which represent content type*/
  strtok(file_name, ".");
  strcpy(extension, strtok(NULL, "."));
  if (!strcmp(extension, "html") || !strcmp(extension, "htm")) {
    return "text/html";
  } else
    return "text/plain";
}

void send_error(FILE* fp) {
  /*status line and header*/
  char protocol[] = "HTTP/1.1 400 Bad Request\r\n";
  char date[] = "Date: Fri, 7 July 2023 2:12:09 GMT\r\n";
  char server[] = "Server: Linux Web server\r\n";
  char cntLen[] = "Content-Length: 1024\r\n";
  char cntType[] = "Content-Type: text/html\r\n\r\n";

  /*entity body*/
  char content[] =
      "<html><head><title>NETWORK</title></head>"
      "<body><font size=+5><br>Error occured! Please check filename or request "
      "method!"
      "</font></body></html>";

  fputs(protocol, fp);
  fputs(date, fp);
  fputs(server, fp);
  fputs(cntLen, fp);
  fputs(cntType, fp);
  fputs(content, fp);
  fflush(fp);
}

void send_data(FILE* fp, char* cont_type, char* file_name) {
  /*define request line and header*/
  char protocol[] = "HTTP/1.1 200 OK\r\n";
  char server[] = "Server: Linux Web server\r\n";
  char cntLen[] = "Content-Length: 1048\r\n";
  char cntType[LITTLE_BUF];
  sprintf(cntType, "Content-Type: %s\r\n\r\n", cont_type);

  /*open target file*/
  FILE* target_file = fopen(file_name, "r");
  if (target_file == NULL) {
    send_error(fp);
    return;
  }
  /*send request line and header*/
  fputs(protocol, fp);
  fputs(server, fp);
  fputs(cntLen, fp);
  fputs(cntType, fp);
  /*send entity body*/
  char buf[BUFSIZE];
  while (fgets(buf, BUFSIZE, target_file) != NULL) {
    fputs(buf, fp);
    fflush(fp);
  }
  fflush(fp);
  fclose(target_file);
  fclose(fp);
}

void error_handling(char* msg) {
  fputs(msg, stderr);
  exit(1);
}
