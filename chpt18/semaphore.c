#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

void* _read(void* arg);
void* accumulate(void* arg);
int num = 0;
int sum = 0;
static sem_t sem_empty;
static sem_t sem_full;
int main(int argc, char* argv[]) {
  sem_init(&sem_empty, 0, 1);
  sem_init(&sem_full, 0, 0);

  pthread_t th_id1, th_id2;

  pthread_create(&th_id1, NULL, _read, NULL);
  pthread_create(&th_id2, NULL, accumulate, NULL);

  pthread_join(th_id1, NULL);
  pthread_join(th_id2, NULL);

  puts("worker thread model under semaphore mechanism!");
  printf("result: %d\n", sum);

  sem_destroy(&sem_empty);
  sem_destroy(&sem_full);
  return 0;
}

void* _read(void* arg) {
  for (int i = 0; i < 6; ++i) {
    fputs("Input num: ", stdout);
    sem_wait(&sem_empty);
    scanf("%d", &num);
    sem_post(&sem_full);
  }
  return NULL;
}

void* accumulate(void* arg) {
  for (int i = 0; i < 6; ++i) {
    sem_wait(&sem_full);
    sum += num;
    sem_post(&sem_empty);
  }
  return NULL;
}
