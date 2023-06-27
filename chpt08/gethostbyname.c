#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void error_handling(const char* message);

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage:%s <domain name> \n", argv[0]);
    exit(1);
  }
  struct hostent* host = gethostbyname(argv[1]);
  printf("official name: %s \n", host->h_name);
  printf("alias list: \n");
  for (int i = 0; host->h_aliases[i]; ++i)
    printf("%d: %s \n", i + 1, host->h_aliases[i]);
  printf("host address type: %s \n",
         (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");
  printf("address_length: %d \n", host->h_length);
  /* Note type conversion */
  for (int i = 0; host->h_addr_list[i]; ++i)
    printf("IP address list: %d---%s \n", i + 1,
           inet_ntoa(*((struct in_addr*)host->h_addr_list[i])));
  return 0;
}

void error_handling(const char* message) {
  fputs(message, stderr);
  exit(1);
}

