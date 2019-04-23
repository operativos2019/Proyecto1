#define _GNU_SOURCE

#include <malloc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mypthread.h"

/**
 *  Scheduler
 */
void* scheduler( void* argument ) {

  queuePointer = 1;

  int ticketsCount;
  int ticketWinner;
  mypthread_t* winnerThread;

  while(1) {
    if (queueSize > 1) {
      if( queuePointer >= queueSize ) {
        queuePointer = 1;
      }

      else if(schedType == 1) {

      }

      else if(schedType == 2) {
        
        //count tickets
        ticketsCount = 0;

        for(int i = 1; i < queueSize; i++) {
          ticketsCount += queue[i]->tickets;
        }

        ticketWinner = ticketsCount * (double)rand() / (double)RAND_MAX;

        printf("%d\n", ticketWinner);

        ticketsCount = 0;

        for(int i = 1; i < queueSize; i++) {
          if(ticketsCount >= ticketWinner){
            winnerThread = queue[i];
            break;
          }
          ticketsCount += queue[i]->tickets;
        }

        kill(winnerThread->pid, SIGCONT);
        usleep(100);
        kill(winnerThread->pid, SIGSTOP);
            
      }
      else {

      kill(queue[queuePointer]->pid, SIGCONT);
      usleep(100);
      kill(queue[queuePointer]->pid, SIGSTOP);
      queuePointer++;
      }

    }  
  }

  return 0;
}

/*
 * Used to pass arguments to clone to comply with its signature   
 */
struct ThreadArguments {
	void* (*function)(void*);
  void*               args;    
};

/**
 *  Pthreads tickets
 */
void pthread_ticket(mypthread_t* thread, int tickets){
  thread->tickets = tickets;
}

/*
 * Exists to give the proper function type to clone.
 */ 
static int threadStart( void* arg ) {
	struct ThreadArguments* arguments = (struct ThreadArguments*) arg;
  arguments->function(arguments->args);
  free( arg );
  free(arguments);
	return 0;
}

/*
 *  Used to create threads 
 *  return 0 if succesful
 */
int pthread_create(mypthread_t*             thread,
                   const pthread_attr_t*      attr,
                   void* (*function)       (void*),
                   void*                       arg) {

  schedType = 2; //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  if( !schedUp ) {
    printf("sched created\n");
    schedUp  = 1;

    //init queues
    queueSize = 0;

    pthread_create(&schedThread, NULL, scheduler, NULL); 
    kill(schedThread.pid, SIGCONT);
  }

  void* stack;
        
  // Allocate the stack
  stack = malloc( STACK );
  if ( stack == 0 ) {
    printf( "mypthread error: Unable to alocate thread stack\n" );
    return -1;
  }

  thread->stack = stack;
  
  /* Create the arguments structure. */
  struct ThreadArguments* arguments = NULL;     
	arguments = (struct ThreadArguments*) malloc( sizeof(*arguments) );
    
	if ( arguments == 0 ) {
		free( thread->stack );
		printf( "mypthread error: Unable to allocate thread arguments.\n" );
	    return -2;
	}

	arguments->function = function; 
  arguments->args = arg;         

  //defining thread scheduling params
  thread->tickets = 1;
       
  // Call the clone system call to create the child thread
  thread->pid = clone(&threadStart,
                      (char*) stack + STACK,
                      //SIGCHLD | CLONE_VM,
                      SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM , 
                      arguments );

  if ( thread->pid == 0 ) {
    printf( "mypthread error: Unable to create thread\n" );
    return 1;
  }

  kill(thread->pid, SIGSTOP); 

  //insert thread in queue
  queue[queueSize] = thread;
  queueSize++;

  return 0;            

}


/**
 *  Waits for the especified thread to end
 */
int pthread_join(mypthread_t thread, void **retval) {

  printf("Waiting for proccess to end \n");

  pid_t pid = waitpid( thread.pid, 0, 0 );
  if ( pid == -1 ) {
    perror( "waitpid\n" );
    exit( 3 );
  }
        
  // Free the stack
  free( thread.stack );

  return pid;

}

/**
 *  Detaches thread execution
 */
int pthread_detach(mypthread_t thread) {
    kill(thread.pid, SIGCONT);
    return 0;
}

/**
 *  Gives up processor
 */
int pthread_yield() {
	sched_yield();
    return 0;
}

/**
 *  Gives up processor
 */
void pthread_exit(void* retval) {
  exit(retval);
}

/**
 * creates mutex
 */ 
int mymutex_init(mymutex_t *mutex) {
  if (mutex->ready) {
    return -1;
  }

  mutex->ready = 1;
  mutex->state = FREE;
  return 0;
}

/**
 * locks mutex
 */
int mymutex_lock(mymutex_t *mutex) {
  if (mutex->ready) {
    while (xchg(&mutex->state, 1) != 0) ;
    return 0;
  }
  return -1;
}

/**
 * try locking mutex
 */
int mymutex_trylock(mymutex_t *mutex) {
  if (mutex->ready)
    return xchg(&mutex->state, 1);
  return -1;
}

/**
 * unlocks the mutex
 */
int mymutex_unlock(mymutex_t *mutex) {
  if (mutex->ready)
    return !xchg(&mutex->state, 0);
  return -1;
}

/**
 * destroy the mutex
 */
int mymutex_destroy(mymutex_t *mutex) {
  if (mutex->ready) {
    mutex->ready = 0;
    return 0;
  }
  return -1;
}
