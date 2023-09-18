#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define MESSAGE_LEN 100


int main(int argc, char *argv[]) {
  char buff[MESSAGE_LEN];
  unsigned short port_server;
  int sockfd;
  struct sockaddr_in server;
  socklen_t len = sizeof(server);

  if (argc < 3) {
    printf("Usage:\n\t%s <address_server> <port_server>\n", argv[0]);
    exit(1);
  }

  //WARN: небезопасное преобразование...
  port_server = atoi(argv[2]);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd == -1) {
    perror("socket error");
    exit(1);
  }

  

  server.sin_family = AF_INET;
  server.sin_port = htons(port_server);

  if (inet_pton(AF_INET, argv[1], &server.sin_addr) <= 0) {
    perror("inet_pton error");
    exit(1);
  }

  if (connect(sockfd, (struct sockaddr*) &server, len) == -1) {
    perror("connect error");
    exit(1);
  }

  

  printf("Write text: ");
  if (fgets(buff, MESSAGE_LEN, stdin) == NULL) {
    printf("fgets error");
    // Непонятно как дальше действовать, закрыть сокет или что?
    exit(1);
  }

  if (send(sockfd, buff, strlen(buff) + 1, 0) == -1) {
    perror("send error");
    exit(1);
  }

    

  if (recv(sockfd, buff, MESSAGE_LEN, 0) == -1) {
    perror("recv error");
  }

  printf("%s", buff);

  close(sockfd);
}