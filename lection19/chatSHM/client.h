#define MAX_ONE_MESSAGE_SIZE 100

struct threadPayloadMessage {
  struct messages *msgs;
  sem_t *semID;
};

void *getSharedMemory(int*, size_t);
sem_t* getSemaphore();
void* readMessages(void*);
void *sendMessage(void*);
