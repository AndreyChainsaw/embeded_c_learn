#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <stdio.h>

#include <curses.h>
#include <termios.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <stdlib.h>

#include "hash.h"

#define SERVER_MESS 0
#define MAX_MSG_SIZE 100
#define MAX_LEN_NAME 10

struct mbuf {
  long mtype;
  char username[MAX_LEN_NAME];
  char mtext[MAX_MSG_SIZE];
};

void getScreenSize(struct winsize *size) {
  ioctl(fileno(stdout), TIOCGWINSZ, (char*) size);
}

void sig_winch(int signo) {
  struct winsize size;
  getScreenSize(&size);
  resizeterm(size.ws_row, size.ws_col);
}


int main(int argc, char *argv[]) {

  // /* Работа с окнами */
  // WINDOW *wndChat;
  // WINDOW *wndTextField;
  // char username[10];
    
  // struct winsize sizeScreen;

  // signal(SIGWINCH, sig_winch);

  // initscr();
  // getScreenSize(&sizeScreen);
  // cbreak();
  // start_color();

  // wndChat = newwin(sizeScreen.ws_row - 1, sizeScreen.ws_col - 1, 0, 0);
  // wprintw(wndChat, "Enter your login: ");
  // refresh();

  // wgetnstr(wndChat, username, 10);
  // wclear(wndChat);

  // long hashedUsername = hashString(username);
  // wprintw(wndChat, username);
  // wrefresh(wndChat);

  // wndTextField = newwin(2, sizeScreen.ws_col, 0, 0);
  // mvwin(wndTextField, sizeScreen.ws_row - 2, 0);
  // wrefresh(wndTextField);

  /* Вот эту задачу в отдельный поток */
  // while (1) {
  //   wgetch(wndTextField);
  //   wrefresh(wndTextField);

  // }



  /* работа с очередью */
  struct mbuf message = {
    .mtype = hashString("andrey"),
    .username = "andrey",
    .mtext = "hello everybody!"
  };

  key_t keyQueue = ftok("./server", 1);
  int msgid = msgget(keyQueue, 0);

  if (-1 == msgid) {
    perror("msgget error");
    exit(0);
  }

  // не уверен, что sizeof - хорошая идея, там ведь учитывается long....
  msgsnd(msgid, &message, sizeof(message), 0);





  // delwin(wndTextField);
  // delwin(wndMain);

  // endwin();
  return 0;
}