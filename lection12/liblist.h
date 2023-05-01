typedef struct list
{
	struct abonent *value; //void???
	struct list *next;
}list;

list* init(struct abonent*);
list* addHead(list *, struct abonent*);
list* addElem(list *, struct abonent*); // Добавить после этого элемента
list* delElem(list *node, list *root);
list* delHead(list *root);
list* getElementById(list * root, int number);
void listPrint(list *lst);
