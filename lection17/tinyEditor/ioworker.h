/* Сохранение содержимого буффера curses в файл */
void saveFile(WINDOW *textWnd, int fd, size_t count);

/* Открытие указанного файла, и чтение содержимого.
   Модифицирует файловый дескриптор и отдает буффер
   с содержимым файла */

char* getContentFile(char* fileName, int *fd, size_t count);