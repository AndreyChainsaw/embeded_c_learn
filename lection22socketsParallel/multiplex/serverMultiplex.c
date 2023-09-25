#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <time.h>
#include <poll.h>
#include <stdio.h>
#include <unistd.h>

#define SERVER_PORT 54321
#define SERVER_ADDRESS "127.0.0.1"

void getDate(char *buf, int len) {
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  snprintf(buf, len, "%d-%02d-%02d %02d:%02d:%02d\n",
          tm.tm_year + 1900, tm.tm_mon + 1,
          tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

int getUDP(int activeSocket) {
  struct sockaddr_in clientAddr;
  socklen_t len = sizeof(struct sockaddr_in);

  char datetime[30];
  char command;

  if (recvfrom(activeSocket, &command, 1, 0, (struct sockaddr*)&clientAddr, &len) == -1) {
    perror("recvfrom UDP error");
    return -1;
  }

  getDate(datetime, 30);

  if (sendto(activeSocket, datetime, 30, 0, (struct sockaddr*) &clientAddr, len) == 1) {
    perror("sendto UDP error");
    return -1;
  }

  return 1;



}

int acceptTCP(int passiveSocket) {
  struct sockaddr_in clientAddr;
  socklen_t len = sizeof(struct sockaddr_in);
  int clientfd = accept(passiveSocket, (struct sockaddr*) &clientAddr, &len);

  if (clientfd == -1) {
    perror("acccept TCP error");
    return -1;
  }

  char datetime[30];
  char command;

  getDate(datetime, 30);

  if (recv(clientfd, &command, 1, 0) == -1) {
    perror("recv TCP error");
    close(clientfd);
    return -1;
  }

  if (send(clientfd, datetime, 30, 0) == -1) {
    perror("send error");
    close(clientfd);
    return -1;
  }

  close(clientfd);
  return 0;

}


int main() {
  struct sockaddr_in server;

  /* TCP - 0; UDP - 1 */
  int sockets[2];
  int prot[2] = {SOCK_STREAM, SOCK_DGRAM};
  int (*actions[2])(int) = {acceptTCP, getUDP};

  socklen_t len = sizeof(struct sockaddr_in);
  
  for (int i = 0; i < 2; i++) {
    sockets[i] = socket(AF_INET, prot[i], 0);

    if (sockets[i] == -1) {
      perror("socket error");
      exit(1);
    }
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(SERVER_PORT);

  if (inet_pton(AF_INET, SERVER_ADDRESS, &server.sin_addr) != 1) {
    perror("inet pton error");
    exit(1);
  }

  for (int i = 0; i < 2; i++) {
    if (bind(sockets[i], (struct sockaddr*) &server, len) == -1) {
      perror("bind error");
      exit(1);
    }
  }

  if (listen(sockets[0], 5) == -1) {
    perror("listen error");
    exit(1);
  }

  struct pollfd fds[2];

  for (int i = 0; i < 2; i++) {
    fds[i].fd = sockets[i];
    fds[i].events = POLLIN;
  }

  while(1) {
    int ret = poll(fds, 2, 100);

    switch (ret) {
      case -1:
        perror("poll error");
        break;
      case 0:
        break;
      default:
        for (int i = 0; i < 2; i++) {
          if (fds[i].revents & POLLIN) {
            /* Вызываем обработчик UDP или TCP */
            actions[i](sockets[i]);
          }
        }
    }
  }

}

