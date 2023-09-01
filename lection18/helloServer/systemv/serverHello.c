#include <sys/stat.h>
#include <stdio.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <string.h>

#define KEY_FILE "./forFTOK"
#define TEXT_LEN 20

/* Инициализируем тело сообщения:
    msg.mtype = 1 - от сервера клиенту
    msg.mtype > 1 - от клиента серверу
*/

#define SERVER_MESS 1
#define CLIENT_MESS 2

/* Структура сообщения */
struct mbuf {
  long mtype;            /* Обязательное поле. Это приоритет */
  char mtext[TEXT_LEN];  /* Текст сообщения */
};

int main(void) {
  struct mbuf msg;
  char msgText[TEXT_LEN] = "hello client!";

  key_t keyQueue = ftok(KEY_FILE, 1);

  int msgid = msgget(keyQueue, IPC_CREAT | S_IRUSR | S_IWUSR);

  if (-1 == msgid) {
    perror("Error. Can`t get msgid");
    exit(EXIT_FAILURE);
  }

  msg.mtype = SERVER_MESS;
  memcpy(msg.mtext, msgText, TEXT_LEN);

  /* Отправляем сообщение в очередь для клиента */
  msgsnd(msgid, &msg, TEXT_LEN, 0);
  printf("Server was sent message. Wait reply...\n");

  /* Ждем сообщение от клиента. Блокирующий системный вызов */

  if (msgrcv(msgid, &msg, TEXT_LEN, CLIENT_MESS, 0) == -1) {
    perror("msgrcv. Can`t read from queue");
    exit(EXIT_FAILURE);
  }
  printf("\n[Reply from client]: %s\n", msg.mtext);


  return 0;
}