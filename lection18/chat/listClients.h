/* Description here */

struct clientsList {
  char name[10];
  struct clientsList *next;
};

int push(struct clientsList*, char*);

int removeElemByName(struct clientsList*, char*);

struct clientsList* initList();

int getLenList(struct clientsList*);

char isInList(struct clientsList*, char*);
