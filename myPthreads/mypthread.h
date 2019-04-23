#ifndef MYPTHREAD_H
#define MYPTHREAD_H 1

#include <sys/types.h>

// 64kB stack
#define STACK 1024*64

#define MAX_THREADS 1024

/*
 * mypthread struct used to track id and stack 
 */
typedef struct {
    void* stack;
    pid_t pid;
    int tickets;
    int srrValue;

    int priority;
} mypthread_t;

int schedUp;
int schedType;
mypthread_t schedThread;


mypthread_t* queue[MAX_THREADS];
int queuePointer;
int queueSize;

/*
mypthread_t* aQueue[MAX_THREADS];
mypthread_t* bQueue[MAX_THREADS];
int a;
int b;*/

/*
 *  Used to create threads 
 *  return 0 if succesful
 */
extern int pthread_create(mypthread_t*      thread,
                   const pthread_attr_t*      attr,
                   void* (*function)       (void*),
                   void*                       arg);

/**
 *  Waits for the especified thread to end
 */
extern int pthread_join(mypthread_t thread, void **retval);

/**
 *  Detaches thread execution
 */
extern int pthread_detach(mypthread_t thread);

/**
 *  Pthreads tickets
 */
extern void pthread_ticket(mypthread_t* thread, int tickets);

/**
 *  Gives up processor
 */
extern int pthread_yield();

/**
 *  Ends thread
 */
extern void pthread_exit(void* retval);

#define FREE 0
#define LOCKED 1

typedef unsigned int uint;

typedef struct mymutex {
  uint state;
  int ready;
} mymutex_t;

/**
 * creates mutex
 */ 
extern int mymutex_init(mymutex_t *);

/**
 * locks mutex
 */
extern int mymutex_lock(mymutex_t *);

/**
 * try locking mutex
 */
extern int mymutex_trylock(mymutex_t *);

/**
 * unlocks the mutex
 */
extern int mymutex_unlock(mymutex_t *);

/**
 * destroy the mutex
 */
extern int mymutex_destroy(mymutex_t *);

static inline unsigned int xchg(uint *addr, uint newval) {
  uint result;
  asm volatile("lock; xchgl %0, %1" :
               "+m" (*addr), "=a" (result) :
               "1" (newval) :
               "cc");
  return result;
}

#endif