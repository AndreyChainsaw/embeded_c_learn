#define STORE_CELL 5
#define STORE_CAPACITY 1000

#define CUSTOMERS_COUNT 3
#define CUSTOMERS_INIT_DEMAND 10000

#define LOADERS_COUNT 1
#define LOADERS_SUPPLY 1000

void printStoreStatus(void);

void *loadStoreCell(void*);

void *buyFromStore(void *);