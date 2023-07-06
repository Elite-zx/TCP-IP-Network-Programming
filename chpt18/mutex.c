#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* thread_des(void* msg);
void* thread_inc(void* msg);
const int num_thread = 100;
const int max_inc_des = 100;

long long num = 0;
pthread_mutex_t mutex;

int main(int argc, char* argv[]) {
  pthread_t thread_id[num_thread];
  /* initialize a mutex*/
  pthread_mutex_init(&mutex, NULL);

  for (int i = 0; i < num_thread; ++i) {
    if (i % 2)
      pthread_create(&thread_id[i], NULL, thread_inc, NULL);
    else
      pthread_create(&thread_id[i], NULL, thread_des, NULL);
  }
  for (int i = 0; i < num_thread; ++i) pthread_join(thread_id[i], NULL);

  puts("Caculate sum by using worker thread model!");
  printf("result: %lld\n", num);
  pthread_mutex_destroy(&mutex);
  return 0;
}

void* thread_inc(void* msg) {
  /*keep critical section*/
  /*thread synchronization*/
  pthread_mutex_lock(&mutex);
  for (int i = 0; i < max_inc_des; ++i) num += i;
  pthread_mutex_unlock(&mutex);
  return NULL;
}

void* thread_des(void* msg) {
  for (int i = 0; i < max_inc_des; ++i) {
    pthread_mutex_lock(&mutex);
    num -= i;
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}
