/* для вызова потока inputTextModify */
struct dataPthreadTextModify {
  WINDOW *wnd;
  int msgid;
  struct mbuf *message;
};

/* для вызова потока getMesssage */
struct dataPthreadGetMessage {
  WINDOW *wnd;
  long hashedUsername;
  int msgid;
};

void getScreenSize(struct winsize *size);

void sig_winch(int signo);

void *sendMessage(char *messageText, struct mbuf *message, int msgid);

void *inputTextModify(void *vars);

void *getMessage(void *vars);

