#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void timeout(int sig) {
  if (sig == SIGALRM) puts("Time out !");
  alarm(2);  // after 2s, event will toggle
}
int main(int argc, char* argv[]) {
  struct sigaction act;
  act.sa_handler = timeout;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);  // set 0
  sigaction(SIGALRM, &act, 0);
  alarm(2);
  for (int i = 0; i < 3; ++i) {
    puts("wait...");
    // waiting for time out, in fact, running time will be 6 seconds instead of
    // 3*300=900 seconds
    sleep(300);
  }
}
