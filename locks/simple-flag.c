/*simple-flag lock (Faulty approuch) 
 multiple threads might interact with the flag simultaneously
This simple locking mechanism using a flag fails because 
it doesn't guarantee mutual exclusion and is inefficient, especially on single-core systems.
--non-atomic operations
--wastes pu cycles (spin wating)*/
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#define LOOP_LIMIT 1e5

typedef struct __lock_t
{
    bool flag;
}_lock_t;
//global variable
volatile int counter=0;

void init (_lock_t *mutex){
    //1 -- lock aquired, 0 -- not aquired
    mutex->flag =0;
  
}
void lock (_lock_t *mutex){
    // spin-waiting , if lock aquired by another thread
    while(mutex->flag ==1); 
    mutex->flag =1;
  
}
void unlock (_lock_t *mutex){
    mutex->flag =0;
   
}
void * mythread(void *arg){
    _lock_t *ptr = (_lock_t*)arg;
    for(int i=0;i<LOOP_LIMIT;i++){
        lock(&(*ptr));
        counter++;
        unlock(&(*ptr));
    }
    return NULL;
}

int main (){
    _lock_t tala;
    pthread_t t1,t2;
    init(&tala);
    pthread_create(&t1,NULL,mythread,&tala);
    pthread_create(&t2,NULL,mythread,&tala);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    double data_race;
    char *race = (counter<2*LOOP_LIMIT?"Data Race":"By-luck (No Data Race happnes)");
    data_race = (1 -((float)counter/(2*LOOP_LIMIT)))*100;
    printf("counter : %d\nResult: %s\nData Race(percentage) : %.3f\n",counter,race,data_race);
  //  free(lock);
    return 0;
}
