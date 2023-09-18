#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define SOCKET_PATHNAME "/tmp/af_local_stream.sock"

char isContinue = 1;

void terminateLoop(int sig) {
  // Завершаем цикл
  printf("Run graceful shutdown...\n");
  isContinue = 0;
}


int main(void) {
  struct sigaction act = {0};
  act.sa_handler = terminateLoop;

  sigaction(SIGINT, &act, NULL);
  sigaction(SIGTERM, &act, NULL);
  sigaction(SIGABRT, &act, NULL);

  struct sockaddr_un server, client;
  int socketFD, clientFD;
  
  socketFD = socket(AF_LOCAL, SOCK_STREAM, 0);

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

  if (listen(socketFD, 5) == -1) {
    perror("listen failed");
    exit(1);
  }

  socklen_t clientSize = sizeof(client);
  char message[100];
  char messageReply[110];

  while (isContinue) {
    //Данная функция создает отдельный fd под КАЖДОГО клиента
    clientFD = accept(socketFD, (struct sockaddr *) &client, &clientSize);

    if (clientFD == -1) {
      perror("accept error");
      continue;
    }

    if (recv(clientFD, message, 100, 0) == -1) {
      perror("recv error");
      continue;
    }

    snprintf(messageReply, 110, "[got message] %s", message);
    if (send(clientFD, messageReply, 110, 0) == -1) {
      perror("send error");
      continue;
    }

    close(clientFD);
    
  }
  
  // закрываем пассивный сокет
  close(socketFD);
  unlink(SOCKET_PATHNAME);
  return 0;
}