#define _GNU_SOURCE

#include <malloc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>

#include "mypthread.h"


/*
 * Used to pass arguments to clone to comply with its signature   
 */
struct ThreadArguments {
	void* (*function)(void*);
    void*               args;    
};

/*
 * Exists to give the proper function type to clone.
 */ 
static int threadStart( void* arg ) {
	struct ThreadArguments* arguments = (struct ThreadArguments*) arg;
	//void* (*function)() = arguments->function;
    //void* args = arguments->args;
	//free( arguments );
	//arguments = NULL;
    arguments->function(arguments->args);

	printf( "Child created and calling function = %p\n", arg );
	//function(args);
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
    
    void* stack;
        
    // Allocate the stack
    stack = malloc( STACK );
    if ( stack == 0 ) {
        printf( "Unable to alocate thread stack\n" );
        return -1;
    }

    thread->stack = stack;

    struct ThreadArguments* arguments = NULL;     
    /* Create the arguments structure. */
	arguments = (struct ThreadArguments*) malloc( sizeof(*arguments) );
    
	if ( arguments == 0 ) {
		free( thread->stack );
		printf( "Unable to allocate fiber arguments.\n" );
	    return -2;
	}

	arguments->function = function; 
    arguments->args = arg;         

    printf( "Creating child thread\n" );
       
    // Call the clone system call to create the child thread
    thread->pid = clone(&threadStart,
                  (char*) stack + STACK,
                  //SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM,
                  SIGCHLD | CLONE_VM,
                  //SIGCHLD|CLONE_VM|CLONE_FS|CLONE_FILES|CLONE_SIGHAND |CLONE_SYSVSEM|CLONE_SETTLS|CLONE_PARENT_SETTID|CLONE_CHILD_CLEARTID,
                  arguments );

    if ( thread->pid == 0 ) {
        printf( "Unable to create thread\n" );
        return 1;
    }            

    return 0;            

}

/**
 *  Waits for the especified thread to end
 */
int pthread_join(mypthread_t thread, void **retval) {

    printf("Waiting for proccess to end \n");

    pid_t pid = waitpid(thread.pid, 0, 0 );
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

/*
void* threadFunction( void* argument )
{
         printf( "child thread exiting\n" );
         return 0;
}

int main() {

    mypthread_t thread1, thread2;
    pthread_create(&thread1, NULL, &threadFunction, NULL);
    pthread_create(&thread2, NULL, &threadFunction, NULL);
    pthread_detach(thread1);
    pthread_detach(thread2);

    printf("execution ended\n");

    return 0;

}*/