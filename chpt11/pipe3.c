#include <stdio.h>
#include <unistd.h>

const int BUFSIZE = 36;

int main(int argc, char* argv[]) {
  char str1[] = "Hello parent! I am child!";
  char str2[] = "Hello child! I am parent!";
  char buf[BUFSIZE];
  int pipe_handle1[2], pipe_handle2[2];
  pipe(pipe_handle1);
  pipe(pipe_handle2);

  pid_t pid = fork();
  if (!pid) {
    write(pipe_handle1[1], str1, sizeof(str1));
    int str_len = read(pipe_handle2[0], buf, BUFSIZE - 1);
    buf[str_len] = '\0';
    printf("Child: message from parent --- %s\n", buf);
  } else {
    write(pipe_handle2[1], str2, sizeof(str2));
    int str_len = read(pipe_handle1[0], buf, BUFSIZE - 1);
    buf[str_len] = '\0';
    printf("Parent: message from child --- %s\n", buf);
  }
  return 0;
}
