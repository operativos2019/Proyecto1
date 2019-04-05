#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

int main(int argc, char **argv){

    //to install the bclient
    system("bash ./installBenchmarkTool.sh");

    char FIFO[1035]="bclient localhost 8001 html_basic_document.html 5 5";
    //strcat(command,container);
    //strcat(command," >/dev/null");
    system(FIFO);
    
    char THREADED[1035]="bclient http://127.0.0.1/ 8001 html_basic_document.html 5 5";
    system(THREADED);
    
}