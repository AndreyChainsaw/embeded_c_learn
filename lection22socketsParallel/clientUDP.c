#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_CLIENTS 3 

#define SERVER_PORT 54321
#define SERVER_ADDRESS "127.0.0.1"

pthread_t clients[MAX_CLIENTS];
struct sockaddr_in server; 
socklen_t len = sizeof(struct sockaddr_in);

void *communicateWithServer() {
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  if (sockfd == -1) {
    perror("socket error");
    exit(1);
  }


  if (connect(sockfd, (struct sockaddr*) &server, len) == -1) {
    perror("connect failed");
    exit(1);
  }

  char command[1] = { 1 };
  if (send(sockfd, command, 1, 0) == -1) {
    perror("send error");
    exit(1);
  }

  char reply[30];

  if (recv(sockfd, reply, 30, 0) == -1) {
    perror("recv error");
    exit(1);
  }

  printf("%s", reply);

  close(sockfd);

  pthread_exit(0);
}

int main() {

  server.sin_family = AF_INET;
  server.sin_port = htons(SERVER_PORT);

  if (inet_pton(AF_INET, SERVER_ADDRESS, &server.sin_addr) <= 0) {
    perror("inet_pton error");
    exit(1);
  }

  for (int i = 0; i < MAX_CLIENTS; i++) {
    pthread_create(&clients[i], NULL, communicateWithServer, NULL);
  }

  for (int i = 0; i < MAX_CLIENTS; i++) {
    pthread_join(clients[i], NULL);
  }





  return 0;
}
