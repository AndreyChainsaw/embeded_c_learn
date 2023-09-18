#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define MESSAGE_LEN 100
#define SERVER_PORT 54321
#define SERVER_ADDRESS "127.0.0.1"

int main(void) {
  int clientfd, sockfd;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd == -1) {
    perror("socket error");
    exit(1);
  }

  struct sockaddr_in server, client;

  server.sin_family = AF_INET;
  server.sin_port = htons(SERVER_PORT);

  if (inet_pton(AF_INET, SERVER_ADDRESS, &server.sin_addr) <= 0) {
    perror("inet_pton error");
    exit(1);
  }

  if (bind(sockfd, (struct sockaddr*) &server, sizeof(server)) == -1) {
    perror("bind error");
    exit(1);
  }

  if (listen(sockfd, 5) == -1) {
    perror("listen error");
    exit(1);
  }

  char message[MESSAGE_LEN];
  char messageReply[MESSAGE_LEN];

  while(1) {
    socklen_t len = sizeof(client);
    clientfd = accept(sockfd, (struct sockaddr*)&client, &len);

    if (clientfd == -1) {
      perror("accept error");
      continue;
    }

    if (recv(clientfd, message, MESSAGE_LEN, 0) == -1) {
      perror("recv error");
      close(clientfd);
      continue;
    }

    snprintf(messageReply, MESSAGE_LEN, "[got message] %s", message);

    if (send(clientfd, messageReply, MESSAGE_LEN, 0) == -1) {
      perror("send error");
      continue;
    }

    close(clientfd);

  }
}