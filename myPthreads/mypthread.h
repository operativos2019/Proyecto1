#ifndef MYPTHREAD_H
#define MYPTHREAD_H 1

#include <sys/types.h>

// 64kB stack
#define STACK 1024*64

/*
 * mypthread struct used to track id and stack 
 */
typedef struct {
    void* stack;
    pid_t pid;
} mypthread_t;

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


#endif