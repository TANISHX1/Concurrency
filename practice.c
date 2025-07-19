#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct node{
    int num;
    struct node *prev;
    struct node *next;
}node;

int value=0;
void addnode( node *data,int total_node){
    node*tail =NULL;
    node *head=NULL ;
    node*temp;
    temp = head;
    for(int i=0;i<total_node;i++){
        node *new_node = (node*)malloc(sizeof(node));
        new_node->num = value++;
        //from head<-tail
        new_node->prev=tail;
        tail = new_node;
        
        //from head->tail
        new_node->next =NULL;  
        if(head == NULL){
            head = new_node;
            temp = new_node;
        }
        else{
            temp->next = new_node;
            temp = new_node;
        }
    }
    printnode(data,head,tail);
}

void printnode(node*data , node *head ,node*tail ){
    node *pos = tail;
    node *temp;
    while(pos != NULL){
        printf("%d\t",pos->num);
        temp = pos->prev;
        pos = temp;
    }
    printf("\n");
    pos = head;
    while (pos !=NULL){
        printf("%d\t",pos->num);
        temp = pos->next;
        free(pos);
        pos =temp;
    }
}

int main(){
    node data;
    addnode(&data,5);
    return 0;
} 