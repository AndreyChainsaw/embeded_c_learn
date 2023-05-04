#include <stdio.h>
#include <stdlib.h>
#include "liblist.h"

#define SIZE_NAME 10

struct abonent
{
	char surname[SIZE_NAME];
	char name[SIZE_NAME];
	char phoneNumber[11]; // INFO: or better use unsigned long int?
};

// TODO: add cleaner function after exit;
// TODO: write compare string function by alphabet

struct abonent* addUser(list *, int);
void delUser(list*, int);
void printUserBySeqNumb(list *, int);
void printAllUsers(list *);

int weightString(char *string);
void sortBySurname(list *, int);

int main(void)
{
	struct abonent fakeHead;
	list *users = init(&fakeHead);
	
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

		int statusRead = scanf("%d", &action);

		if (0 == statusRead)
		{
			printf("Incorrect Action!\n");
			scanf("%*[^\n]"); // clear the buffer
		}

		else
		{
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

					int statusRead = scanf("%d", &number);

					if (0 == statusRead)
					{
						printf("Incorrect input!\n");
						break;
					}

					delUser(users, number);
					tail--;
					break;

				case 4:
					sortBySurname(users, tail);
					break;

				case 5:
					printf("Good bye!\n");
					exit(1);

				default:
					printf("Incorrect acton!\n");
					break;

			}
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

// INFO: liblist should do it or not?
void printAllUsers(list *users)
{
	list *ptr = users->next; // INFO: first element is fake head

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

int weightString(char *string)
{
	int summ = 0;

	for (int i = 0; string[i] != 0; i++)
	{
		summ += (int)string[i];
	}

	return summ;
}

//insert Sort
void sortBySurname(list *users, int size)
{
	
	for (int i = 2; i < size; i++)
	{
		// WARN: Too much getElementById (iterate to list)... Should optimize it!
		for (int j = i; j > 0 && weightString(getElementById(users, j-1)->value->surname) > weightString(getElementById(users, j)->value->surname); j--)
		{
			list *second = getElementById(users, j-1);
			list *first = getElementById(users, j-2);

			first->next = second->next;
			second->next = first->next->next;
			first->next->next = second;
		}
	}
}
