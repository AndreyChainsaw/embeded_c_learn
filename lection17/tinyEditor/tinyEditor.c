#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <curses.h>
#include "curses_ui.h"
#include "ioworker.h"

#define MAX_LEN 21
#define BLOCK_SIZE 4096


int main(int argc, char *argv[]) {
  
  int fd;
  WINDOW *wnd;
  WINDOW *wndPanel;
  struct winsize sizeScreen;

  // Контроль за переполнением делает curses
  char fileName[MAX_LEN];
  
  signal(SIGWINCH, sig_winch);
  wnd = initFirstScr(&sizeScreen, fileName);
  
  char *buff = getContentFile(fileName, &fd, BLOCK_SIZE);
  wndPanel = initEditScr(&sizeScreen, wnd, buff);

  /* Очищаем содержимое буфера. Новые данные
     Возьмем из буфера curses */
  free(buff);
  
  keypad(wnd, 1);
  int ch;

  while (1) {
    ch = wgetch(wnd);

    if (ch > 255) {
      specialKeys(ch, wnd, wndPanel, fd);
    }
  }
}
