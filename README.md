# Concurrency
it contains codes related to the  multiprocessing &amp;&amp; multiprogramming , which shows how Concurrency works in UNIX/Linux systems( POSIX Sys)

# Bugs
 ### non-Deadlock bugs
* Condition variables help fix order-violations (e.g., ensuring threads wait for a certain condition  before  proceeding).
* Locking mechanisms like mutexes and semaphores help fix atomicity-violations (e.g., ensuring critical sections are executed atomically).

Both are used to manage concurrency but target different types of bugs.

 ### Deadlock Bugs
 Beyond the concurrency bugs mentioned above, a classic problem that
arises in many concurrent systems with complex locking protocols is known
as deadlock. Deadlock occurs, for example, when a thread (say Thread 1) is holding a lock (L1) and waiting for another one (L2); unfortunately,
the thread (Thread 2) that holds lock L2 is waiting for L1 to be released.
Here is a code snippet that demonstrates such a potential deadlock:
```
Thread 1: 
pthread_mutex_lock(L1); 
pthread_mutex_lock(L2); 
Thread 2:
pthread_mutex_lock(L2);
pthread_mutex_lock(L1);
```
**Why do Deadlock Occur**
1) n large code bases, complex dependencies arise between components. Take the operating system, for example. The virtual memory system might need to access the file system in order to page
in a block from disk; the file system might subsequently require a page of memory to read the block into and thus contact the virtual memory system. Thus, the design of locking strategies in large systems must be
carefully done to avoid deadlock in the case of circular dependencies that may occur naturally in the code.

2) Another reason is due to the nature of encapsulation. As software developers, we are taught to hide details of implementations and thus make software easier to build in a modular way. Unfortunately, such modularity does not mesh well with locking.

**Conditions for Deadlock**
Important: All four conditions must hold simultaneously for deadlock to occur.
* Mutual Exclusion : atleast one resource is non-shareable,means only one thread can hold the lock at a time.
* Hold & Wait : one thread is holding resources (or lock) and Waiting for other requested resource which is held by other thread.
* Non preemption : Resources can not be taken forcefully from the threads ,which acquires them.
* Circular Wait : circular chain (circular list ) or threads holding some resources and waiting for requested resources which is held by other threads of the circular chain. 

**Deadlock Prevention**
* circular wait : To prevent circular wait (and thus deadlock), always acquire locks in a well-defined order. In small systems, a single total order is sufficient; in large or complex systems, a carefully designed partial order is more practical and still safe.
* hold and wait : to avoid this, acquire all locks at once ,
``` 
pthread_mutex_lock(prevention); // begin acquisition
 pthread_mutex_lock(L1);
 pthread_mutex_lock(L2);
 ...
 pthread_mutex_unlock(prevention); // end 
```
* Non preemption :prevention technique is generally not directly implemented. Instead, the pthread_mutex_trylock() function allows a thread to voluntarily release a lock it holds (L1) if it can't acquire another lock (L2).
Here's the breakdown:

1) No True Preemption: Actual preemption would involve the system forcibly taking a lock away from a thread. trylock doesn't do that.
2) Voluntary Release: trylock lets a thread check if a lock is available. If not, the thread can release the locks it does hold and try again later. This avoids deadlock.
3) Livelock Risk: This approach can lead to livelock, where threads repeatedly try and fail to acquire locks. Adding a random delay can help avoid livelock.

**Deadlock avoidance via scheduling**

(assumed there is no concurrency (means no context switching))
instead of prevention  , some time deadlock avoidance (via scheduling )is better , here avoidance means 
avoiding the conditions which could leads to deadlock . example : we have 4 threads(T1,T2,T3,T4) and 2 cores(C1,C2). case 1 , assume that T1 , T2 requires two locks L1,L2 (acquires locks in any order )and T3 requires L2 and T4 requires no locks . so ,to avoid deadlock ,system schedules t1,t2(both are scheduled like non premptive scheduling ) in same core (c1) and t3,t4 in c2 , to avoid deadlock. avoidance of deadlock via scheduling is not a widely-used general-purpose solution.   
