#include <arpa/inet.h>
#include <stdio.h>
int main(int argc, char *argv[]) {
  unsigned short host_port = 0x1234;
  unsigned short net_port;
  unsigned long host_addr = 0x12345678;
  unsigned long net_addr;

  net_port = htons(host_port);  //转换为网络字节序
  net_addr = htonl(host_addr);

  // %#x中的#x表示以16进制输出数据并在开头加上0x,同理还有8进制的%#o
  printf("Host ordered port: %#x \n", host_port);
  printf("Network ordered port: %#x \n", net_port);
  printf("Host ordered address: %#lx \n", host_addr);
  printf("Network ordered address: %#lx \n", net_addr);

  return 0;
}
