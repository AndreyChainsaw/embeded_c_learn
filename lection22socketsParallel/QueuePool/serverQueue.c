#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define QUEUE_NAME "/mq-server-queue"
#define QUEUE_PERMS "0600"
#define MAX_QUEUE_SIZE 10

#define SERVER_PORT 54321
#define SERVER_ADDRESS "127.0.0.1"

#define MAX_WORKERS 6

mqd_t qd_server;

void terminateServer(){
  if (mq_close(qd_server) == -1) {
    perror("mq_close error");
  }

  if (mq_unlink(QUEUE_NAME) == -1) {
    perror("mq_unlink error");
  }
}

void handleSignal(int sig) {
  /* run atexit function force */
  exit(1);
}


int sendDate(int sockfd, mqd_t qd_name) {

  socklen_t len = sizeof(struct sockaddr_in);
  char datetime[30];
  char buff[len];
  struct sockaddr_in *client;

  while(1) {
    if (mq_receive(qd_name, buff, len, NULL) == -1) {
      perror("mq_receive error");
      continue;
    }

    /* "Преобразовываю" массив char в указатель на структуру */
    client = (struct sockaddr_in*) buff;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(datetime, 30, "%d-%02d-%02d %02d:%02d:%02d\n",
            tm.tm_year + 1900, tm.tm_mon + 1,
            tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    if (sendto(sockfd, datetime, 30, 0, (struct sockaddr *)client, len) == -1) {
      perror("sendto error");
      continue;
    }

    printf("Process [%d] completed!\n", getpid());

  }

  exit(0);
}


/* Создаем дочерние процессы (numb - количество дочерних процессов) */
/* Передаем функцию, которую должны выполнять дочерние процессы */
int createWorkers(int numb, int (*action)(int, int), int sockfd, mqd_t qd_name) {
  for (int i =0; i < numb; i++) {
    pid_t pidNumber = fork();

    switch (pidNumber) {
    case 0:
      action(sockfd, qd_name);
      break;
    case -1:
      return -1;
    default:
      break;
    }

  }

  return 0;
}

void serverPrepare() {
  struct sigaction act = {0};
  act.sa_handler = handleSignal;

  sigaction(SIGINT, &act, NULL);

  if (atexit(terminateServer) == -1) {
    perror("atexit register error");
  }
}

int prepareSocket(struct sockaddr_in *endpoint) {
  int sock = socket(AF_INET, SOCK_DGRAM, 0);

  if (sock == -1) {
    perror("socket error");
    exit(1);
  }

  endpoint->sin_family = AF_INET;
  endpoint->sin_port = htons(SERVER_PORT);

  if (inet_pton(AF_INET, SERVER_ADDRESS, &endpoint->sin_addr) <= 0) {
    perror("inet_pton error");
    exit(1);
  }

  return sock;

}

mqd_t prepareMQ() {
  struct mq_attr attr;

  attr.mq_flags = 0;
  attr.mq_maxmsg = MAX_QUEUE_SIZE;
  attr.mq_msgsize = sizeof(struct sockaddr_in);
  attr.mq_curmsgs = 0;

  mqd_t queue_fd = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, QUEUE_PERMS, &attr);

  if (queue_fd == -1) {
    perror("mq_open error");
    exit(1);
  }

  return queue_fd;
}

int main() {
  /* Первичная подготовка к запуску */
  serverPrepare();

  /* Создаем сокеты */
  struct sockaddr_in server,client;
  socklen_t len = sizeof(struct sockaddr_in);

  int mainSock = prepareSocket(&server);

  /* Подготавливаем очередь сообщений */
  qd_server = prepareMQ();


  createWorkers(MAX_WORKERS, sendDate, mainSock, qd_server);

  if (bind(mainSock, (struct sockaddr*) &server, len) == -1) {
    perror("bind error");
    exit(1);
  }

  char command;
  while(1) {
    len = sizeof(struct sockaddr_in);

    if (recvfrom(mainSock, &command, 1, 0, (struct sockaddr*) &client, &len) == -1) {
      perror("recvfrom error");
      continue;
    }

    /* Отдаем запрос на обработку */
    if (mq_send(qd_server, (const char *) &client, len, 0) == -1) {
      perror("mq_send error");
      continue;
    }

  }

  for (int i = 0; i < MAX_WORKERS; i++) {
    wait(NULL);
  }

}