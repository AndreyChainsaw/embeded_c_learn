// TODO: Не все функции нужно выносить наружу
void getScreenSize(struct winsize *size);

void sig_winch(int signo);

void specialKeys(int ch, WINDOW *wnd, WINDOW *wndPanel, int fd);

WINDOW* initFirstScr(struct winsize *sizeScr, char *fileName);

WINDOW* initEditScr(struct winsize *sizeScr, WINDOW *textWnd, char *fileText);