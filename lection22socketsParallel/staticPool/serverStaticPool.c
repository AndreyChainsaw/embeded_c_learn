#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define  MAX_WORKERS 6
#define SERVER_PORT 54321
#define SERVER_ADDRESS "127.0.0.1"

pthread_t workers[MAX_WORKERS];
int sockfd;

void *serveClient() {
    int clientSocket;
    struct sockaddr_in client;
    socklen_t lenClient = sizeof(struct sockaddr_in);

    char command;
    char datetime[30];
    time_t t;

    while (1) {
      clientSocket = accept(sockfd, (struct sockaddr*)&client, &lenClient);

      if (clientSocket == -1) {
        perror("accept error");
        continue;
      }

      /* Получаем число, если 1, то отдаем дату */
      if (recv(clientSocket, &command, 1, 0) == -1) {
        perror("recv error");
        close(clientSocket);
        continue;
      }

      /* генерируем datetime, чтобы отдать клиенту */
      t = time(NULL);
      struct tm tm = *localtime(&t);
      snprintf(datetime, 30, "%d-%02d-%02d %02d:%02d:%02d\n",
              tm.tm_year + 1900, tm.tm_mon + 1,
              tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

      if (send(clientSocket, datetime, 30, 0) == -1) {
        perror("send error");
        close(clientSocket);
        continue;
      }

      printf("thread [%lu] done\n", pthread_self());

      close(clientSocket);

    }
}

int main() {
  int clientfd[MAX_WORKERS];
  struct sockaddr_in server;
  socklen_t len = sizeof(struct sockaddr_in);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd == -1) {
    perror("socket error");
    exit(1);
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(SERVER_PORT);

  if (inet_pton(AF_INET, SERVER_ADDRESS, &server.sin_addr) != 1) {
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

  while(1) {
    for (int i = 0; i < MAX_WORKERS; i++) {
      pthread_create(&workers[i], NULL, serveClient, NULL);
    }

    for (int i = 0; i < MAX_WORKERS; i++) {
      /* Я их никогда не дождусь. Стоит ли их в joinable состоянии оставлять? */
      pthread_join(workers[i], NULL);
    }
  }
}