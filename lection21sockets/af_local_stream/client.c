#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MESSAGE_TEXT "hello server!\0"
#define SOCKET_PATHNAME "/tmp/af_local_stream.sock"

int main(void) {
  struct sockaddr_un server;

  int sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);

  if (sockfd == -1) {
    perror("socker error");
    exit(1);
  }

  server.sun_family = AF_LOCAL;
  memcpy(server.sun_path, SOCKET_PATHNAME, strlen(SOCKET_PATHNAME) + 1);

  socklen_t serverSize = sizeof(server);
  if (connect(sockfd, (struct sockaddr *) &server, serverSize) == -1) {
    perror("connect error");
    exit(1);
  }

  if(send(sockfd, MESSAGE_TEXT, strlen(MESSAGE_TEXT)+1, 0) == -1) {
    perror("send error");
    exit(1);
  }

  char reply[110];
  if (recv(sockfd, reply, 110, 0) == -1) {
    perror("recv error");
    exit(1);
  }

  printf("%s", reply);



  return 0;
}