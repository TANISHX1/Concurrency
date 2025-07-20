/*simple hash table
 - uses string operations to compare keys during insert
   due to which time complexity increases + due to other factors
 - randomly selects one key (out of 100)each iteration during insert*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define TABLE_MAX 20

char *keys(int index)
{
    char *keys[100] = {
        "192.168.1.1", "192.168.1.2", "192.168.1.3", "192.168.1.4", "192.168.1.5",
        "192.168.1.6", "192.168.1.7", "192.168.1.8", "192.168.1.9", "192.168.1.10",
        "192.168.1.11", "192.168.1.12", "192.168.1.13", "192.168.1.14", "192.168.1.15",
        "192.168.1.16", "192.168.1.17", "192.168.1.18", "192.168.1.19", "192.168.1.20",
        "192.168.1.21", "192.168.1.22", "192.168.1.23", "192.168.1.24", "192.168.1.25",
        "192.168.1.26", "192.168.1.27", "192.168.1.28", "192.168.1.29", "192.168.1.30",
        "192.168.1.31", "192.168.1.32", "192.168.1.33", "192.168.1.34", "192.168.1.35",
        "192.168.1.36", "192.168.1.37", "192.168.1.38", "192.168.1.39", "192.168.1.40",
        "192.168.1.41", "192.168.1.42", "192.168.1.43", "192.168.1.44", "192.168.1.45",
        "192.168.1.46", "192.168.1.47", "192.168.1.48", "192.168.1.49", "192.168.1.50",
        "192.168.1.51", "192.168.1.52", "192.168.1.53", "192.168.1.54", "192.168.1.55",
        "192.168.1.56", "192.168.1.57", "192.168.1.58", "192.168.1.59", "192.168.1.60",
        "192.168.1.61", "192.168.1.62", "192.168.1.63", "192.168.1.64", "192.168.1.65",
        "192.168.1.66", "192.168.1.67", "192.168.1.68", "192.168.1.69", "192.168.1.70",
        "192.168.1.71", "192.168.1.72", "192.168.1.73", "192.168.1.74", "192.168.1.75",
        "192.168.1.76", "192.168.1.77", "192.168.1.78", "192.168.1.79", "192.168.1.80",
        "192.168.1.81", "192.168.1.82", "192.168.1.83", "192.168.1.84", "192.168.1.85",
        "192.168.1.86", "192.168.1.87", "192.168.1.88", "192.168.1.89", "192.168.1.90",
        "192.168.1.91", "192.168.1.92", "192.168.1.93", "192.168.1.94", "192.168.1.95",
        "192.168.1.96", "192.168.1.97", "192.168.1.98", "192.168.1.99", "192.168.1.100"};
    return keys[index];
}
typedef struct node
{
    char *key;
    int value;
    struct node *next;
} node;

typedef struct
{
    node *table[TABLE_MAX];
} Hashtable;
// create node
node *createnode(char *key, int value)
{
    node *new_node = (node *)malloc(sizeof(node));
    new_node->key = strdup(key);
    new_node->value = value;
    new_node->next = NULL;

    return new_node;
}
// initalize Hashtable
Hashtable *creathashtable()
{
    Hashtable *table = (Hashtable *)malloc(sizeof(Hashtable));
    for (int i = 0; i < TABLE_MAX; i++)
    {
        table->table[i] = NULL;
    }
    return table;
}

unsigned int hash(char *key)
{
    unsigned int sum = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        sum += (int)key[i];
    }
    return sum % TABLE_MAX;
}

void insert(Hashtable *table, char *key, int value)
{
    int index = hash(key);
    node *current = table->table[index];
    if (current == NULL)
    {
        table->table[index] = createnode(key, value);
        return;
    }

    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            current->value = value;
            return;
        }
        else if (current->next == NULL)
        {
            break;
        }
        current = current->next;
    }
    current->next = createnode(key, value);
}

node *search(Hashtable *table, char *key)
{
    unsigned int index = hash(key);
    node *current = table->table[index];
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int main()
{
    srand(time(NULL));
    node *pos;
    int randomnum;
    Hashtable *table = creathashtable();
    for (int i = 0; i < 100; i++)
    {
        randomnum = rand() % 99;
        insert(table, keys(randomnum), rand() % (int)2e5);
    }
    for (int i = 0; i < 100; i++)
    {
        pos = search(table, keys(i));
        if (pos == NULL)
        {
            printf("\033[30mNo , entry\033[0m\n");
            continue;
        }
        printf(" \033[32mKey : %-20s | value :%d\033[0m\n", pos->key, pos->value);
    }
}
