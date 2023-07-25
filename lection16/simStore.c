#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "simStore.h"


// TODO: auto fill array 
int store[STORE_CELL] = {STORE_CAPACITY, STORE_CAPACITY,
                         STORE_CAPACITY, STORE_CAPACITY,
                         STORE_CAPACITY};

pthread_t customers[CUSTOMERS_COUNT];
pthread_t loaders[LOADERS_COUNT];

// TODO: auto fill array
pthread_mutex_t mutexStoreCell[STORE_CELL] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER,
                                              PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER,
                                              PTHREAD_MUTEX_INITIALIZER};


int main(void) {
  printStoreStatus();

  // Создаем поток для погрузчика, чтобы он пополнял магазины
  pthread_create(&loaders[0], NULL, loadStoreCell, NULL);

  // Создаем потоки для покупателей
  for (int i = 0; i < CUSTOMERS_COUNT; i++) {
    pthread_create(&customers[i], NULL, buyFromStore, NULL);
  }

  // Дожидаемся завершения потока покупателей
  for (int i = 0; i < CUSTOMERS_COUNT; i++) {
    pthread_join(customers[i], NULL);
  }

  printStoreStatus();
  return 0;
}

void printStoreStatus(void) {
  for (int i = 0; i < STORE_CELL; i++) {
    printf("store [%d] = %d ", i, store[i]);
  }

  printf("\n\n==STATUS STORE===\n\n");
}


/* Погрузчик пополняет магазины (STORE_CELL) с определенным
   интервалом. Запускается в отдельном потоке. Ищет свободную
   ячейку (магазин), блокирует ее и пополняет. По-умолчанию
   данный поток не переходит в состояние "terminated". Основной
   поток не ждет завершения данного потока. */

void *loadStoreCell(void*) {
  while (1) {
    for (int index = 0; index < STORE_CELL; index++) {
      int statusMutex = pthread_mutex_trylock(&mutexStoreCell[index]);
      if (0 == statusMutex) {
        store[index] += LOADERS_SUPPLY;

        printf("LOADERS ADD %d TO CELL %d. NOW = %d\n", 
          LOADERS_SUPPLY, index, store[index]);

        pthread_mutex_unlock(&mutexStoreCell[index]);
        sleep(3);
        break;
      }
    }
  }

  pthread_exit(0);
}

/* Покупатели с определенным интервалом скупают
   товары из свободного (незаблокированного) магазина (STORE_CELL).
   Занятость того или иного магазина определяется
   мьютексом. Как только покупатель удовлетворит свою
   потребность, поток покупателя завершается */

void *buyFromStore(void *) {
  int customerID = pthread_self();
  int currentDemand = CUSTOMERS_INIT_DEMAND;

  while (currentDemand > 0) {
    for (int i = 0; i < STORE_CELL; i++) {
      int statusMutex = pthread_mutex_trylock(&mutexStoreCell[i]);

      if (0 == statusMutex) {
        if (store[i] > 0) {
          currentDemand -= store[i];
          store[i] = 0;

          printf("customer[%u] buy from store %d. Current demand = %d\n", customerID, i, currentDemand);
          pthread_mutex_unlock(&mutexStoreCell[i]);
          sleep(1);
          break;
        }

        else {
          pthread_mutex_unlock(&mutexStoreCell[i]);
          continue;
        }

      }

    }
  }

  printf("Customer[%u] exit\n", customerID);
  pthread_exit(0);
}
