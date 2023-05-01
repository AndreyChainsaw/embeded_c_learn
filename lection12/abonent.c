#include <stdio.h>
#include <stdlib.h>
#include "liblist.h"

struct abonent
{
	char surname[10];
	char name[10];
	char phoneNumber[11]; // INFO: or better use unsigned long int?
};

// TODO: add check user input
// TODO: add cleaner function after exit;
// TODO: quickSort?


struct abonent* addUser(list *, int);
void delUser(list*, int);
void printUserBySeqNumb(list *, int);
void printAllUsers(list *);

int main(void)
{
	//WARN: should refactor it
	struct abonent example;
	list *users = init(&example); // TODO: init only on first iteration
	
	// WARN: bad idea.....
	unsigned int tail = 1;

	while (1)
	{
		int action;

		printf("============================\n\n");
		printf("1. Display all users\n");
		printf("2. Add user\n");
		printf("3. Delete user\n");
		printf("4. Sort users by surname\n");
		printf("5. Exit from programm\n\n");
		printf("============================\n\n");
		printf("Action: ");

		scanf("%d", &action);

		switch(action)
		{
			case 1:
				printAllUsers(users);
				break;

			case 2:
				addUser(users, tail);
				tail++;
				break;

			case 3:
				int number;
				printf("Enter the number of the user to be deleted: ");
				scanf("%d", &number);
				delUser(users, number);
				tail--;
				break;

			case 4:

				break;

			case 5:
				printf("Good bye!\n");
				exit(1);

			default:
				printf("Incorrect acton!\n");
				break;

		}
	}
	
	return 0;
}

struct abonent* addUser(list *user, int number)
{
	struct abonent *tmp = (struct abonent*)malloc(sizeof(struct abonent));

	printf("\nEnter a surname: ");
	scanf("%s", tmp->surname);

	printf("Enter a name: ");
	scanf("%s", tmp->name);

	list *node = getElementById(user, number-1);
	addElem(node, tmp);

	return tmp;
}

// INFO: may be add return code??? 
void delUser(list *users, int number)
{
	list *node = getElementById(users, number);
	delElem(node, users);
}

void printUserBySeqNumb(list *users, int number)
{
	list *listUser = getElementById(users, number);
	struct abonent *user = listUser->value;

	printf("name = %s\n", user->name);
	printf("surname = %s\n", user->surname);
}

// TODO liblist should do it or not?
void printAllUsers(list *users)
{
	list *ptr = users->next; // WARN: first user is default. Should refactor

	int number = 1;
	while (ptr != 0)
	{
		// WARN: Duplicate with printUserBySeqNumb
		printf("\n");
		printf("\tUser number = %d\n", number);
		printf("\tname = %s\n", ptr->value->name);
		printf("\tsurname = %s\n\n", ptr->value->surname);
		ptr = ptr->next;
		number++;
	}
}

