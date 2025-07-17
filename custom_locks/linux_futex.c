
#include <stdio.h>
#include <pthread.h>
#include <linux/futex.h>
#include <stdatomic.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdbool.h>

 _Atomic int mutex =0;
  volatile int counter =0;
int even_multiple=0;
int odd_multiple=0;
void lock(_Atomic int *mutex){
    int v;
    //atomically test and set 
    //[Attempt 1 ]:if lock is free ,aquires lock immediately
    if((atomic_fetch_or(mutex,(1<<31)) & (1<<31)) ==0){
         return; //fast path:  lock aquired 
        }
        //atomically increments the waiters
        //if failed in [attempt 1] add itself in waiters
        atomic_fetch_add(mutex,1);

        while(1){
            //again atomically check the lock(by checking 31st bit)
            //[Attemp 2 ]:trys again 
        if((atomic_fetch_or(mutex,(1<<31)) & (1<<31)) ==0){
            //atomically decrements the waiter and lock aquired
            atomic_fetch_sub(mutex,1);
            return;
        }
        //atomically loads the value
        v = atomic_load(mutex);
        //spins if lock is free but we lost the race (race condition)
        
        if(v>=0)
        //tries again in next iteration
        continue; 
        //sleep in kernal queue if lock is not free
        
        syscall(SYS_futex,mutex,FUTEX_WAIT,v,NULL,NULL,0);
        }
  
    }
    void unlock (_Atomic int *mutex){
        if(atomic_fetch_add(mutex,0x80000000) + 0x80000000 == 0){
            return ;
        }

        syscall(SYS_futex,mutex,FUTEX_WAKE,1,NULL,NULL,0);
    }
    void even_odd (){

            if(counter%2 ==0)
            even_multiple++;
        
        else{
            odd_multiple++;
        }
    }
    void *mythread(void *arg){
       
        for(int i=0;i<1e4;i++){
            lock(&mutex);
             even_odd();
             counter++;
            unlock(&mutex);
        }
       
         return NULL;
    }
int main(){

    pthread_t t1,t2;
    pthread_create(&t1,NULL,mythread,NULL);
    pthread_create(&t2,NULL,mythread,NULL);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    printf("Counter : %d\n odd multiple :%d\n Evven multiple :%d\n",counter,odd_multiple,even_multiple);
    return 0;
}
