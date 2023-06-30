
#include <stdio.h>
#include <time.h>

const int BUFSIZE = 36;

int main(int agrc, char* argv[]) {
  clock_t start_time = clock();
  FILE* fp1 = fopen("IO_test.txt", "r");
  FILE* fp2 = fopen("cpy2.txt", "w");

  int len = 0;
  char buf[BUFSIZE];
  while (fgets(buf, BUFSIZE, fp1) != NULL) {
    fputs(buf, fp2);
  }

  fclose(fp1);
  fclose(fp2);
  clock_t end_time = clock();
  printf("total running time: %f", (double)end_time - start_time);
  return 0;
}
