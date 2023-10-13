#include "rawAddition.h"


int main(void) {
  //TODO: поменять на нормальный. Или же научиться определять MAC по интерфейсу
  unsigned char dst_mac[6];
  unsigned char src_mac[6];

  struct sockaddr_ll server;
  struct packet packet;
  socklen_t socklen = sizeof(struct sockaddr_ll);

  /* Подготавливаем сокет */
  int sockfd = createSocket(&server, dst_mac);

  /* заполнение заголовков ETH */
  fillEthHeaders(&packet.eth_packet, dst_mac, src_mac);

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
