//TODO: change the name of functions in description

#include <mypthreads.h>

//http://man7.org/linux/man-pages/man3/pthread_create.3.html
/*
The pthread_create() function starts a new thread in the calling
process.  The new thread starts execution by invoking
start_routine(){} arg is passed as the sole argument of start_routine().
*/
int mythread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg){


}

//
/*
The pthread_cancel() function sends a cancellation request to the
thread thread.  Whether and when the target thread reacts to the
cancellation request depends on two attributes that are under the
control of that thread: its cancelability state and type.
*/
int mythread_end(pthread_t thread){

}

//http://man7.org/linux/man-pages/man3/pthread_yield.3.html
/*
thread is placed at the end of the run queue for its static priority
and another thread is scheduled to run.  For further details, see
sched_yield(2)
*/
int mythread_yield(void){

}

//http://man7.org/linux/man-pages/man3/pthread_join.3.html
/*
The pthread_join() function waits for the thread specified by thread
to terminate.  If that thread has already terminated, then
pthread_join() returns immediately.  The thread specified by thread
must be joinable.
*/
int mythread_join(pthread_t thread, void **retval){

}

//http://man7.org/linux/man-pages/man3/pthread_detach.3.html
/*
The pthread_detach() function marks the thread identified by thread
as detached.  When a detached thread terminates, its resources are
automatically released back to the system without the need for
another thread to join with the terminated thread.
*/
int mythread_detach(pthread_t thread){}

//http://man7.org/linux/man-pages/man3/pthread_mutexattr_init.3.html
/*
The pthread_mutexattr_init() function initializes the mutex
attributes object pointed to by attr with default values for all
attributes defined by the implementation.
*/
int mymutex_init(pthread_mutexattr_t *attr){

}

//
/*
The pthread_mutexattr_destroy() function destroys a mutex attribute
object (making it uninitialized).  Once a mutex attributes object has
been destroyed, it can be reinitialized with
pthread_mutexattr_init().
*/
int mymutex_destroy(pthread_mutexattr_t *attr){

}

//https://computing.llnl.gov/tutorials/pthreads/
/*
The pthread_mutex_lock() routine is used by a thread to acquire a lock on 
the specified mutex variable. If the mutex is already locked by another 
thread, this call will block the calling thread until the mutex is unlocked.*/
int mymutex_lock(pthread_mutex_t *mutex){

}

//https://computing.llnl.gov/tutorials/pthreads/
/*
pthread_mutex_unlock() will unlock a mutex if called by the owning thread.
Calling this routine is required after a thread has completed its use of 
protected data if other threads are to acquire the mutex for their work 
with the protected data. An error will be returned if:
    If the mutex was already unlocked
    If the mutex is owned by another thread
*/
int mymutex_unlock(pthread_mutex_t *mutex){

}

//https://computing.llnl.gov/tutorials/pthreads/
/*
pthread_mutex_trylock() will attempt to lock a mutex. However, if the mutex
is already locked, the routine will return immediately with a 
"busy" error code. This routine may be useful in preventing deadlock 
conditions, as in a priority-inversion situation.
*/
int mymutex_trylock(pthread_mutex_t *mutex){

}


//http://man7.org/linux/man-pages/man7/pthreads.7.html


/*
mythread_setsched: This method will 
take care of changing the 
scheduling type of the thread.
*/
void mythread_setsched(void){

}