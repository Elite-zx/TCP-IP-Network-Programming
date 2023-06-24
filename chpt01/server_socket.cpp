int main(int argc, char* agrv[]) {
  int serv_sock;
  struct sockaddr_in serv_addr;
  char* serv_port = "9190";

  /*Create a server-side socket (listening socket)*/
  serv_sock = socket(PF_INET, SOCK_STREAM, 0);

  /*Address information initialization*/
  memset(serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(serv_port));
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  /*allocate Address information for server*/
  bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
}

