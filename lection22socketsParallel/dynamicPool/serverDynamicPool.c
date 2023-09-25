#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_WORKERS 2

pthread_attr_t workerAttr;
pthread_mutex_t acceptAllowMutex = PTHREAD_MUTEX_INITIALIZER;

int sockfd, clientfd;

void *serveClient() {
  /* Копируем fd, потом открываем mutex (разрешаем следующий accept) */
  /* Данный поток сам закроет fd */

  int cpClientFd = clientfd;
  pthread_mutex_unlock(&acceptAllowMutex);

  char command;

  if (recv(cpClientFd, &command, 1, 0) == -1) {
    perror("recv error");
    //TODO: проверить успешность закрытия
    close(cpClientFd);
    pthread_exit(0);
  }
  char datetime[30];

  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  snprintf(datetime, 30, "%d-%02d-%02d %02d:%02d:%02d\n",
          tm.tm_year + 1900, tm.tm_mon + 1,
          tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);


  if (send(cpClientFd, datetime, 30, 0) == -1) {
    perror("send error");
    close(cpClientFd);
    pthread_exit(0);
  }

  printf("[%lu]ready\n", pthread_self());
  close(cpClientFd);
  pthread_exit(0);
}


int main()
{
  
  struct sockaddr_in server, client;
  socklen_t len = sizeof(struct sockaddr_in);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd == -1) {
    perror("socket error");
    exit(1);
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(54321);

  if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) != 1) {
    perror("inet_pton error");
    exit(1);
  }

  if (bind(sockfd, (struct sockaddr*) &server, len) == -1) {
    perror("bind error");
    exit(1);
  }

  if (listen(sockfd, 4) == -1) {
    perror("listen error");
    exit(1);
  }

  /* Отсоединяем поток. Ждать основной поток его не будет. */
  /* Это позволит автоматически освобождать ресурсы, */
  /* когда поток  завершится */
  pthread_attr_setdetachstate(&workerAttr, PTHREAD_CREATE_DETACHED);
  
  while(1) {

    pthread_t worker;

    pthread_mutex_lock(&acceptAllowMutex);
    clientfd = accept(sockfd, (struct sockaddr*)&client, &len);

    if (clientfd == -1) {
      perror("accept error");
    }
    else {
      /* Я точно не знаю когда именно проснется мой поток... Проблема */
      /* Я могу поставить mutex перед accept , снимаю его сразу после копирования дескриптора*/
      if (pthread_create(&worker, &workerAttr, serveClient, NULL) != 0) {
        perror("pthread_create failed");
        continue;
      }
    }


  }



}