#include "rawAddition.h"

unsigned int crc16(char *buff, size_t len) {
  unsigned int csum = 0;
  unsigned short *ptr = (unsigned short*) buff;

  /* Берем массив парами байт и суммируем */
  for (int i = 0; i < len / 2; i++) {
    csum = csum + ptr[i];
  }

  unsigned short tmp = csum >> 16;
  csum = (csum & 0xFFFF) + tmp;

  return (unsigned) ~csum;
}


int createSocket(struct sockaddr_ll *server, unsigned char *dst_mac) {
  int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  socklen_t socklen = sizeof(struct sockaddr_ll);

  if (sockfd == -1) {
    perror("socket error");
    exit(1);
  }

  unsigned int ifindex = if_nametoindex(INTERFACE_NAME);

  if (ifindex == 0) {
    perror("if_nametoindex error");
    exit(1);
  }

  memset(server, 0, socklen);

  server->sll_family = AF_PACKET;
  server->sll_halen = ETH_ALEN;
  server->sll_ifindex = ifindex;
  memcpy(server->sll_addr, dst_mac, 6);

  return sockfd;
}


void fillEthHeaders(struct eth_headers *eth,
 unsigned char *dst_mac, unsigned char *src_mac) {
  
  memcpy(eth->dst_mac, dst_mac, ETH_ALEN);
  memcpy(eth->src_mac, src_mac, ETH_ALEN);
  eth->proto = htons(0x0800); 
}


void fillUdpHeaders(struct udp_headers *udp) {
  udp->src_port   = htons(CLIENT_PORT);
  udp->dst_port   = htons(SERVER_PORT);
  
  /* disable validation */
  udp->check_sum  = 0;
  udp->len        = htons(sizeof(struct udp_headers));

  /*copy message to payload */
  memcpy(udp->payload, MESSAGE, strlen(MESSAGE));
}


void fillIpHeaders(struct iphdr *ip) {
  ip->version   = 4;
  ip->ihl       = 5;

  /* Длина заголовка IP в байтах */
  size_t len = (ip->ihl * 32) / 8;

  ip->tot_len   = htons(len + sizeof(struct udp_headers));
  ip->tos       = 0;
  ip->id        = 0;
  ip->frag_off  = htons(0x4000); /*dont fragment */
  ip->ttl       = 64;
  ip->protocol  = IPPROTO_UDP;
  ip->check     = 0;

  inet_pton(AF_INET, CLIENT_ADDRESS, &ip->saddr);
  inet_pton(AF_INET, SERVER_ADDRESS, &ip->daddr);

  /* Разворачивать контрольную сумму не нужно */
  ip->check     = crc16((char*)ip, len);
}
