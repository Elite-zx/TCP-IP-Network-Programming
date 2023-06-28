#include <stdio.h>
#include <unistd.h>

const int BUFSIZE = 36;

int main(int argc, char* argv[]) {
  char str0[] = "Hello! child process!";
  char message[BUFSIZE];
  int pipe_handle[2];
  pipe(pipe_handle);

  pid_t pid = fork();
  if (!pid) {
    write(pipe_handle[1], str0, sizeof(str0));
  } else {
    read(pipe_handle[0], message, BUFSIZE);
    printf("parent get message from child : %s", message);
  }
  return 0;
}
