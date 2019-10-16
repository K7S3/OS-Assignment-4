
# README

## 1. Problem 1

### Normal Quick Sort 

* The normal way of coding mergesort.
```
gcc q1.c
./a.out
```
### Concurrent Quick Sort Using Processes 

* A shared memory space is created between the child processes forked. We need a shared memory space because the child and the parent process run in separate memory spaces. The memory writes by one of the process will not change in the other and hence we need a shared memory segment.
* The Left and Right part of the array with the pivot is sorted concurrently(simultaneously).
* Uses shmget for shared memory allocation.shmget() requests the kernel to allocate a shared memory for both the processes.
* Uses shmat for shared memory operations. shmat() attaches the systems V shared memory segment identified by a shmid to the address space of the calling process.

```
gcc q1.c
./a.out
```
### Concurrent Quick Sort Using Threads

* Threads are created and joined for both the left and the right array. 
* Threads lie within the sam process and there is no need for shared memory using shmget/shmat.

```
gcc q1_thread.c -lpthread
./a.out
```

### Performance Analysis
* The time is calculated using the clockgettime() function for 10000 size. 

Method | Time Taken
--------|-----------
Vanilla Quick Sort | 0.004829
Using Processes | 0.550925
Using Threads |  0.169713


* Vanilla Quick Sort < Using Threads < Using Processes

#### Reason Why Using Vanilla Quick Sort < Using Threads

* There is minimal workload on each thread that is sorting a few thousand numbers. 
* But for doing this minimal task, the thread creads multiple other thread each time.
* This causes a overhead due to thread creation.(Thread creation is expensive).
* To solve this problem, we need to ensure that each thread has sufficient amount of work to do.
* We need to limit the number of threads created since when it increases a certain value(#cores), we get less benefits of parallel processing.

#### Reason Why Using Threads < Using Processes

* Threads are used for small tasks and sorting being a small task performs better, whereas processes are used for more 'heavyweight' tasks.
* Threads takes less time to terminate when compared to process.
* The threads are much more efficient in terms of communication.

#### Reason Why Vanilla Quick Sort < Using Processes

* Using processes takes more time than vanilla quick sort since when one child, say left child, accesses the left array, the array gets loaded into the cache of the processor. 
* When the right array is accessed due to context switching, a cache miss occurs since the cache is already filled with the left segment ans then the right segment is copied into the cache. 
* The to and fro process created due to context switching drastically degrades the performance so that it performs poorer than the Vanilla Quick Sort.

## 3. Ober Cab Services

### TO RUN

```
gcc q3.c -lpthread
./a.out
```
### Implementation

* Threads are created for each of the Riders and Servers
* Mutex is used to have mutual exclusion between multiple threads so that one thread doesn't have overlap with the other thread in the critical region(shared memory). This is done using the pthreadmutex_lock and pthreadmutex_unlock functions.
* The Max Wait time for each rider is a random value b/w 0 and 5. Similarly the ride time for each rider is b/w 0 and 10.
* The Rider Thread Function (Function called when thread is created) calls the bookcab function for each rider. THe bookcab function assigns a cab number to the rider based on their choice. This is done with an additional helper function acceptRide Mutex is used to prevent other threads interfering with the current thread's data.
* The Payment Thread Function for each server checks the number of riders left and awaits payment from the rider. The rider makes payment after the ride and this payment is initialized and then for the server to process it takes 2 seconds. For our small system, this is artificially simulated using sleep() function.


### Assumptions

* Just after the rider reaches destination he iniitates payment to the server but server takes 2s to process.
* Payment Completion Time = 2s
* Max Wait time = rand() % 5 
* Ride time = rand() % 10 denotes the time the rider takes to reach destination
* Initially no one is already in the RIDING STATE. That is we book cabs for all the riders. No one is prebooked at the start.
* Initially all servers are free and are ready for request.
* A single cab can be in both Premier and Pool modes. Just as it was mentioned in the pdf.
* The cab goes to WAITSTATE after completion of the ride similar to the server which goes to free state after processing the request

#### States of Cab : 

* waitState ​ (no rider in cab)
* onRidePremier​
* onRidePoolFull ​ (pool ride with two riders)
* onRidePoolOne​ (pool ride with only one rider).

### SAMPLE OUTPUT

Enter the Number of Riders: 10 
Enter the Number of Cabs: 10
Enter the Number of Servers: 10
Rider 1 wants to book a Premier cab. 
Rider 3 wants to book a Premier cab. 
Rider 3 books cab 0 succesfully.
Rider 1 books cab 1 succesfully.
Rider 0 wants to book a Pool cab. 
Rider 0 books cab 2 succesfully.
Rider 2 wants to book a Premier cab. 
Rider 2 books cab 2 succesfully.
Rider 5 wants to book a Pool cab. 
Rider 5 books cab 2 succesfully.
Rider 6 wants to book a Pool cab. 
TimeOut for Rider 6
Rider 8 wants to book a Pool cab. 
Rider 7 wants to book a Premier cab. 
Rider 7 books cab 3 succesfully.
Rider 9 wants to book a Premier cab. 
Rider 9 books cab 4 succesfully.
Ride is over for rider id 9
Payment intiated by rider 9 to server 0
Rider 8 books cab 3 succesfully.
Rider 4 wants to book a Premier cab. 
Rider 4 books cab 5 succesfully.
Ride is over for rider id 7
Payment intiated by rider 7 to server 1
Ride is over for rider id 3
Payment intiated by rider 3 to server 2
Payment completed by rider 9 to server 0
Payment completed by rider 7 to server 1
Payment completed by rider 3 to server 2
Ride is over for rider id 1
Payment intiated by rider 1 to server 0
Ride is over for rider id 2
Payment intiated by rider 2 to server 1
Ride is over for rider id 0
Payment intiated by rider 0 to server 2
Ride is over for rider id 5
Payment intiated by rider 5 to server 3
Payment completed by rider 1 to server 0
Ride is over for rider id 4
Payment intiated by rider 4 to server 0
Payment completed by rider 2 to server 1
Payment completed by rider 0 to server 2
Payment completed by rider 5 to server 3
Ride is over for rider id 8
Payment intiated by rider 8 to server 1
Payment completed by rider 4 to server 0
Payment completed by rider 8 to server 1
