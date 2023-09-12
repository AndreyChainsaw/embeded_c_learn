#define SEM_NAME "/chatSEM"
#define SHM_NAME "/chatSHM"

#define INIT_MESSAGE "[server] hello everybody!\n"
#define MAX_BUFFER_CHAT 1000

struct messages {
  char messageText[MAX_BUFFER_CHAT];
  size_t offset;
};

size_t pushMessage(struct messages*, char*, sem_t*);