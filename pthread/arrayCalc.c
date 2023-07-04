#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define SIZE_ARRAY 1000000000
#define THREAD_COUNT 10000


struct arr {
	char *arrayStart;
	size_t start;
	size_t end;
};


char *createArray(size_t);
void *getSumm(void *);

int main(void) {
	char *array = createArray(SIZE_ARRAY);
	unsigned int chunk = SIZE_ARRAY / THREAD_COUNT;

	// под 24 байта на структуру. Много....
	struct arr *structArray = (struct arr*) malloc (sizeof(struct arr) * chunk);

	if (NULL == structArray) {
		perror("Ошибка выделения памяти!");
		exit(0);
	}

	pthread_t threads[THREAD_COUNT];
	
	for (size_t i = 0; i < THREAD_COUNT; i++) {
		structArray[i].arrayStart = array;
		structArray[i].start = i * chunk;
		structArray[i].end =  (i * chunk) + chunk;
		pthread_create(&threads[i], NULL, getSumm, (void*) &structArray[i]);
		
	}

	// store results from threads
	void *chunkResults[THREAD_COUNT];

	// wait all threads and store his results to chunkResults[]
	for (size_t i = 0; i < THREAD_COUNT; i++) {
		pthread_join(threads[i], &chunkResults[i]);
	}

	free(array);
	free(structArray);

	// summ results[] elements
	int output = 0;
	for (size_t i = 0; i < THREAD_COUNT; i++) {
		output += *((int *) chunkResults[i]);
		free(chunkResults[i]);
	}

	printf("[THREADS %d] %d\n", THREAD_COUNT, output);

	return 0;
}

char *createArray(size_t elemArray) {
	char *array = (char *) malloc( elemArray * sizeof(char));

	for (size_t i = 0; i < elemArray; i++) {
		array[i] = 1;
	}

	return array;
}

void *getSumm(void *threadData) {
	int *result = (int *) malloc(sizeof(int));
	struct arr *arrayData = (struct arr *) threadData;

	for (size_t i = arrayData->start; i < arrayData->end; i++) {
		*result += arrayData->arrayStart[i];
	}

	return (void *) result;
}