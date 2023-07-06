#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void* thread_main(void* arg);
void error_handling(const char* message);

int main(int argc, char* argv[]) {
  pthread_t thread_id;
  int thread_param = 6;
  void* thr_ret;

  /* create pthread*/
  if (pthread_create(&thread_id, NULL, thread_main, (void*)&thread_param) != 0)
    error_handling("pthread_create() error!");

  if (pthread_join(thread_id, &thr_ret) != 0)
    error_handling("pthread_join() error!\n");

  printf("Thread return message: %s", (char*)thr_ret);
  free(thr_ret);
  puts("end of process");
  return 0;
}

void* thread_main(void* arg) {
  int cnt = *((int*)arg);
  char* ret_msg = (char*)malloc(sizeof(char) * 27);
  strcpy(ret_msg, "Hello! I am thread~\n");
  for (int i = 0; i < cnt; ++i) {
    puts("thread is running");
    sleep(1);
  }
  return (void*)ret_msg;
}

void error_handling(const char* message) {
  fputs(message, stderr);
  exit(1);
}
