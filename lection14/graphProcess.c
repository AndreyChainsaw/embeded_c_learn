#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void)
{
	printf("MAIN PROCESS PID IS %d\n", (int) getpid());

	pid_t firstChild = fork(); // 1001
	pid_t secondChild = fork(); // 1002 and 1003

	if (0 == firstChild && 0 != secondChild) // in 1001
	{

		printf("\n\tI am 1001\n\tMY PID IS %d\n\tMY PPID IS %d\n",
			(int) getpid(), getppid());


		pid_t fourChild = fork(); //1004

		if (0 == fourChild) // in 1004
		{
			printf("\n\t\tI am 1004\n\t\tMY PID IS %d\n\t\tMY PPID IS %d\n",
				(int) getpid(), getppid());

			exit(0);
		} else
		{
			waitpid(fourChild, 0, 0); // wait 1004
			waitpid(secondChild, 0, 0); // wait 1003
			exit(0);
		}
			

	} else if (0 == firstChild && 0 == secondChild) // in 1003
	{
		printf("\n\t\tI am 1003\n\t\tMY PID IS %d\n\t\tMY PPID IS %d\n",
			(int) getpid(), getppid());

		exit(0);

	} else if (0 < firstChild && 0 == secondChild) // in 1002
	{
		printf("\n\tI am 1002\n\tMY PID IS %d\n\tMY PPID IS %d\n",
			(int) getpid(), getppid());

		pid_t fiveChild = fork();

		if (0 == fiveChild)
		{
			printf("\n\t\tI am 1005\n\t\tMY PID IS %d\n\t\tMY PPID IS %d\n",
				(int) getpid(), getppid());

			exit(0);
		}
		else
		{
			waitpid(fiveChild, 0, 0); // wait 1005
			exit(0);
		}

		
	}

	if (0 < firstChild && 0 < secondChild)
	{
		waitpid(firstChild, 0, 0);
		waitpid(secondChild, 0, 0);
		return 0;
	}

	
}