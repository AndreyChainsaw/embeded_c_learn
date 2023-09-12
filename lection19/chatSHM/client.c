#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>

#include "chatCommon.h"
#include "client.h"

//TODO перенести в argv
#define username "[user] "

int main(void) {
  struct messages *msgs;
  int fdShMem;
  pthread_t thread_read_message;
  pthread_t thread_send_message;

  msgs = getSharedMemory(&fdShMem, sizeof(*msgs));
  sem_t *semID = getSemaphore();

  struct threadPayloadMessage tpm = {
    .msgs = msgs,
    .semID = semID
  };

  pthread_create(&thread_read_message, NULL, readMessages, (void*)&tpm);
  pthread_create(&thread_send_message, NULL, sendMessage, (void*)&tpm);

  pthread_join(thread_read_message, 0);
  pthread_join(thread_send_message, 0);

  return 0;

}

void *getSharedMemory(int *fd, size_t sizeMem) {
  *fd = shm_open(SHM_NAME, O_RDWR, 0600);

  if (*fd == -1) {
    perror("shm_open error");
    exit(1);
  }

  if (ftruncate(*fd, sizeMem) == -1) {
    perror("ftruncate error");
    exit(1);
  }

  void *address = mmap(0, sizeMem, PROT_READ | PROT_WRITE, MAP_SHARED, *fd, 0);

  if (address == MAP_FAILED) {
    perror("mmap eroror");
    exit(1);
  }

  return address;
}

sem_t* getSemaphore() {
  sem_t *semID = sem_open(SEM_NAME, 0);

  if (semID == SEM_FAILED) {
    perror("sem_open error");
    exit(1);
  }

  return semID;
}


/* Отдельный поток под чтение сообщений */
void* readMessages(void *payload) {
  struct threadPayloadMessage *trm = (struct threadPayloadMessage*) payload;
  struct messages *msgs = trm->msgs;
  sem_t *semID = trm->semID;

  size_t currentOffset = 0;

  while(1) {
    sem_wait(semID);

    if (currentOffset != msgs->offset) {
      printf("%s", (msgs->messageText + currentOffset));
      currentOffset = msgs->offset;
    }

    sem_post(semID);
    sleep(2);
  } 
}

/* Отдельный поток под запись */
void *sendMessage(void *payload) {
  struct threadPayloadMessage *tpm = (struct threadPayloadMessage*) payload;
  struct messages *msgs = tpm->msgs;
  sem_t *semID = tpm->semID;


  char localMessage[MAX_ONE_MESSAGE_SIZE];

  int usernameLen = strlen(username);
  memcpy(localMessage, username, usernameLen);

  while (1) {
    fgets(localMessage + usernameLen,
      MAX_ONE_MESSAGE_SIZE - usernameLen, stdin);

    pushMessage(msgs, localMessage, semID);
  }
}