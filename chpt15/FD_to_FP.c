#include <fcntl.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
  int fd = open("switch.txt", O_WRONLY | O_CREAT);
  if (fd == -1) {
    fputs("open() error!", stderr);
    return -1;
  }
  FILE* fp = fdopen(fd, "w");
  fputs("switch fd to fp!", fp);
  fclose(fp);  // fd become pointless
  return 0;
}
