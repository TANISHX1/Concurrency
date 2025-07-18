/*conurent linked list
 # WORKFLOW
  -fetches file data line-by-line (data_fetch) , stores dynamically through (strdup)
  -finds substring (data_find)
  -cleans eap allocated memory
   */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

volatile int count = 1;
typedef struct __node_t
{
    char *line;
    int node_no;
    struct __node_t *next;

} _node_t;

typedef struct _list_e
{
    _node_t *head;
    pthread_mutex_t lock;
    FILE *fptr;
} _list_e;

void init(_list_e *List)
{
    List->head = NULL;
    pthread_mutex_init(&List->lock, NULL);
}

void update_list(_list_e)
{
}
void *data_fetch(void *arg)
{
    _list_e *data_fetch = (_list_e *)arg;

    char buffer[200];
    pthread_mutex_lock(&data_fetch->lock);
    while (fgets(buffer, sizeof(buffer), data_fetch->fptr) != NULL)
    {
        _node_t *new_node = (_node_t *)malloc(sizeof(_node_t));

        new_node->line = strdup(buffer);
        new_node->node_no = count++;
        new_node->next = data_fetch->head;
        data_fetch->head = new_node;
    }
    pthread_mutex_unlock(&data_fetch->lock);

    return NULL;
}
void print(_list_e *fetched_list)
{
    _node_t *pos = fetched_list->head;
    while (pos != NULL)
    {
        printf("%d\t%s", pos->node_no, pos->line);
        pos = pos->next;
    }
}

void *data_find(void *arg)
{
    _list_e *data_fetch = (_list_e *)arg;
    _node_t *pos = data_fetch->head;
    while (pos != NULL)
    {
        if (strstr(pos->line, "Space"))
        {
            printf("node_no : %d\n", pos->node_no);
            printf("%s", pos->line);
        }
        pos = pos->next;
    }
}

void mem_clean(_list_e *fetched_data)
{
    _node_t *pos = fetched_data->head;
    _node_t *temp;
    while (pos != NULL)
    {
        temp = pos->next;
        free(pos->line);
        free(pos);
        pos = temp;
    }
}
int main()
{
    FILE *fptr = fopen("unique_words.txt", "r");
    if (!fptr)
    {
        fprintf(stderr, "[Error] invalide_file\n");
        exit(1);
    }
    _list_e LISt;
    LISt.fptr = fptr;
    init(&LISt);

    pthread_t t1, t2;
    pthread_create(&t1, NULL, data_fetch, &LISt);
    pthread_join(t1, NULL);
    printf("Fetched\n");
    pthread_create(&t2, NULL, data_find, &LISt);
    pthread_join(t2, NULL);
    mem_clean(&LISt);
    printf("Done\n");
    return 0;
}