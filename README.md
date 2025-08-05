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