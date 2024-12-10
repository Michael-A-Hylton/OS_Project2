Steve Charles 2850400 50%
Michael Hylton 2788154 50%


part 1:

Design ideaas:
  -Whenever a program calls a lock or unlock function, it will initiate the spinlock until that process is finished
  -A while loop with sched_yield() inside will force it to yield as long as the spinlock is set, and it will suspend if there are any readers or writers
  -The unlock function will wake the next function once there are no readers.

Code Logic:
  -The variables used throughout are defined in sync.h and called using rwlock->{variable_name}
  -The variables are first initiated where the number of readers and writers are set to 0, as well as the spinlock.
  -upon entering any lock or unlock funtion, have the while loop with sched_yield(). But the address of the spinlock is pass to test_and_set_bit which will set it to 1 until it changes, trapping the thread in the loop until the process is finsihed.
  -To unlock, the number of readers is decremented or the number of writers is set to 0

Compilation Instructions:
  - type "make test" into the console.
  - type ./test
  - alternatively, you can type gcc -m32 -o test sthread.c sync.ctest.c -lpthread.

Output:
  Initiated
Read Lock
thread20 ===> Reader thread iteration 0, balance = 10000

Read unlock

Write Lock

Write Lock
Write Lock

Read Lock

Write Lock

Write Lock

Read Lock

Write Lock

Write Lock

Read Lock

Write Lock

Write Lock

Read Lock

Write Lock

Write Lock
Write Lock

Read Lock
R
Write Lock
Read Lock


ead Lock
Write Lock



rite Lock
Write Lock


WWrite Lock
Write Lock

Write Lock
Write Lock

Read Lock
RWrite Lock
Write Lock
Write Lock

Read Lock
Read Lock




Write Lock
Write Lock
Write Lock

Read Lock
Read Lock

Read Lock


Write Lock
the balnce is 10000.

part 2: 

Design ideaas:
  -The program prompts the user to input the number of threads they want to process. The method takes any amount of threads 
  -The idea of the program is to accept user input and pass each thread a thread id that is assinged through the for loop. 
  -These threads all call routine where a isPrime function checks if the number is either prime or composite. 
  -Once this is determined the the threads execute through a series of for loops determining when the prime threads are ready for execution
  -Once all prime threads have executed all composite threads will execute and the program will terminate.

Code Logic
  -pthreads library contains functions such as mutex lock and unlock as well as wait and broadcast function. 
  -The lock is set at the beginning of the if statements and waits for one of the if statements to process before unlocking and allowing another thread to edit the global tracker variable. This functionality is used to prevent race conditions
  -The tracker variable is used to track how many threads have been processed and allows for the program to determine when to start executing the composite threads
  -the pthread_cond_wait(&CV, &lock); is used to stop composite threads from executing as the point of the program is to process all prime threads first. This function puts the thread to sleeps and holds it in the current state and waits for pthread_cond_broadcast(&CV); to signal the threads to pick up and execute. 
  -Once these composite threads have executed the program will terminate.


Compilation instructions:
  -type make run in the command line. This will compile the file
  -Now you can use ./run to run the program
  -The program should prompt you to enter the desired number of threads for input. 

                -$Enter desired number of threads:
                  20 <----- This is where you are prompted to enter a number. Once you have entered the desired number make sure to press enter.
  -OUTPUT: 
waiting for all prime threads to process. Thread ID: 0
waiting for all prime threads to process. Thread ID: 1
This is a new thread processing, threadID: 2
This is a new thread processing, threadID: 3
waiting for all prime threads to process. Thread ID: 4
This is a new thread processing, threadID: 5
waiting for all prime threads to process. Thread ID: 6
This is a new thread processing, threadID: 7
waiting for all prime threads to process. Thread ID: 8
waiting for all prime threads to process. Thread ID: 9
waiting for all prime threads to process. Thread ID: 10
This is a new thread processing, threadID: 11
waiting for all prime threads to process. Thread ID: 12
This is a new thread processing, threadID: 13
waiting for all prime threads to process. Thread ID: 14
waiting for all prime threads to process. Thread ID: 15
waiting for all prime threads to process. Thread ID: 16
This is a new thread processing, threadID: 17
waiting for all prime threads to process. Thread ID: 18
This is a new thread processing, threadID: 19
Thread has been broadcasted, here is the composite thread being processed: 1
Thread has been broadcasted, here is the composite thread being processed: 8
Thread has been broadcasted, here is the composite thread being processed: 12
Thread has been broadcasted, here is the composite thread being processed: 15
Thread has been broadcasted, here is the composite thread being processed: 6
Thread has been broadcasted, here is the composite thread being processed: 10
Thread has been broadcasted, here is the composite thread being processed: 0
Thread has been broadcasted, here is the composite thread being processed: 16
Thread has been broadcasted, here is the composite thread being processed: 18
Thread has been broadcasted, here is the composite thread being processed: 14
Thread has been broadcasted, here is the composite thread being processed: 4
Thread has been broadcasted, here is the composite thread being processed: 9
Threads executed: 20

-Each thread is producing at max 2 lines of out put. Most are just giving one. 



