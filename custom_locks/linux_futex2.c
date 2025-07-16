#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdatomic.h>
#include <linux/futex.h>
#include <string.h>
#include <time.h>

typedef struct _line {
    int value;
    char *one_line;
    struct _line *next;
}_line;
 
 _Atomic int mutex=0;
short int value=1;


void lock(_Atomic int *mutex){
    int ov;
    //[Attempt 1]:fast path :immediate lock acquire
    if((atomic_fetch_or(mutex,1<<31) & 1<<3) ==0){
        return ;
    }
    //waiters increments
    atomic_fetch_add(mutex,1);
    while(1){
        //[Attempt 2]:Re-attempt 
        if((atomic_fetch_or(mutex,1<<31) & 1<<31) ==0){
            atomic_fetch_sub(mutex,1);
            return ;
        }
         ov =atomic_load(mutex);
         //[Attempt 3]: lock is free race to acquire lock 
         if(ov>=0){
            continue;
         }
         syscall(SYS_futex,mutex,FUTEX_WAIT,ov,NULL,NULL,0);
    }
}

void unlock (_Atomic int *mutex){
    if((atomic_fetch_add(mutex,0x80000000) + 0x80000000) == 0){
        return ;
    }
    syscall(SYS_futex,mutex,FUTEX_WAKE,1,NULL,NULL,0);
}
 void *data_fetch(void *arg){
    FILE *fptr;
    char buffer[1024];
    fptr = fopen("input.txt","r");
    if(fptr ==NULL){
        fprintf(stderr,"[Error] invalid_file_or_file_not_exist_check_path\n");
        exit(1);
    }
    while(fgets(buffer,sizeof(buffer),fptr) != NULL){
        int size = strlen(buffer)+1;
        char *new_line  = (char*)malloc(size*sizeof(char));
        strcpy(new_line,buffer);
        put_line(new_line,false,NULL,false);
    }
    fclose(fptr);
   return NULL;
 }
 _line *head =NULL;
void put_line(char *buffer,bool valid , int rvalue , bool clear){
    if(!valid)
     {
        _line *entry = (_line*)malloc(sizeof(_line));
       entry->one_line = buffer;
       entry->value = value++;
       entry->next = head;
       head = entry;
    }
    if(valid){
        _line *pot = head;
        while(pot->next!=NULL){
            if(pot->value ==rvalue ){
                printf("%s",pot->one_line); 
                break;
            }
            
            else {
                _line *temp = pot->next;
                pot =temp;
            }            
        }
    }
    if(clear){
        if(head == NULL){
            return;
        }
        _line*pot =head;
        _line*temp ;
        while(pot!=NULL){
           temp = pot->next;
           free(pot->one_line);
           free(pot);
           pot =temp;
        }
    }
}
    void *line_selector(void *arg){
        // for(int i =0;i<10;i++){
            lock(&mutex);
            int randomnum = rand()%200;
            put_line(NULL,true,randomnum,false);
            unlock(&mutex);
        // }
        return NULL;
    }

int main(){
    srand(time(NULL));
    pthread_t t1,t2,t3;
    pthread_create(&t1,NULL,data_fetch,NULL);
    pthread_join(t1,NULL);
    pthread_create(&t2,NULL,line_selector,NULL);
    pthread_create(&t3,NULL,line_selector,NULL);
    pthread_join(t2,NULL);
    pthread_join(t3,NULL);
    put_line(NULL,NULL,NULL,true);
    printf("ok\n");
}