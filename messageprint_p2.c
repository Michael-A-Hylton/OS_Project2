#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>


//mutex var creation not init
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t CV = PTHREAD_COND_INITIALIZER;


//tracker to keep track of where we are with total number of threads in execution
int tracker = 0;

//tracks total num of threads
int numOfThreads;

//(number_prime_finished)keeps track of the prime thread--> 0 means composite 1 means prime
int prime = 0;


//function used to check if the number is prime or composite
int isPrime(int num){
    //assumes number is not prime
    int checkPrime = 0;
    if(num <= 1){
        return 0;
    }
    if(num == 2){
        return 1;
    }
    for(int b = 2; b < num; b++){
        if(num % b == 0){
            checkPrime = 0;
            break;
        }
        else{
            checkPrime = 1;
        };
    }
    return checkPrime;
}


//routine for thread creation
void* routine(void* arg){
    int ptid = *((int*)arg);
    //casting manually assigned threadID number ot thread number
    pthread_mutex_lock(&lock);
    //calls function to check if thread is prime or composite
    prime = isPrime(ptid);
    //getting threadID will work off of this to process composite and prime threadID
    if(prime == 0 && tracker == numOfThreads){
        printf("This is a new thread processing, threadID: %d\n", ptid);
        tracker++;
        pthread_mutex_unlock(&lock);
        pthread_cond_broadcast(&CV);
    }
    else if(prime == 1 && tracker == numOfThreads){
        printf("This is a new thread processing, threadID: %d\n", ptid);
        tracker++;
        pthread_mutex_unlock(&lock);
        pthread_cond_broadcast(&CV);
    }
    else if(prime == 1){
        printf("This is a new thread processing, threadID: %d\n", ptid);
        tracker++;
        pthread_mutex_unlock(&lock);
    }
    else{
        while(1){

            tracker++;
            printf("waiting for all prime threads to process. Thread ID: %d\n", ptid);
            pthread_cond_wait(&CV, &lock);
            printf("Thread has been broadcasted, here is the composite thread being processed: %d\n", ptid);
            pthread_mutex_unlock(&lock);
            return 0;
        }
    }
    return 0;
}



int main(int argc, char* argv[]){
    //variable to track thread
    int i;

    //prompting and reading integer from console
    printf("Enter desired number of threads:\n");
    scanf("%d", &i);

    pthread_t thread[i]; //creating threads


    numOfThreads = i-1; //number of threads to execute

    int* threadIDs = (int*)malloc(i * sizeof(int)); //dynamically allocating space for threads

    for(int x = 0; x < i; x++){
        threadIDs[x] = x;
        if(pthread_create((&thread[x]), NULL, &routine, &threadIDs[x]) != 0){
            return 1;
        }
    }
    for(int x = 0; x < i; x++){
        if(pthread_join(thread[x], NULL) != 0){
            return 2;
        }
    }
    //destroying mutex
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&CV);
    printf("Threads executed: %d\n", tracker);
    return 0;
}


