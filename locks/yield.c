/*pesudeocode for hardware based lock 
 this C code gives 
 --indeterminate output
 --not-atomic
 --stucks when race condition occurs(generally in test_and_set function (because it should be atomic))
 --inplementation of hardware based lock(only shows the basic implementation not a real lock ,neither works perfectly )*/
#include <stdio.h>
#include <pthread.h>
typedef struct _lock_t{
    int flag ;
}_lock_t;

volatile int counter=0;

int test_and_set(int *old_num ,int new){

    int old = *old_num;
    *old_num = new;

    return old;
}
void init (_lock_t *mutex){
    mutex->flag =0;
}
void lock(_lock_t*mutex){
    while(test_and_set(&mutex->flag,1) ==1)
    sched_yield();
}
void unlock(_lock_t *mutex){
    mutex->flag =0;
}
_lock_t lockv;

void *mythread(void *arg){
    for(int i=0;i<1e4;i++){
        lock(&lockv);
        counter++;
        unlock(&lockv);
    }
}

int main(){
    init(&lockv);
    pthread_t t1, t2;
    pthread_create(&t1,NULL,mythread,NULL);
    pthread_create(&t2,NULL,mythread,NULL);
    printf("Counter[1]: %d\n",counter);

    pthread_join(t1,NULL);
    printf("Counter[2]: %d\n",counter);

    pthread_join(t2,NULL);
    printf("Counter[3]: %d\n",counter);
    return 0;
}