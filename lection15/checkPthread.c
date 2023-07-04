#include <pthread.h>
#include <stdio.h>

void *thread_calc(void *args)
{
	int *ptr = (int *) args;
	*ptr += 1;
	printf("I am the child thread!\n");
	printf("%d\n", *ptr);
	pthread_exit(500);
}

int main(void)
{
	pthread_t thread;

	int i = 120;
	void *status;
	pthread_create(&thread, NULL, thread_calc, (void *) &i);

	printf("I am the main thread!\n");

	pthread_join(thread, &status);

	printf("%d\n", (int)status);
	return 0;
}