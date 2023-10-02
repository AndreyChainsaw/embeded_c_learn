#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> /* inet_pton */

#define CLIENT_PORT 54320
#define SERVER_PORT 54321
#define SERVER_ADDRESS "127.0.0.1"

#define MESSAGE "hello world!!!\n"

struct udp_headers {
  unsigned short src_port;
  unsigned short dst_port;
  unsigned short len;
  unsigned short check_sum;
  char payload[100];
};

int main(void) {
  int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  socklen_t socklen = sizeof(struct sockaddr_in);
  if (sockfd == -1) {
    perror("socket error");
    exit(1);
  }

  struct sockaddr_in server;

  server.sin_family = AF_INET;
  server.sin_port = htons(SERVER_PORT);

  if (inet_pton(AF_INET, SERVER_ADDRESS, &server.sin_addr) <= 0) {
    perror("inet_pton error");
    exit(1);
  }

  struct udp_headers udp;
  
  udp.src_port = htons(CLIENT_PORT);
  udp.dst_port = htons(SERVER_PORT);
  memcpy(udp.payload, MESSAGE, strlen(MESSAGE));
  udp.len = htons(8 + strlen(MESSAGE));

  if (sendto(sockfd, (char*)&udp, sizeof(struct udp_headers), 0, (struct sockaddr*)&server, socklen) == -1) {
    perror("sendto error");
    exit(1);
  }

  while (1) {
    if (recvfrom(sockfd, (void*)&udp, sizeof(struct udp_headers), 0, (struct sockaddr*)&server, &socklen) == -1) {
      perror("recvfrom error");
      continue;
    }

    /* skip ip_header (20Bytes) and udp src_port(2Bytes) */
    unsigned short *ptr = (unsigned short*)&udp;
    ptr += 11;

    /* Если порт назначения мой, то обработай */
    if (ntohs(*(ptr)) == CLIENT_PORT) {
      /* skip dst_port, len, check_summ */
      ptr += 3;
      printf("[Reply from server] %s", (char*)ptr);
    }
  }



  return 0;
}