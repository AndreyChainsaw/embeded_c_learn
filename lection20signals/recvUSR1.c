#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("Usage error:\n \t%s <PID_PROCESS>\n", argv[0]);
    exit(1);
  }

  /*convert string to unsigned long long (8 bytes) */
  unsigned long long pidProcess = strtoll(argv[1], NULL, 10);
  
  if(kill(pidProcess, SIGUSR1) == -1) {
    perror("erorr kill command");
    exit(1);
  }

  return 0;

  
}