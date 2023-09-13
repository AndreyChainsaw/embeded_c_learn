#include <stdio.h>
#include <signal.h>

void handlerUSR1(int sig) {
  printf("Process get signal SIGUSR1\n");
}


int main(void) {
  struct sigaction act = { 0 };

  act.sa_handler = handlerUSR1;
  act.sa_flags   = SA_RESTART;

  sigaction(SIGUSR1, &act, NULL);
  int trash = 0;
  scanf("%d", &trash);
  return 0;
}