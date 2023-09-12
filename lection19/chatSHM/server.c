#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#include "server.h"
#include "chatCommon.h"



int main(void) {
  struct messages *msgs;
  int fdShMem;

  sem_t *semID = getSemaphore();
  msgs = getSharedMemory(&fdShMem, sizeof(*msgs));

  /* Очищаем выделенную память */
  memset(msgs, 0,  sizeof(*msgs));

  pushMessage(msgs, INIT_MESSAGE, semID);
  printf("%s", msgs->messageText);

  /* Держим память, ничего не делаем, пока пользователь не нажмет кнопку */
  int cmd = 0;
  scanf("$d", &cmd);

  pushMessage(msgs, INIT_MESSAGE, semID);

  sem_close(semID);
  munmap(msgs, sizeof(*msgs));
  close(fdShMem);

  return 0;

}

sem_t *getSemaphore() {
  sem_t *semID = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0666, 1);

  if (errno == EEXIST) {
    sem_close(semID);
    sem_unlink(SEM_NAME);
  }

  semID = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0666, 1);

  if (semID == SEM_FAILED) {
    perror("sem_open error");
    exit(1);
  }

  return semID;

  
}

void *getSharedMemory(int *fdShMem, size_t size) {
  *fdShMem = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);

  if (*fdShMem == -1) {
    perror("shm_open error");
    exit(1);
  }

  if(ftruncate(*fdShMem, size) == -1) {
    perror("ftruncate error");
    exit(1);
  }

  void *address = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, *fdShMem, 0);

  if (address == MAP_FAILED) {
    perror("mmap error");
    exit(1);
  }

  return address;
}