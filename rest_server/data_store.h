#ifndef DATA_STORE_H
#define DATA_STORE_H

#define MAX_ITEMS 100

#include "http_server.h"

typedef struct
{
    int id;
    char name[256];
    char description[1024];
    float price;
} Item;

int create_item(const char *name, const char *description, float price);
Item *get_item(int id);
int update_item(int id, const char *name, const char *description, float price);
int delete_item(int id);
int get_item_count();

#endif
