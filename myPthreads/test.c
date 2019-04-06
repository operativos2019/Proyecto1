#include "mypthread.h"

#include <stdio.h>
#include <stdlib.h>


void* threadFunction( void* argument )
{
         printf( "child thread entering\n" );   
         printf("pointer: %p\n", argument);
         printf("data: %s\n", ((char*)argument));
         printf( "child thread exiting\n" );
         return 0;
}

int main() {

    char* message1 = "Thread1";
    char* message2 = "Thread2";

    mypthread_t thread1, thread2;
    pthread_create(&thread1, NULL, threadFunction, (void*)message1);
    pthread_create(&thread2, NULL, threadFunction, (void*)message2);
    pthread_detach(thread1);
    pthread_detach(thread2);

    printf("execution ended\n");

    return 0;}