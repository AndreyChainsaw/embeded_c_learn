#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <curses.h>
#include <stdio.h>


/* Сохранение содержимого буффера curses в файл */
void saveFile(WINDOW *textWnd, int fd, size_t count) {
  
  wmove(textWnd, 0, 0);
  
  if(lseek(fd, 0, SEEK_SET) == -1) {
    perror("Ошибка при смещении позиции файла!");
  }
  
  char *buff = malloc(count);

  if (NULL == buff) {
    perror("Ошибка выделения памяти для сохранения файла!");
  }

  winnstr(textWnd, buff, count);

  if(write(fd, buff, count) == -1) {
    perror("Ошибка записи!");
  }

  free(buff);
}

/* Открытие указанного файла, и чтение содержимого.
   Модифицирует файловый дескриптор и отдает буффер
   с содержимым файла */

char* getContentFile(char* fileName, int *fd, size_t count) {
  *fd = open(fileName, O_RDWR);

  //TODO сделать проверку, что если файла нет? Как сообщить пользователю?
  if (-1 == *fd) {
    perror("Ошибка открытия файла!");
  }

  char *buff = malloc(count);

  if (NULL == buff) {
    perror("Ошибка выделения буффера под чтение файла!");
  }

  if (read(*fd, buff, count) == -1) {
    perror("Ошибка чтения файла!");
  }

  return buff;
}
