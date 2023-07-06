#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* thread_main(void* arg);
void error_handling(const char* message);

int main(int argc, char* argv[]) {
  pthread_t thread_id;
  int thread_param = 6;

  /* create pthread*/
  if (pthread_create(&thread_id, NULL, thread_main, (void*)&thread_param) !=
      0) {
    error_handling("pthread_create() error!");
  }

  sleep(8);
  puts("end of process");
  return 0;
}

void* thread_main(void* arg) {
  int cnt = *((int*)arg);
  for (int i = 0; i < cnt; ++i) {
    puts("thread is running");
    sleep(1);
  }
  return NULL;
}

void error_handling(const char* message) {
  fputs(message, stderr);
  exit(1);
}
