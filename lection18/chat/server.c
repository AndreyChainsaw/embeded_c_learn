#include <sys/ipc.h> /* ftok */
#include <fcntl.h>
#include <sys/msg.h>
#include <stdio.h>  /* print errors */
#include <stdlib.h> /* exit function */
#include <string.h>
#include <pthread.h>
#include "hash.h" /* функция для хэширования имени пользователя */
#include "listClients.h"

#define SERVER_MESS 0
#define MAX_MSG_SIZE 100
#define MAX_LEN_NAME 10

struct mbuf {
  long mtype;
  char username[MAX_LEN_NAME];
  char mtext[MAX_MSG_SIZE];
};

struct queueMessage {
  int msgid;
  struct clientsList *connUsers;
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



// Функция для отдельного потока
void* forwardMessage(void *packet) {
  //WARN: переделать эту функцию connUsers не возвращается назад
  struct queueMessage *messageInfo = (struct queueMessage*)(packet);
  int msgid = messageInfo->msgid;
  struct clientsList *connUsers = messageInfo->connUsers;

  struct mbuf message;

  while (1) {
    /* Ждем сообщения от клиентов */
    msgrcv(msgid, &message, sizeof(message), SERVER_MESS, 0);
    printf("Server apply message from %s: %s\n", message.username, message.mtext);

    /* Проверяем клиента на наличие его в списке подключений
       Если его там нет, то добавляем в список
    */
    if (isInList(connUsers, message.username) == 0) {
      push(connUsers, message.username);
    }

    sprintf(message.mtext, "[%s] %s\n", message.username, message.mtext);

    /* Рассылаем сообщения всем подключенным
       пользователям кроме отправителя */
    long hashNameSndr = hashString(message.username);

    while (connUsers != NULL) {

      long hashNameDst = hashString(connUsers->name);

      //WARN: С этой строкой какая-то проблема. Не срабатывает проверка на отправителя
      if (hashNameSndr != hashNameDst) {

        message.mtype = hashNameDst;

        //TODO сделать проверку msgsnd
        msgsnd(msgid, &message, MAX_MSG_SIZE, 0);

      }

      connUsers = connUsers->next;

    }

    printf("Server end sends\n");
  }

}

int main(int argc, char *argv[]) {
  struct queueMessage messageInfo;
  messageInfo.connUsers = initList();
  messageInfo.msgid = initMessageQueue(argv[0]);
  
  /* Инициализируем отдельный поток под
     прием и отправку сообщений клиентам */

  //WARN а нужно ли это?
  pthread_t threadSender;

  pthread_create(&threadSender,NULL,forwardMessage,
                (void *) &messageInfo);


  pthread_join(threadSender, NULL);





  destroyMessageQueue(messageInfo.msgid);
  return 0;
}