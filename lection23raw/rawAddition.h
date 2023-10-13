#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>        /* inet_pton */
#include <linux/ip.h>
#include <unistd.h>
#include <net/if.h>           /* if_nametoindex */
#include <linux/if_ether.h>   /* ethhdr */
#include <linux/if_packet.h>  /* sockaddr_ll */

#define CLIENT_PORT 54320
#define SERVER_PORT 54321
#define SERVER_ADDRESS ""
#define CLIENT_ADDRESS ""

#define MESSAGE "hello world!!!\n"
#define INTERFACE_NAME ""


struct eth_headers {
  unsigned char dst_mac[6];
  unsigned char src_mac[6];
  unsigned short proto;
} __attribute__((packed));

struct udp_headers {
  unsigned short     src_port;
  unsigned short     dst_port;
  unsigned short     len;
  unsigned short     check_sum;
  char               payload[100];
};

struct packet {
  struct eth_headers eth_packet;
  struct iphdr ip_packet;
  struct udp_headers udp_packet;
} __attribute__((packed));


unsigned int crc16(char *buff, size_t len);

int createSocket(struct sockaddr_ll *server, unsigned char *dst_mac);

void fillEthHeaders(struct eth_headers *eth,
  unsigned char *dst_mac, unsigned char *src_mac);

void fillUdpHeaders(struct udp_headers *udp);

void fillIpHeaders(struct iphdr *ip);