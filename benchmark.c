#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

//http://www.convertdatatypes.com/Convert-char-Array-to-double-in-CPlusPlus.html

char* runCommand(char* command){
    FILE *fp;
    char output[1035];

    fp = popen(command, "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
    exit(1);
  }

  /* Read the output a line at a time - output it. */
  while (fgets(output, sizeof(output)-1, fp) != NULL) {
  }
  pclose(fp);
  
  char *str_to_ret = malloc (sizeof (char) * 1035);
  strcpy(str_to_ret, output);
  return str_to_ret;
   
}

FILE* createCSVFile(char* machine,  char* test){
    
    FILE *fp;
    int i, count;
    
    char filename[1035] = "Benchmark_Report_";
    strcat(filename, machine);
    strcat(filename, "_");
    strcat(filename, test);
    strcat(filename, ".csv");
    fp = fopen(filename, "w+");
    
    fprintf(fp, "%s,%s,%s,%s,%s,%s,%s\n", "Test","Requests","Initial request time","Kind of file","Size of file","Response time","Average response time");

    return fp;

}

int amountRequests(int threads, int cycles){
    return threads*cycles;
}

/* delay in milli seconds */
void delay(float milli_seconds) {
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
}

//Initial request time
char* initialRequestTime(char * machine, char *port, char *file){
    char request[1035] = "curl -sw \"%{time_connect}\" -o ";
    strcat(request, file);
    strcat(request, " http://");
    strcat(request, machine);
    strcat(request, ":");
    strcat(request, port);
    strcat(request, "/");
    strcat(request, file);

    char * time_taken = runCommand(request);
    return time_taken;
}

void copy_string(char d[], char s[]) {
  
  int c = 0;
  char *trash = malloc (sizeof (char) * 1035);

  //while (s[c] != '.') {
    //TODO: erase this space characters
    //d[c] = ' ';
    //c++;
  //}
  while (s[c] != ' ') {
    d[c] = s[c];
    c++;
  }
  d[c] = '\0';
}

char* fileKind(char *file){
    char command[1035] = "file ";
    strcat(command,file);
    char* fileKind = runCommand(command);
    fileKind[strlen(fileKind)-1] = 0;
    int c = 0;
    char *str_to_ret = malloc (sizeof (char) * 1035);
    copy_string(str_to_ret, fileKind);
    return str_to_ret;

}

char* fileSize(char *file){
    char command[1035] = "stat --format=%s ";
    strcat(command,file);
    char* fileSize = runCommand(command);
    fileSize[strlen(fileSize)-1] = 0;
    strcat(fileSize," bytes");
    return fileSize;
}


//Response time
//https://www.jonefox.com/blog/2011/10/10/find-the-time-to-first-byte-using-curl/
char* responseTime(char * machine, char *port, char *file){
    char request[1035] = "curl -sw \"%{time_starttransfer}\" -o ";
    strcat(request, file);
    strcat(request, " http://");
    strcat(request, machine);
    strcat(request, ":");
    strcat(request, port);
    strcat(request, "/");
    strcat(request, file);

    char * time_taken = runCommand(request);
    return time_taken;
}

//Average response time
double averageResponseTime(int requests, double* responseTime){
    //TODO: Fix the problem with the zeros in the array
    double sum = 0.00;
    for(int i = 0; i < requests; i++){
        sum += responseTime[i];
    }
    return sum/requests;
}

//type of test
char *extractTest(char* container){

    //to delete the '\n' char
    char* containerID = container;
    containerID[strlen(containerID)-1] = 0;
    
    char command[100] = "sudo docker cp ";
    strcat(command,containerID);
    strcat(command,":/etc/webserver/config.conf config.conf");
    system(command);

    FILE *file;
    file = fopen("config.conf", "r");
    if (file == NULL){
        perror("Config file not readable\n");
        return NULL;
    }
    char *scheduler = (char *)calloc(256, sizeof(char));
    while (!feof(file)){
        if (fscanf(file, "SCHEDULER=%s", scheduler) == 1){
            break;
        }
        fgetc(file);
    }
    if ((scheduler != NULL) && (scheduler[0] == '\0')){
        perror("scheduler not found on config file, using default\n");
        //Default scheduler
        return "Self-fish Round Robin";
    }
    fclose(file);
    return scheduler;
}

FILE* addToCSVFile(FILE* filePointer,  char* test, int requests, char* initial_request_time, char* fileType, char* fileSize, 
                char* response_time, double average_response_time){

    
    fprintf(filePointer, "%s,%d,%s,%s,%s,%s,%f\n", test, requests, initial_request_time, fileType, fileSize, response_time, average_response_time);

    //fclose(fp);
    return filePointer;
}

struct args {
    int threads;
    int cycles;
    FILE* filePointer;
    char* test;
    char* machine;
    char* port;
    char* file;
};
//if we need more that one argument we create a struct and we past a pointer to it
void *wholeInfo(void *args){
    
    char* machine = ((struct args*)args)->machine;
    char* port = ((struct args*)args)->port;
    char* file = ((struct args*)args)->file;
    int cycles = ((struct args*)args)->cycles;
    int threads = ((struct args*)args)->threads;
    char* test = ((struct args*)args)->test;
    FILE* counterInFile = ((struct args*)args)->filePointer;

    for(int i = 0; i < cycles; i++){
        int requests = amountRequests(threads,cycles);
        char* initial_request_time = initialRequestTime(machine,port,file);
        char* fileTypeString = fileKind(file);
        char* fileSizeString = fileSize(file);
        char* response_time = responseTime(machine,port,file);
        //TODO: fix the mess with the average response time
        //averageResponseTime(requests,responseTimeArray);
        addToCSVFile(counterInFile,test,requests,initial_request_time,fileTypeString,fileSizeString,response_time,0.00);
    } 
}

int main(int argc, char **argv){

    //in case container is not running
    char* container = runCommand("sudo docker ps -a | grep webserverfifo | awk '{print $1}'");
    char command[1035]="sudo docker start ";
    strcat(command,container);
    strcat(command," >/dev/null");
    system(command);

    char *machine = argv[1];
    char *port = argv[2];
    char *file = argv[3];
    int threads = atoi(argv[4]);
    int cycles = atoi(argv[5]);

    char* test = extractTest(container);
    FILE* counterInFile = createCSVFile(machine,test);

    //to make the threads
    for (int i = 0; i < threads; i++){

        pthread_t thread_id;
        struct args *argsForThread = (struct args *)malloc(sizeof(struct args));
        
        //arguments
        argsForThread->cycles = cycles;
        argsForThread->threads = threads;
        argsForThread->filePointer = counterInFile;
        argsForThread->test = test;
        argsForThread->machine = machine;
        argsForThread->port = port;
        argsForThread->file = file;

        pthread_create(&thread_id, NULL, wholeInfo, (void *)argsForThread);
        pthread_join(thread_id, NULL);
    }

    fclose(counterInFile);
}