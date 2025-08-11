# Event Based Concurrency
the problem with threading , first, complexity of threads , like locking order , using conditional variables for waiting and singling mechanism , basically which increaes the complexity and chances of deadlock .
 second ,  lack of scheduling control (You can’t easily influence which thread should run next, how long a thread runs, or how context switching is performed.) , we have little control on schedling the thread(means when to run which thread using singling mechanism).
 
 ### Descriptor
 A data structure that represent a resource like file , socket, devies and provides a way for process to interact with them .
 * In Unix-like systems (and their descendants), a file descriptor is essentially an integer handle that represents an open resource, such as a file, network socket, or pipe. It's not the resource itself but a reference the operating system uses to track interactions. When you open a file or create a socket, the OS assigns a unique descriptor (usually starting from 0 for standard input, 1 for output, and 2 for error).

   ```Why they matter: Descriptors abstract away the complexity of hardware and I/O operations. In an event loop like the one with select(), you use them to monitor for events (e.g., "Is this socket ready to read?"). It's efficient, but let's face it, numbering things from 0 feels like an OS engineer's inside joke on the rest of us.```
   **File Descriptors:**

* Description: For regular files on disk (e.g., reading/writing a text file).
* Example: Opening a file with ```open()``` returns a descriptor you can use with ```read()``` or ```write()```
* In Context: In ```select()```, you'd watch these for read/write events, like checking if a file has new data available.