#include <sys/ipc.h> /* ftok */
#include <fcntl.h>
#include <sys/msg.h>
#include <stdio.h>  /* print errors */
#include <stdlib.h> /* exit function */
#include <string.h>
#include <pthread.h>
#include "hash.h" /* функция для хэширования имени пользователя */
#include "listClients.h"

#define SERVER_MESS 1
#define MAX_MSG_SIZE 100
#define MAX_LEN_NAME 10

struct mbuf {
  long mtype;
  char username[MAX_LEN_NAME];
  char mtext[MAX_MSG_SIZE];
};


int initMessageQueue(char *key) {
  key_t keyQueue = ftok(key, 1);

  if (-1 == keyQueue) {
    perror("ftok failed!");
    exit(EXIT_FAILURE);
  }

  int msgid = msgget(keyQueue, IPC_CREAT | S_IRUSR | S_IWUSR);

  if (-1 == msgid) {
    perror("Error. Can`t get msgid!");
    exit(EXIT_FAILURE);
  }

  return msgid;
}

//INFO для разработки можно пока оставить эту функцию
void destroyMessageQueue(int msgid) {
  if (-1 == msgctl(msgid, IPC_RMID, NULL)) {
    perror("rm queue failed!");
  } 
}

void forwardMessage(struct clientsList *connUsers, int msgid) {

  struct mbuf message;

  while (1) {
    /* Ждем сообщения от клиентов */
    
    msgrcv(msgid, &message, sizeof(message), SERVER_MESS, 0);
    printf("Server apply message from %s: [%s]\n", message.username, message.mtext);

    /* Проверяем клиента на наличие его в списке подключений
       Если его там нет, то добавляем в список
    */

    struct clientsList *current = connUsers;

    if (isInList(current, message.username) == 0) {
      push(current, message.username);
    }

    /* Рассылаем сообщения всем подключенным
       пользователям кроме отправителя */
    long hashNameSndr = hashString(message.username);

    while (current != NULL) {

      long hashNameDst = hashString(current->name);

      if (hashNameSndr != hashNameDst && hashNameDst != 0) {

        message.mtype = hashNameDst;

        //TODO сделать проверку msgsnd
        msgsnd(msgid, &message, MAX_MSG_SIZE, 0);

      }
      current = current->next;
    }

    printf("Server end sends\n");
  }

}

int main(int argc, char *argv[]) {
  struct clientsList *connUsers = initList();
  int msgid = initMessageQueue(argv[0]);
  
  forwardMessage(connUsers, msgid);

  destroyMessageQueue(msgid);
  return 0;
}