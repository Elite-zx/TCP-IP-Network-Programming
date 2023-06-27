#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
void error_handling(const char* message);

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage:%s <IP address> \n", argv[0]);
    exit(1);
  }
  struct in_addr addr;
  addr.s_addr = inet_addr(argv[1]);
  struct hostent* host_info = gethostbyaddr((char*)&addr, 4, AF_INET);
  if (host_info == NULL) error_handling("gethostbyaddr() error!\n");
  printf("official name: %s \n", host_info->h_name);
  printf("alias list: \n");
  for (int i = 0; host_info->h_aliases[i]; ++i)
    printf("%d: %s \n", i + 1, host_info->h_aliases[i]);
  printf("host_info address type: %s \n",
         (host_info->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");
  printf("address_length: %d \n", host_info->h_length);
  /* Note type conversion */
  for (int i = 0; host_info->h_addr_list[i]; ++i)
    printf("IP address list: %d---%s \n", i + 1,
           inet_ntoa(*((struct in_addr*)host_info->h_addr_list[i])));
  return 0;
}

void error_handling(const char* message) {
  fputs(message, stderr);
  exit(1);
}

