#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> /* inet_pton */
#include <linux/ip.h>
#include <unistd.h>

#define CLIENT_PORT 54320
#define SERVER_PORT 54321
#define SERVER_ADDRESS "127.0.0.1"

#define MESSAGE "hello world!!!\n"


struct udp_headers {
  unsigned short     src_port;
  unsigned short     dst_port;
  unsigned short     len;
  unsigned short     check_sum;
  char               payload[100];
};

struct packet {
  struct iphdr ip_packet;
  struct udp_headers udp_packet;
};


int createSocket(struct sockaddr_in *server) {
  int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  socklen_t socklen = sizeof(struct sockaddr_in);
  if (sockfd == -1) {
    perror("socket error");
    exit(1);
  }

  int enable = 1;
  if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &enable, sizeof(enable)) == -1) {
    perror("setsockopt error");
  }

  /* Port не заполняю, так как это делается вручную */

  server->sin_family = AF_INET;
  if (inet_pton(AF_INET, SERVER_ADDRESS, &server->sin_addr) <= 0) {
    perror("inet_pton error");
    exit(1);
  }

  return sockfd;
}

//TODO: getPort (little endian)
//TODO: getPayload (little endian)

void fillUdpHeaders(struct udp_headers *udp) {

  udp->src_port   = htons(CLIENT_PORT);
  udp->dst_port   = htons(SERVER_PORT);
  udp->check_sum  = 0;
  udp->len        = htons(8 + strlen(MESSAGE));

  /*copy message to payload */
  memcpy(udp->payload, MESSAGE, strlen(MESSAGE));
}

void fillIpHeaders(struct iphdr *ip) {
  ip->version   = 4;
  ip->ihl       = 5;
  ip->tos       = 0;
  ip->id        = 0; /* auto */
  ip->frag_off  = htons(0x4000); /*dont fragment */
  ip->ttl       = 64;
  ip->protocol  = IPPROTO_UDP;
  ip->check     = 0; /* auto */
  ip->saddr     = 0; /* auto */
  inet_pton(AF_INET, SERVER_ADDRESS, &ip->daddr);
}

int main(void) {
  struct sockaddr_in server;
  struct packet packet;
  socklen_t socklen = sizeof(struct sockaddr_in);

  /* Подготавливаем сокет */
  int sockfd = createSocket(&server);

  /* заполнение заголовков IP */
  fillIpHeaders(&packet.ip_packet);

  /* заполнение заголовков UDP */
  fillUdpHeaders(&packet.udp_packet);

  if (sendto(sockfd, (char*)&packet, sizeof(struct packet), 0, (struct sockaddr*)&server, socklen) == -1) {
    perror("sendto error");
    exit(1);
  }
  
  while (1) {
    if (recvfrom(sockfd, (void*)&packet, sizeof(struct packet), 0, (struct sockaddr*)&server, &socklen) == -1) {
      perror("recvfrom error");
      continue;
    }

    struct udp_headers *udp_packet = &packet.udp_packet;


    /* Если порт назначения мой, то обработай */
    if (ntohs(udp_packet->dst_port) == CLIENT_PORT) {
      printf("[Reply from server] %s", udp_packet->payload);
    }
  }



  return 0;
}