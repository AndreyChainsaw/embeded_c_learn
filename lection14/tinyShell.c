#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 11
#define LEN_COMMAND 50


// Поиск утилит должен быть в PATH. Пока можно думать, что она в PATH по-умолчанию
// ls -la | grep bin (как работать с пайплайном?)

char** tokenize(char*);

int main(int argc, char argv[]){
	char command[LEN_COMMAND] = {0};

	while (1) {
		size_t lenFirstCommand = 0;
		printf("# ");
		fgets(command, LEN_COMMAND, stdin);
		
		command[strcspn(command, "\n")] = 0;

		char **tokenPhrase = tokenize(command);
		
		pid_t childPID = fork();

		if (0 == childPID) {
			char fullPath[LEN_COMMAND + 6] = "/bin/"; // TODO: И что с размером делать?? Ерунда же полная...

			strcat(fullPath, *tokenPhrase);
			execv(fullPath, tokenPhrase);

		}
		else {
			int status = 0;
			wait(&status);
		}
	}
	return 0;

}

char** tokenize(char *phrase){
	char *token = strtok(phrase, " ");
	char **tokenPhrase = (char**) malloc(MAX_ARGS * sizeof(char*));

	for (int i = 0; token != NULL && i < MAX_ARGS - 1; i++) {
		tokenPhrase[i] = token;
		token = strtok(NULL, " ");
	}

	return tokenPhrase;
}
