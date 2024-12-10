/*
 * Michael Hylton.c
 *
 * sync.c
 *
 * Synchronization routines for SThread
 */

#define _REENTRANT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sched.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdbool.h>
#include "sthread_p2.h"
#include "sync_p2.h"

/*
 * Atomic operations for x86 architecture.
 */
static inline int test_and_set_bit(volatile unsigned long *addr)
{
	int oldval;
	__asm__ __volatile__("xchgl %0, %1"
			: "=r"(oldval), "+m"(*(addr))	/* output */
			: "0"(1)						/* input */
			: "memory"	/* clobbered: changing contents of memory */
			);
	return oldval;
}
static inline void clear_bit(volatile unsigned long *addr)
{
	unsigned long oldval;
	__asm__ __volatile__("xchgl %0, %1"
			: "=r"(oldval), "+m"(*(addr))	/* output */
			: "0"(0)						/* input */
			: "memory"	/* clobbered: changing contents of memory */
			);
}

/*
 * rwlock routines
 */

int sthread_rwlock_init(sthread_rwlock_t *rwlock)
{
        printf("\nInitiated");
        rwlock->numOfReaders=0;
        rwlock->numOfWriters=0;
        rwlock->spinLock=0;
        return 0;
}

int sthread_rwlock_destroy(sthread_rwlock_t *rwlock)
{
        printf("\nDestroy");
        return 0;
}

int sthread_read_lock(sthread_rwlock_t *rwlock)
{
        printf("\nRead Lock\n");
        //keeps the thread lock while writing
        while(test_and_set_bit(&rwlock->spinLock)){
          sched_yield();
        }
        if(rwlock->numOfWriters){
            sthread_suspend(); // if the writer is active, it suspeneds
        }
        rwlock->numOfReaders++;
        rwlock->spinLock=0;
        return 0;
}


int sthread_read_unlock(sthread_rwlock_t *rwlock)
{
        printf("\nRead unlock\n");
        while(test_and_set_bit(&rwlock->spinLock)){
          sched_yield();
        }
        rwlock->numOfReaders--;
        //if no readers exist after decrementing, wakeup the next queued writers
        if (rwlock->numOfReaders ==0){ 
            sthread_wake(NULL); 
            
          } 
        rwlock->spinLock=0;
        return 0;
}

int sthread_write_lock(sthread_rwlock_t *rwlock)
{
        printf("\nWrite Lock\n");
        while(test_and_set_bit(&rwlock->spinLock)){
          sched_yield();
        }
        
        if (rwlock->numOfReaders >0 || rwlock->numOfWriters){ 
            sthread_suspend(); // if the writer is active, it suspeneds
            
         }
        rwlock->numOfWriters=1;
        rwlock->spinLock=0;
        return 0;
        
}



int sthread_write_unlock(sthread_rwlock_t *rwlock)
{
        printf("\nWrite unlock\n");
        while(test_and_set_bit(&rwlock->spinLock)){
          sched_yield();
        }
        rwlock->numOfWriters=0;
        //if no readers exist after decrementing, wakeup the next queued writers or queued reader
        sthread_wake(NULL); //then try the writer
        rwlock->spinLock=0;
             
        return 0;
}

int sthread_read_try_lock(sthread_rwlock_t *rwlock)
{
        /* FILL ME IN! */
        return 0;
}
int sthread_write_try_lock(sthread_rwlock_t *rwlock)
{
        /* FILL ME IN! */
        return 0;
}
