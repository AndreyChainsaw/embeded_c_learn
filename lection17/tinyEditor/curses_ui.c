#include <termios.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <curses.h>
#include <stdio.h>
#include "curses_ui.h"
#include "ioworker.h"

#define MAX_LEN 21
#define BLOCK_SIZE 4096

void getScreenSize(struct winsize *size) {
  ioctl(fileno(stdout), TIOCGWINSZ, (char*) size);
}

void sig_winch(int signo) {
  struct winsize size;
  getScreenSize(&size);
  resizeterm(size.ws_row, size.ws_col);
}


//TODO если x вылез за рамки, то y менять
// x < 0, тогда y-1, если x > max, то y+1

/* Обработка спец-символов: стрелки, backspace, F(n)*/
void specialKeys(int ch, WINDOW *wnd, WINDOW *wndPanel, int fd) {
  int x,y;
  getyx(wnd, y, x);
  switch (ch) {
    case KEY_UP:
      y--;
      break;
    case KEY_DOWN:
      y++;
      break;
    case KEY_RIGHT:
      x++;
      break;
    case KEY_LEFT:
      x--;
      break;
    case KEY_BACKSPACE:
      mvwaddch(wnd, y, x, ' ');
      break;
    case KEY_F(1):
      delwin(wndPanel);
      delwin(wnd);

      endwin();
      exit(EXIT_SUCCESS);
    case KEY_F(2):
      saveFile(wnd, fd, BLOCK_SIZE);
    default:
      break;
      
  }
  wmove(wnd, y, x);
  wrefresh(wnd);
}


/* Инициализация экрана, окна библиотеки curses.
   Ввод имени файла для редактирования */
WINDOW* initFirstScr(struct winsize *sizeScr, char *fileName) {
  WINDOW *textWnd = malloc(sizeof(textWnd));

  initscr();
  getScreenSize(sizeScr);

  cbreak();
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLUE);


  textWnd = newwin(sizeScr->ws_row - 1, sizeScr->ws_col - 1, 0, 0);
  wmove(textWnd, sizeScr->ws_row / 2, 3);
  wprintw(textWnd, "Print file name to edit: ");
  refresh();

  wgetnstr(textWnd, fileName, MAX_LEN);

  return textWnd;
}

/* Открытие окна текстового редактора c содержимым файла*/
WINDOW* initEditScr(struct winsize *sizeScr, WINDOW *textWnd, char *fileText) {

  WINDOW *wndPanel = malloc(sizeof(wndPanel));

  wclear(textWnd);
  wprintw(textWnd, fileText);
  wrefresh(textWnd);

  wndPanel = newwin(1, sizeScr->ws_col, 0, 0);
  wattron(wndPanel, COLOR_PAIR(1));
  wbkgd(wndPanel, COLOR_PAIR(1));
  mvwin(wndPanel, sizeScr->ws_row - 1, 0);
  wprintw(wndPanel, "F1-Close File\tF2-Save File");
  wrefresh(wndPanel);

  wmove(textWnd, 0, 0);

  return wndPanel;
}