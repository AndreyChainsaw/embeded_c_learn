#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MESSAGE_TEXT "hello server udp!\0"
#define SOCKET_PATHNAME "/tmp/af_local_dgram.sock"
#define SOCKET_CLIENT_PATHNAME "/tmp/af_local_dgram_client.sock"

int main(void) {
  struct sockaddr_un server, client;

  int sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0);

  if (sockfd == -1) {
    perror("socker error");
    exit(1);
  }

  server.sun_family = AF_LOCAL;
  memcpy(server.sun_path, SOCKET_PATHNAME, strlen(SOCKET_PATHNAME) + 1);

  client.sun_family = AF_LOCAL;
  memcpy(client.sun_path, SOCKET_CLIENT_PATHNAME, strlen(SOCKET_CLIENT_PATHNAME) + 1);

  if (bind(sockfd, (struct sockaddr*) &client, sizeof(client)) == -1) {
    perror("bind error");
    exit(1);
  }
  socklen_t serverSize = sizeof(server);

  if(sendto(sockfd, MESSAGE_TEXT, strlen(MESSAGE_TEXT)+1, 0, (struct sockaddr*) &server, serverSize) == -1) {
    perror("send error");
    exit(1);
  }

  char reply[100];
  if (recvfrom(sockfd, reply, 100, 0, (struct sockaddr*) &server, &serverSize) == -1) {
    perror("recv error");
    exit(1);
  }

  printf("%s", reply);

  return 0;
}