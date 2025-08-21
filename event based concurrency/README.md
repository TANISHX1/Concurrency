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

# Drawbacks
1. he event-based approach is that it does not
integrate well with certain kinds of systems activity, such as paging. For
example, if an event-handler page faults, it will block, and thus the server
will not make progress until the page fault completes. Even though the
server has been structured to avoid explicit blocking, this type of implicit
blocking due to page faults is hard to avoid and thus can lead to large
performance problems when prevalent.

2. he event-based approach that we should mention. For example, when systems moved from a single
CPU to multiple CPUs, some of the simplicity of the event-based ap-
proach disappeared. Specifically, in order to utilize more than one CPU,
the event server has to run multiple event handlers in parallel; when do-
ing so, the usual synchronization problems (e.g., critical sections) arise,
and the usual solutions (e.g., locks) must be employed. Thus, on mod-
ern multicore systems, simple event handling without locks is no longer
possible.

3. event-based code can be hard to manage over time,
as the exact semantics of various routines changes. For example,
if a routine changes from non-blocking to blocking, the event handler
that calls that routine must also change to accommodate its new nature,
by ripping itself into two pieces. Because blocking is so disastrous for
event-based servers, a programmer must always be on the lookout for
such changes in the semantics of the APIs each event uses.

4. asynchronous disk I/O is now possible on most plat-
forms, it has taken a long time to get there, and it never quite
integrates with asynchronous network I/O in as simple and uniform a
manner as you might think. For example, while one would simply like
to use the select() interface to manage all outstanding I/Os, usually
some combination of select() for networking and the AIO calls for
disk I/O are required.
