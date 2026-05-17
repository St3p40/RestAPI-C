#include "data_store.h"

static Item items[MAX_ITEMS];
static int item_count = 0;

int create_item(const char *name, const char *description, float price)
{
    if (item_count >= MAX_ITEMS)
    {
        return -1;
    }

    Item new_item;
    new_item.id = item_count + 1;
    strncpy(new_item.name, name, sizeof(new_item.name));
    strncpy(new_item.description, description, sizeof(new_item.description));
    new_item.price = price;

    items[item_count++] = new_item;
    return new_item.id;
}

Item *get_item(int id)
{
    for (int i = 0; i < item_count; i++)
    {
        if (items[i].id == id)
        {
            return &items[i];
        }
    }
    return NULL;
}

int update_item(int id, const char *name, const char *description, float price)
{
    Item *item = get_item(id);
    if (item)
    {
        strncpy(item->name, name, sizeof(item->name));
        strncpy(item->description, description, sizeof(item->description));
        item->price = price;
        return 0;
    }
    return -1;
}

int delete_item(int id)
{
    for (int i = 0; i < item_count; i++)
    {
        if (items[i].id == id)
        {
            for (int j = i; j < item_count - 1; j++)
            {
                items[j] = items[j + 1];
                items[j].id -= 1;
            }
            item_count--;
            return 0;
        }
    }
    return -1;
}

int get_item_count()
{
    return item_count;
}
