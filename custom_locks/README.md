## Custom Locks
 custom created locks using atomic operations , systemcalls and minimal kernal support

# linux_futex
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
     - Atomically adds `0x80000000` (bit 31) to clear lock bit.
      - If result is zero, no waiters, simply returns (fast path).
      - Otherwise, calls futex wake syscall to unblock one waiter thread.

 4. Futex syscalls:
    - `FUTEX_WAIT`: puts thread to kernel sleep if mutex value matches expected.
    - `FUTEX_WAKE`: wakes one waiting thread to retry acquiring the lock.

 5. The combination of atomic operations and futex system calls provides highly efficient locking:
     - Low overhead when uncontended (mostly user space).
     - Minimal CPU wasted on busy waiting by sleeping when locked.
