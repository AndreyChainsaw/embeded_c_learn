#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define SERVER_QUEUE_NAME "/mq-hello-server"
#define CLIENT_QUEUE_NAME "/mq-hello-client"
#define QUEUE_PERMS 0600
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256

int main(void) {
  mqd_t qd_server, qd_client;
  char message[MAX_MSG_SIZE] = "Hello client!";

  printf("Server: hello, world!\n");

  struct mq_attr attr;

  attr.mq_flags = 0;
  attr.mq_maxmsg = MAX_MESSAGES;
  attr.mq_msgsize = MAX_MSG_SIZE;
  attr.mq_curmsgs = 0;

  qd_server = mq_open(SERVER_QUEUE_NAME, O_CREAT | O_RDWR, QUEUE_PERMS, &attr);

  if (qd_server == -1) {
    perror("mq_open server failed");
    exit(EXIT_FAILURE);
  }

  qd_client = mq_open(CLIENT_QUEUE_NAME, O_CREAT | O_RDWR, QUEUE_PERMS, &attr);

  if (qd_client == -1) {
    perror("mq_open client failed!");
    exit(EXIT_FAILURE);
  }

  if (mq_send(qd_client, message, MAX_MSG_SIZE, 0) == -1) {
    perror("mq_send to client failed!");
    exit(EXIT_FAILURE);
  }

  printf("Server: reply message to client. Wait reply...\n");

  if (mq_receive(qd_server, message, MAX_MSG_SIZE, NULL) == -1) {
    perror("mq_receive from server failed");
    exit(EXIT_FAILURE);
  }

  printf("[CLIENT REPLY]: %s\n", message);


  if (mq_close(qd_server) == -1) {
    perror("mq_close server failed");
  }

  
  if(mq_unlink(SERVER_QUEUE_NAME) == -1){
    perror("mq_unlink server failed");
  }

  if(mq_close(qd_client) == -1) {
    perror("mq_close client failed");
  }


  if (mq_unlink(CLIENT_QUEUE_NAME) == -1) {
    perror("mq_unlink client failed");
  }

  return 0;


  
}