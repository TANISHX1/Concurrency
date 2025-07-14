/*simple TAS(test-and-set)  approach
 -- not atomic 
 -- stucks when race condition (data race) occurs
 -- indeterminate result */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct __lock_tt
{
    int flag;
}_lock_t;

volatile int counter=0;
int testandset(int *old_ptr , int new_value){
   int old = *old_ptr;
   *old_ptr = new_value;
   return old;
}
void init(_lock_t *mutex){
    mutex->flag =0;
}
void lock(_lock_t *mutex){
    while(testandset(&mutex->flag,1) ==1);
}
void unlock(_lock_t *mutex){
    mutex->flag =0;
}

void * mythread(void *arg){
    _lock_t *ptr = (_lock_t*)arg;
    for(int i=0;i<1e5;i++){
        lock(ptr);
        counter++;
        unlock(ptr);
    }
    printf("%p\t%p\t%p\n",ptr,&ptr,&(*ptr));
}

int main(){
    pthread_t t1, t2;
    _lock_t lock;
    init(&lock);
    pthread_create(&t1,NULL,mythread,&lock);
    pthread_create(&t2,NULL,mythread,&lock);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    printf("Counter : %d\n",counter);
    return 0;

}