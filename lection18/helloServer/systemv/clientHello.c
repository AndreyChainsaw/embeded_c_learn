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
  key_t keyQueue = ftok(KEY_FILE, 1);
  int msgid = msgget(keyQueue, 0);

  if (-1 == msgid) {
    perror("Error. Can`t get msgid");
    exit(EXIT_FAILURE);
  }

  /* Получаем сообщения от сервера */
  if (msgrcv(msgid, &msg, TEXT_LEN, SERVER_MESS, 0) == -1){
    perror("msgrcv. Can`t read from queue");
    exit(EXIT_FAILURE);
  }

  printf("[Reply from server]: %s\n", msg.mtext);

  /* Отправляем сообщение серверу */
  char replyServer[TEXT_LEN] = "hi, server!";

  msg.mtype = CLIENT_MESS;
  memcpy(msg.mtext, replyServer, TEXT_LEN);

  if (msgsnd(msgid, &msg, TEXT_LEN, 0) == -1) {
    perror("msgsnd. Can`t reply to server");
    exit(EXIT_FAILURE);
  }

  printf("Client was sent message...\n");

  return 0;

}