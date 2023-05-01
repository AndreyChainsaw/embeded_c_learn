#include <stdio.h>
#include <stdlib.h>

typedef struct list
{
	struct abonent *value; //void???
	struct list *next;
}list;

list* init(struct abonent*);
list* addHead(list *, struct abonent*);
list* addElem(list *, struct abonent*); // Добавить перед этим элементом
list* delElem(list *node, list *root);
list* delHead(list *root);
list* getElementById(list * root, int number);
void listPrint(list *lst);


list* init(struct abonent *value)
{
	list *node = (list*)malloc(sizeof(list));
	node->value = value;
	node->next= 0;

	return node;
}

list* addHead(list *node, struct abonent *value)
{
	list *tmp = (list*)malloc(sizeof(list));
	tmp->next = node;
	tmp->value = value;

	return tmp;
}

list* addElem(list *node, struct abonent *value)
{
	list *tmp = (list*)malloc(sizeof(list));
	
	tmp->value = value;
	tmp->next = node->next;

	node->next = tmp;

    return node;
}

list* delElem(list *node, list *root)
{
	list *tmp = root;

	while (tmp->next != node)
	{
		tmp = tmp->next;
	}

	tmp->next = node->next;
	free(node);

	return tmp;
}

list* delHead(list *root)
{
	list *tmp = root->next;
	free(root);

	return tmp;
}

void listPrint(list *lst)
{
	list *ptr = lst;
	do
	{
		printf("%d\n", ptr->value);
		ptr = ptr->next;
	} while (ptr != 0);
}


list* getElementById(list * root, int number)
{
	list *ptr = root;
	for (int i = 0; i < number; i++)
	{
		ptr = ptr->next;
	}

	return ptr;
}
