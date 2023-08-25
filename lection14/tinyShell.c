#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 11
#define LEN_COMMAND 50
#define MAX_PIPES 10

struct execCandidat {
  char **tokenPhrase;       // Команда, разбитая на токены по " "
  unsigned char *pipes;     // индексы нахождения символов "|" в tokenPhrase
  unsigned char countPipes; // Количество pipe в команде
};


char** tokenize(char*);
void executeInstructions(struct execCandidat *parsedCmd);

int main(int argc, char argv[]){

  char rawCmd[LEN_COMMAND] = {0};
  unsigned char pipes[MAX_PIPES];
  struct execCandidat parsedCmd;

  while (1) {
    size_t lenFirstCommand = 0;
    printf("# ");
    fgets(rawCmd, LEN_COMMAND, stdin);

    pipes[0] = 0;
    
    // Заменяет символ переноса строки на NULL
    rawCmd[strcspn(rawCmd, "\n")] = 0;

    parsedCmd.tokenPhrase = tokenize(rawCmd);
    parsedCmd.pipes = pipes;
    parsedCmd.countPipes = 0;
    

    // Ищем символ pipe
    for (int i = 0; parsedCmd.tokenPhrase[i] != NULL; i++) {
      if (0 == strcmp("|", parsedCmd.tokenPhrase[i])) {
        parsedCmd.countPipes++;
        parsedCmd.pipes[parsedCmd.countPipes - 1] = i;
      }
    }

    executeInstructions(&parsedCmd);
    free(parsedCmd.tokenPhrase);
    
  }
  return 0;

}

void executeInstructions(struct execCandidat *parsedCmd) {
  pid_t childPID[parsedCmd->countPipes + 1];

  // Пока считаем, что может быть только один канал
  if (parsedCmd->countPipes > 0) {
    pid_t childPID[2];
    int pipefd[2];
    pipe(pipefd);

    childPID[0] = fork();

    
    if (childPID[0] == 0) {
      close(pipefd[0]);
      close(STDOUT_FILENO);

      // Перенаправляю stdout в канал
      dup(pipefd[1]);

      char fullPath[LEN_COMMAND + 6] = "/bin/";
      char *tokenPhrase[parsedCmd->pipes[0] + 1];

      for (int i = 0; i < parsedCmd->pipes[0]; i++) {
        tokenPhrase[i] = parsedCmd->tokenPhrase[i];
      }
      //Сделаем его NULL терминированным
      tokenPhrase[parsedCmd->pipes[0]] = NULL;

      strcat(fullPath, tokenPhrase[0]);
      execv(fullPath, tokenPhrase);
    }
    
    wait(NULL);

    childPID[1] = fork();

    if (childPID[1] == 0) {
      close(STDIN_FILENO);
      close(pipefd[1]);
      dup(pipefd[0]);
      close(pipefd[0]);


      char fullPath[LEN_COMMAND + 6] = "/bin/";
      char startCommand = parsedCmd->pipes[0] + 1;
      
      // Ищем конец массива
      unsigned char endPhrase = startCommand;

      while (parsedCmd->tokenPhrase[endPhrase] != NULL) {
        endPhrase++;
      }

      // TODO: функцию по нахождению конца токенов
      char *tokenPhrase[endPhrase];
      
      for (int i = 0; i < endPhrase; i++) {
        tokenPhrase[i] = parsedCmd->tokenPhrase[i + startCommand];
      }
      //Сделаем его NULL терминированным
      tokenPhrase[endPhrase] = NULL;

      strcat(fullPath, tokenPhrase[0]);
      execv(fullPath, tokenPhrase);

    }
    close(pipefd[1]);
    close(pipefd[0]);
    wait(NULL);
  
  }
  else {
    if (fork() == 0){
      char fullPath[LEN_COMMAND + 6] = "/bin/";
      strcat(fullPath, parsedCmd->tokenPhrase[0]);
      execv(fullPath, parsedCmd->tokenPhrase);
    }

    wait(NULL);

    
  }
}

char** tokenize(char *phrase){
  char *token = strtok(phrase, " ");
  char **tokenPhrase = (char**) calloc(MAX_ARGS, sizeof(char*));

  for (int i = 0; token != NULL && i < MAX_ARGS - 1; i++) {
    tokenPhrase[i] = token;
    token = strtok(NULL, " ");
  }

  return tokenPhrase;
}
