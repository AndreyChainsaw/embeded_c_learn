#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_QUEUE_NAME "/mq-hello-server"
#define CLIENT_QUEUE_NAME "/mq-hello-client"
#define MAX_MSG_SIZE 256

int main(void) {
  mqd_t qd_server, qd_client;
  char message[MAX_MSG_SIZE];


  qd_client = mq_open(CLIENT_QUEUE_NAME, O_RDWR);

  if (qd_client == -1) {
    perror("mq_open client failed");
    exit(EXIT_FAILURE);
  }

  qd_server = mq_open(SERVER_QUEUE_NAME, O_RDWR);

  if (qd_client == -1) {
    perror("mq_open server failed");
    exit(EXIT_FAILURE);
  }

  /* Get reply from server */
  printf("Wait reply from server...\n");

  if (mq_receive(qd_client, message, MAX_MSG_SIZE, NULL) == -1) {
    perror("mq_receive from client failed");
    exit(EXIT_FAILURE);
  }

  printf("[SERVER REPLY]: %s\n", message);

  /* Send message to server */

  memcpy(message, "hi, server\0", strlen("hi, server\0") + 1);
  if (mq_send(qd_server, message, MAX_MSG_SIZE, 0) == -1) {
    perror("mq_send to server failed");
    exit(EXIT_FAILURE);
  }

  printf("Client: Send message to server");

  return 0;

}
