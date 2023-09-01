#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include <curses.h>
#include <termios.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <stdlib.h>

#include "client.h"
#include "hash.h"

#define SERVER_MESS 1
#define MAX_MSG_SIZE 100
#define MAX_LEN_NAME 10

struct mbuf {
  long mtype;
  char username[MAX_LEN_NAME];
  char mtext[MAX_MSG_SIZE];
};

void initScreensCurses(WINDOW *wndChat, WINDOW *wndTextField) {
  
}

int main(int argc, char *argv[]) {

  pthread_t threadTextModify;
  pthread_t threadGetMessage;

  /* Работа с окнами */
  WINDOW *wndChat;
  WINDOW *wndTextField;
  char username[10];
    
  struct winsize sizeScreen;

  signal(SIGWINCH, sig_winch);

  initscr();
  getScreenSize(&sizeScreen);
  cbreak();
  start_color();

  wndChat = newwin(sizeScreen.ws_row - 1, sizeScreen.ws_col - 1, 0, 0);
  wprintw(wndChat, "Enter your login: ");
  refresh();

  wgetnstr(wndChat, username, 10);
  wclear(wndChat);
  wrefresh(wndChat);

  wndTextField = newwin(2, sizeScreen.ws_col, 0, 0);
  mvwin(wndTextField, sizeScreen.ws_row - 2, 0);
  wrefresh(wndTextField);

  /* работа с очередью */
  struct mbuf message = {
    .mtype = SERVER_MESS,
    .mtext = "hello everybody!"
  };

  memcpy(message.username, username, 10);

  key_t keyQueue = ftok("./server", 1);
  int msgid = msgget(keyQueue, 0);

  if (-1 == msgid) {
    perror("msgget error");
    exit(0);
  }


  /* Отправляем "hello" сообщение серверу, чтобы он о нас узнал */
  // не уверен, что sizeof - хорошая идея, там ведь учитывается long....
  msgsnd(msgid, &message, sizeof(message), 0);

  /* создаем отдельный поток под получение сообщений */
  struct dataPthreadGetMessage getMsg = {
    .wnd = wndChat,
    .hashedUsername = hashString(message.username),
    .msgid = msgid,
  };

  /* создаем отдельный поток под редактирование поля для текста */
  /* Для этого создадим дополнительную структуру */
  struct dataPthreadTextModify textmodify = {
    .wnd = wndTextField,
    .msgid = msgid,
    .message = &message,
  };

  pthread_create(&threadTextModify, NULL, inputTextModify, (void*)&textmodify);
  pthread_create(&threadGetMessage, NULL, getMessage, (void*)&getMsg);

  pthread_join(threadTextModify, NULL);
  pthread_join(threadGetMessage, NULL);

  delwin(wndTextField);
  delwin(wndChat);

  endwin();


  return 0;
}

void getScreenSize(struct winsize *size) {
  ioctl(fileno(stdout), TIOCGWINSZ, (char*) size);
}

void sig_winch(int signo) {
  struct winsize size;
  getScreenSize(&size);
  resizeterm(size.ws_row, size.ws_col);
}

void *sendMessage(char *messageText, struct mbuf *message, int msgid) {
  memcpy(message->mtext, messageText, MAX_MSG_SIZE);
  msgsnd(msgid, message, sizeof(*message), 0);
}

/* запускается в отдельном потоке */
void *inputTextModify(void *vars) {
  struct dataPthreadTextModify *data = (struct dataPthreadTextModify*)(vars);

  WINDOW *wndTextField = data->wnd;
  int msgid = data->msgid;
  struct mbuf *message = data->message;

  keypad(wndTextField, 1);

  while(1) {
    char symb = (char)wgetch(wndTextField);
    if(symb == '\n') {
      wmove(wndTextField, 0, 0);

      char messageText[MAX_MSG_SIZE];
      int readedBytes = winnstr(wndTextField, messageText, MAX_MSG_SIZE);

      /* Делаем сообщение NULL терминированным */
      for (int i = readedBytes-1; messageText[i] == ' '; i--) {
        messageText[i] = 0;
      }

      sendMessage(messageText, message, msgid);
    }
    wrefresh(wndTextField);
  }
}



/* запускается в отдельном потоке */
void *getMessage(void *vars) {
  struct dataPthreadGetMessage *data = (struct dataPthreadGetMessage*)(vars);

  /* распаковываем полученные данные */
  WINDOW *wndChat = data->wnd;
  int msgid = data->msgid;
  long loginID = data->hashedUsername;

  /* Читаем наши сообщения */
  while (1) {
    struct mbuf newMessage;
    msgrcv(msgid, &newMessage, sizeof(newMessage), loginID, 0);

    /* Выводим полученное сообщение из очереди на экран */
    char formattedMessage[MAX_MSG_SIZE];

    
    snprintf(formattedMessage, MAX_MSG_SIZE,"[%s] %s\n",
              newMessage.username, newMessage.mtext);
    wprintw(wndChat, formattedMessage);
    wrefresh(wndChat);
  }

}
