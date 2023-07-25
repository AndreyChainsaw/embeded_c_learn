/* Количество магазинов */
#define STORE_CELL 5

/* Начальное количество товаров 
   при запуске в магазине */
#define STORE_CAPACITY 1000

/* Количество покупателей */
#define CUSTOMERS_COUNT 3

/* Начальная потребность покупателей */
#define CUSTOMERS_INIT_DEMAND 10000

/* Количество погрузчиков, которые
   поставляют товары в магазин */
#define LOADERS_COUNT 1

/* Количество поставляемых товаров
   погрузчиком в магазины */
#define LOADERS_SUPPLY 1000

void printStoreStatus(void);

void *loadStoreCell(void*);

void *buyFromStore(void *);
