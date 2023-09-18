#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define SERVER_PORT 54321
#define SERVER_ADDRESS "127.0.0.1"
#define MESSAGE_LEN 100

int uppercase(char *str, size_t len) {

  for (int i = 0; str[i] != 0 || i < len; i++) {
    if (str[i] >= 'a' && str[i] <= 'z') {
      str[i] -= 32;
    }
  }

  return 0;
}

int main(void) {
  struct sockaddr_in server, client;
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  char buff[MESSAGE_LEN];

  socklen_t len;

  if (sockfd == -1) {
    perror("socket error");
    exit(1);
  }

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

  while(1) {
    len = sizeof(struct sockaddr_in);

    if (recvfrom(sockfd, buff, MESSAGE_LEN, 0, (struct sockaddr*) &client, &len) == -1) {
      perror("recvfrom error");
      continue;
    }

    // Делаем заглавные буквы
    uppercase(buff, MESSAGE_LEN);

    if (sendto(sockfd, buff, MESSAGE_LEN, 0, (struct sockaddr*) &client, len) == -1) {
      perror("sendto error");
      continue;
    } 
  }
}