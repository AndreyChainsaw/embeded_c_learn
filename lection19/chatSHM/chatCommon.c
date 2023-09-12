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

#include "chatCommon.h"

size_t pushMessage(struct messages *msgs, char *messageText, sem_t *semID) {
  sem_wait(semID);

  size_t sizeMessage = strlen(messageText);
  memcpy((msgs->messageText + msgs->offset), messageText, sizeMessage);

  msgs->offset += sizeMessage;

  sem_post(semID);

  return sizeMessage;
}