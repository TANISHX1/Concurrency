/*
   implementation of futex-based mutex lock with efficient userspace fast path and kernel-assisted blocking.
   
  Key points:
  1. The `mutex` is an atomic integer where:
     - Bit 31 (highest bit) is the lock bit (1 = locked, 0 = unlocked).
     - Lower 31 bits count the number of waiting threads (waiters).
 
  2. lock():
     - Attempts to acquire the lock by atomically setting bit 31.
       - If lock is free (bit 31 clear), it sets bit 31 and returns immediately (fast path).
       - Otherwise, increments waiter count to signal contention.
     - In contention:
      - Loops trying to acquire lock by atomically setting bit 31 again.
      - If fails, reads mutex value:
          - If lock freed but not acquired, spins (busy wait).
          - If locked, calls futex syscall to sleep efficiently.

 3. unlock():
     - Atomically adds 0x80000000 (bit 31) to clear lock bit.
      - If result is zero, no waiters, simply returns (fast path).
      - Otherwise, calls futex wake syscall to unblock one waiter thread.

 4. Futex syscalls:
    - `FUTEX_WAIT`: puts thread to kernel sleep if mutex value matches expected.
    - `FUTEX_WAKE`: wakes one waiting thread to retry acquiring the lock.

 5. The combination of atomic operations and futex system calls provides highly efficient locking:
     - Low overhead when uncontended (mostly user space).
     - Minimal CPU wasted on busy waiting by sleeping when locked.

 */


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
    void even_odd (bool value){
        // 0--even  1--odd
        if(!value)
        {
            if(counter%2 ==0)
            even_multiple++;
        }
        else{
            odd_multiple++;
        }
    }
    void *mythread(void *arg){
        bool *value = (bool*)arg;
        for(int i=0;i<1e4;i++){
            lock(&mutex);
             even_odd(*value);
             counter++;
            unlock(&mutex);
        }
       
         return NULL;
    }
int main(){
    bool even =false;
    bool odd  =true;
    pthread_t t1,t2;
    pthread_create(&t1,NULL,mythread,&even);
    pthread_create(&t2,NULL,mythread,&odd);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    printf("Counter : %d\n odd multiple :%d\n Evven multiple :%d\n",counter,odd_multiple,even_multiple);
    return 0;
}