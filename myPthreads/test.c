#include "mypthread.h"

#include <stdio.h>
#include <stdlib.h>

void* threadFunction( void* argument ) {
    printf( "data: %s\n", ((char*)argument) );
    return 0;
}

int main() {

    char* message1 = "Thread1";
    char* message2 = "Thread2";
    char* message3 = "Thread3";

    mypthread_t thread1, thread2, thread3;
    pthread_create(&thread1, NULL, threadFunction, (void*)message1);
    pthread_create(&thread2, NULL, threadFunction, (void*)message2);
    pthread_create(&thread3, NULL, threadFunction, (void*)message3);
    //pthread_detach(thread1);
    //pthread_detach(thread2);
    pthread_join(thread3,NULL);

    printf("execution ended\n");

    return 0;
}