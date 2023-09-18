#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define SERVER_PORT 54321
#define SERVER_ADDRESS "127.0.0.1"
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

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  if (sockfd == -1) {
    perror("socket error");
    exit(1);
  }

  //WARN: небезопасное преобразование...
  port_server = atoi(argv[2]);

  server.sin_family = AF_INET;
  server.sin_port = htons(port_server);

  if (inet_pton(AF_INET, argv[1], &server.sin_addr) <= 0) {
    perror("inet_pton error");
    exit(1);
  }

  // Передача данных

  printf("Write text: ");
  if (fgets(buff, MESSAGE_LEN, stdin) == NULL) {
    perror("fgets error");
    exit(1);
  }

  if (sendto(sockfd, buff, strlen(buff) + 1, 0, (struct sockaddr*) &server, len) == -1) {
    perror("sendto error");
    exit(1);
  }

  if (recvfrom(sockfd, buff, MESSAGE_LEN, 0, (struct sockaddr*) &server, &len) == -1) {
    perror("recvfrom error");
    exit(1);
  }

  printf("Reply from server: %s", buff);

  if (close(sockfd) == -1) {
    perror("close sockfd error");
  }

  return 0;

}