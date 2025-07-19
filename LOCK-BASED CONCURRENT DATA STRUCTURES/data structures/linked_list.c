// single & simple linked list
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

volatile int node_counter = 1;
typedef struct __node
{
    int node_no;
    int value;
    struct __node *next;
} _node;

void addnode(_node **head, _node **temp, int val)
{
    _node *new_node = (_node *)malloc(sizeof(_node));
    if (!new_node)
    {
        fprintf(stderr, "Malloc Failed to allocate new_node\n");
    }
    new_node->node_no = node_counter++;
    new_node->value = val;
    new_node->next = NULL;
    if(*head==NULL){
        *head =new_node;
        *temp =new_node;
        return;
    }
    
    (*temp)->next = new_node;
    *temp = new_node;
}
void print_list(_node **head)
{
    _node *pos = *head;
    _node *temp;
    printf("\033[34m========= linked List =========\033[0m\n\n");
    while (pos != NULL)
    {
        printf("Node_no : %-5d  Value : %d\n", pos->node_no, pos->value);
        temp = pos->next;
        free(pos);
        pos = temp;
    }
}

int main()
{
    _node *head =NULL ;
    _node *temp;
    srand(time(NULL));
    for (int i = 0; i < rand()%100; i++)
    {
        addnode(&head,&temp,rand()%(int)2e5);
    }
        print_list(&head);

        return 0;
    
}