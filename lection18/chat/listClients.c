#include <stdio.h>    /* print possible errors */
#include <stdlib.h>   /* malloc and calloc */
#include <string.h>   /* memcpy and strcmp */
#include "listClients.h"

int push(struct clientsList *head, char *nameUser) {
  
  struct clientsList *newClient = malloc(sizeof(struct clientsList));

  if (newClient == NULL) {
    perror("Malloc error!");
    return 1;
  }

  newClient->next = NULL;
  memcpy(newClient->name, nameUser, strlen(nameUser) + 1);

  struct clientsList *current = head;

  while (current->next != NULL) {
    current = current->next;
  }

  current->next = newClient;

  return 0;
}

int removeElemByName(struct clientsList *head, char *name) {

  struct clientsList *old = head;
  struct clientsList *current = head;

  while (current != NULL) {

    if (strcmp(current->name, name) == 0) {
      old->next = current->next;
      free(current);
      return 0;
    }

    old = current;
    current = current->next;
  
  }

  return 1;
}

struct clientsList* initList() {
  struct clientsList *head = calloc(sizeof(struct clientsList), 1);

  if (head == NULL) {
    perror("calloc error!");
    return NULL;
  }

  return head;
}

/* может потребоваться при отправке сообщений остальным */
int getLenList(struct clientsList *head) {
  return 0;
}

char isInList(struct clientsList *head, char *name) {
  while (head != NULL) {
    if (strcmp(head->name, name) == 0) {
      return 1;
    }

    head = head->next;
  }


  return 0;
}