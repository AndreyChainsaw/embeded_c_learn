#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define SOCKET_PATHNAME "/tmp/af_local_dgram.sock"

int socketFD;


void stopProcess(void) {
  if (close(socketFD) == -1) {
    perror("close socket error");
  }
  if (unlink(SOCKET_PATHNAME) == -1) {
    perror("unlink socket file error");
  }
}

void terminateLoop(int sig) {
  // Завершаем цикл
  printf("Run graceful shutdown...\n");
  exit(0);
}


int main(void) {
  struct sigaction act = {0};
  act.sa_handler = terminateLoop;

  sigaction(SIGINT, &act, NULL);
  atexit(stopProcess);

  char isContinue = 1;
  struct sockaddr_un server, client;
  
  socketFD = socket(AF_LOCAL, SOCK_DGRAM, 0);

  if (socketFD == -1) {
    perror("socket error");
    exit(1);
  }

  server.sun_family = AF_LOCAL;
  // sun_path must be NULL terminated
  memcpy(server.sun_path, SOCKET_PATHNAME, strlen(SOCKET_PATHNAME) + 1); 

  // Назначаем endpoint для сокета (в нашем случае endpoint - это имя файла)
  if (bind(socketFD, (struct sockaddr *)&server, sizeof(server)) == -1) {
    perror("bind error");
    exit(1);
  }

  socklen_t clientSize;
  char message[100];
  char messageReply[100];

  while (isContinue) {

    clientSize = sizeof(struct sockaddr_un);

    if (recvfrom(socketFD, message, 100, 0, (struct sockaddr*) &client, &clientSize) == -1) {
      perror("recvfrom error");
      continue;
    }

    snprintf(messageReply, 100, "[got message] %s", message);
    if (sendto(socketFD, messageReply, 100, 0, (struct sockaddr*) &client, clientSize) == -1) {
      perror("send error");
      continue;
    }
    
  }
  
  return 0;
}
