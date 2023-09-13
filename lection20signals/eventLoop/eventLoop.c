#include <stdio.h>
#include <signal.h>

int main(void) {
  sigset_t blockedSignals;
  /* Очищаем структуру (маску) */
  sigemptyset(&blockedSignals);

  /* Ставим в структуру бит сигнала SIGUSR1 */
  sigaddset(&blockedSignals, SIGUSR1);

  /* Блокируем сигналы, указанные в blockedSignals */
  sigprocmask(SIG_BLOCK, &blockedSignals, NULL);

  int sig;

  while(1){
    /* блокируем поток. Ждем сигнала, указанного в маске */
    sigwait(&blockedSignals, &sig);
    printf("Get %d signal\n", sig);
    printf("hello world!\n");
  }
  
}