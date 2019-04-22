#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

    int test = 0;
    int requests = 1;
    int initial_request_time = 2;
    int fileType = 3;
    int fileSize = 4;
    int response_time = 5;
    double average_response_time = 6;

/**
 * calculates the average response time
 * currently not being use
**/
double averageResponseTime(int threads, int cycles, char* **data){
    double sum = 0.00;
    int rows = (threads*cycles) + 1;
    for(int i = 0; i < rows; i++){
        sum += strtod(data[i][response_time],NULL);
    }
    return sum/rows;
}

/**
 * creates the csv file where the final information is going to be written
**/
FILE* createReportFile(){
    
    FILE *fp;
    int i, count;
    
    char filename[1035] = "Final_Benchmark_Report.csv";
    fp = fopen(filename, "w+");
    
    fprintf(fp, "%s,%s,%s,%s,%s,%s\n", "Test","Requests","Initial request time","Kind of file","Size of file","Average response time");

    return fp;

}

/**
 * add a row of info to the csv file
**/
FILE* addToReport(FILE* filePointer,  char* test, char* requests, char* initial_request_time, char* fileType, char* fileSize, 
                double average_response_time){
    
    fprintf(filePointer, "%s,%s,%s,%s,%s,%f\n", test, requests, initial_request_time, fileType, fileSize, average_response_time);

    //fclose(fp);
    return filePointer;
}



/**
 * reads a csv file
**/
char* **readCSV(int threads, int cycles, char *filename){


    int row = (threads*cycles) + 1;
    //QUEMADO!???
	int col = 7;

	char* **data;
	data = (char* **)malloc(row * sizeof(char* *));
	for (int i = 0; i < row; ++i){
		data[i] = (char* *)malloc(col * sizeof(char*));
	}

	FILE *file;
	file = fopen(filename, "r");

	int i = 0;
    char line[4098];
	while (fgets(line, 4098, file) && (i < row)){

        char* tmp = strdup(line);

	    int j = 0;
	    char* tok;
	    for (tok = strtok(line, ","); tok && *tok; j++, tok = strtok(NULL, ",\n"))
	    {
            data[i][j] = tok;
	    }

        free(tmp);
        i++;
    }

    return data;
}

/**
 * this function is to run a command instruction but it catches the
 * output of return 
**/
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

/*
 *  Returns the specified port otherwise returns null
 */
int *getPort(char* serverName){
    
    //char* container = runCommand("sudo docker ps -a | grep webserverfifo | awk '{print $1}'");
    char container[100] = "sudo docker ps -a | grep ";
    strcat(container, serverName);
    strcat(container, " | awk '{print $1}'");

    //to delete the '\n' char
    char* containerID = runCommand(container);
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
    int *port = calloc(1, sizeof(int));
    while (!feof(file)){
        if (fscanf(file, "PORT=%d", port) == 1){
            break;
        }
        fgetc(file);
    }
    if ((port != NULL) && (*port == '\0')){
        perror("scheduler not found on config file, using default\n");
        //Default port
        return NULL;
    }
    fclose(file);
    char command_rm_config[1035]="sudo rm config.conf";
    system(command_rm_config);
    return port;
}

/**
 * extracts the type of scheduler thats being use by coping the config file from the container
 * and reading the data
**/
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
    char command_rm_config[1035]="sudo rm config.conf";
    system(command_rm_config);
    return scheduler;
}


int main(int argc, char **argv){
	
    //to install the bclient
    system("bash ./installBclient.sh");


    //in case container is not running
    //TODO: make it parametrizable
    char* container = runCommand("sudo docker ps -a | grep webserverfifo | awk '{print $1}'");
    char command[1035]="sudo docker start ";
    strcat(command,container);
    strcat(command," >/dev/null");
    system(command);


    int serverNumber = 5;
    //char* servers[] = {"webserverfifo","webserverfork","webserverthread","webserverprefork","webserverprethread"};
    char* servers[] = {"webserverfifo"};
    serverNumber = 1;

    char* fileForTest = "html_basic_document.html";
    //char* fileForTest = "lol.txt";
    int threads = 5;
    int cycles = 5;
    
    FILE* report = createReportFile();
    for(int i = 0; i < serverNumber; i++){
        
        char portChar[256];
        sprintf(portChar, "%d", *getPort(servers[i]));
        char* port = portChar;

        char filename[1035] = "Benchmark_Report_";
        strcat(filename, "localhost");
        strcat(filename, "_");
        strcat(filename, port);
        strcat(filename, "_");
        char * testType = extractTest(container);
        strcat(filename, testType);
        strcat(filename, ".csv");

        //Benchmark_Report_localhost_8001_Self-fish Round Robin.csv
        
        char command[1035] = "bclient ";
        strcat(command, "localhost ");
        strcat(command, port);
        strcat(command, " ");
        strcat(command, fileForTest);
        strcat(command, " ");
        char threadsChar[4];
        sprintf(threadsChar, "%d", threads);
        strcat(command,threadsChar);
        strcat(command, " ");
        char cyclesChar[4];
        sprintf(cyclesChar, "%d", cycles);
        strcat(command,cyclesChar);

        printf("comando a correr: %s\n",command);
        system(command);

        char* **dataFromFile = readCSV(threads,cycles,filename);

        char testName[1024] = "";
        
        strcat(testName, servers[i]);
        strcat(testName, " - ");
        strcat(testName, dataFromFile[1][test]);

        addToReport(report, testName, dataFromFile[1][requests],dataFromFile[1][initial_request_time], dataFromFile[1][fileType],dataFromFile[1][fileSize], averageResponseTime(threads,cycles,dataFromFile));
    

    }

    fclose(report);

}