// simple & single reversed linked list
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

volatile int node_count =1;
typedef struct _node{
    int node_no;
    int value ;
    struct _node * prev;
}_node;

void addnode(_node **head , _node **temp ,int value){
     _node *new_node = (_node *)malloc(sizeof(_node));
     if(!new_node){
        fprintf(stderr , "Malloc Failed to allocate Memory\n");

     }
     new_node->node_no = node_count++;
     new_node->value =value;
     new_node->prev = *head;
     *head = new_node;
}

void print_node(_node **head , _node **temp ){
    printf("\033[34m======Reversed linked List======\033[0m\n\n");
    while(*head !=NULL){
        (printf("Node_no. : %-5d  Value : %d\n",(*head)->node_no, (*head)->value));
        *temp = (*head)->prev;
        free(*head);
        *head  = *temp;
    }
}

int main(){
    srand(time(NULL));
    _node *head =NULL;
    _node *temp;
    for(int i=0; i<rand()%100;i++){
        addnode(&head,&temp,rand()%(int)2e5);
    }
    print_node(&head, &temp);
    return 0;
}