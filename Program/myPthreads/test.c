#include "mypthread.h"

#include <stdio.h>
#include <stdlib.h>


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

    return 0;}