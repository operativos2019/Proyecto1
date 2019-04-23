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
FILE* createReportFile(char * name){
    
    FILE *fp;
    int i, count;
    
    char filename[1035] = "";
    strcat(filename, name);
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
char* **readCSV(int threads, int cycles, int cols, char *filename){

    double* averageResponseTime;
    int rows = (threads*cycles) + 1;
	char* **data;

	data = (char* **)malloc(rows * sizeof(char* *));
	for (int i = 0; i < rows; ++i){
		data[i] = (char* *)malloc(cols * sizeof(char*));
	}

	FILE *file;
	file = fopen(filename, "r");

	int i = 0;
    char line[4098];
	while (fgets(line, 4098, file) && (i < rows)){

        char* tmp = strdup(line);

	    int j = 0;
	    char* tok;
	    for (tok = strtok(line, ","); tok && *tok; j++, tok = strtok(NULL, ",\n"))
	    {
            data[i][j] = tok;
            printf("%s\t", data[i][j]);
            if(cols == 6 && j == 5){
                printf("dato de average %f\t", strtod(tok,NULL));
                averageResponseTime[i] = strtod(tok,NULL);
                
            }
	    }
        printf("\n");

        free(tmp);
        i++;
    }
    printf("ESTE: %s\n", data[0][0]);
    int averageResponseTimeCol = 5;
    printf("lol data: %s\n", data[1][averageResponseTimeCol]);
    printf("lol data: %s\n", data[2][averageResponseTimeCol]);
    printf("lol data: %s\n", data[3][averageResponseTimeCol]);
    printf("lol data: %s\n", data[4][averageResponseTimeCol]);
    printf("lol data: %s\n", data[5][averageResponseTimeCol]);
    return data;
}


/**
 * reorders the report
**/
void reorderReport(int threads, int cycles, int serverNumber){
    
    int rows = threads * cycles;
    int cols = 6;
    char* **data = (char* **)malloc(rows * sizeof(char* *));
	for (int i = 0; i < rows; ++i){
		data[i] = (char* *)malloc(cols * sizeof(char*));
	}

    data = readCSV(threads, cycles, cols, "Final_Benchmark_Report_TMP.csv");
   
    int averageResponseTimeCol = 5;
    int *realOrder = malloc(serverNumber * sizeof(int));
    int *averageRT = malloc(serverNumber * sizeof(int));

    printf("wtf: %s\n", data[0][0]);
    printf("wtf: %s\n", data[0][1]);
    printf("wtf: %s\n", data[0][2]);
    printf("wtf: %s\n", data[0][3]);
    printf("wtf: %s\n", data[0][4]);
    printf("wtf1 data: %s\n", data[0][5]);
    printf("wtf data: %s\n", data[1][5]);
    printf("lol data: %s\n", data[1][averageResponseTimeCol]);
    printf("lol data: %s\n", data[2][averageResponseTimeCol]);
    printf("lol data: %s\n", data[3][averageResponseTimeCol]);
    printf("lol data: %s\n", data[4][averageResponseTimeCol]);
    printf("lol data: %s\n", data[5][averageResponseTimeCol]);


    for (int i = 1; i < serverNumber; i++){
		realOrder[i] = i;
        //ESTO VA A ESTAR FEO
        printf("real data: %s\n", data[i][averageResponseTimeCol]);
        printf("real data: %s\n", data[i][averageResponseTimeCol]);
        averageRT[i] = strtod(data[i][averageResponseTimeCol],NULL) * 10000.0;
        printf("index created: %d averageRT created: %d real data: %s\n", realOrder[i],averageRT[i],data[i][averageResponseTimeCol]);
    }

    for (int i = 0; i < (serverNumber-1); i++){
		for (int j = i + 1; j < (serverNumber-1); j++){
            
            printf("index created: %d\n", realOrder[i]);

			if(averageRT[i]> averageRT[j]){
				int tempRO = realOrder[i];
				realOrder[i] = realOrder[j];
				realOrder[j] = tempRO;

                int tempRT = averageRT[i];
				averageRT[i] = averageRT[j];
				averageRT[j] = tempRT;
			}
		}
	}

    FILE* filePointer = createReportFile("Final_Benchmark_Report.csv");

    for (int i = 0; i < serverNumber; i++){
		int index = realOrder[i];
        addToReport(filePointer, data[index][test], data[index][requests],data[index][initial_request_time], data[index][fileType],data[index][fileSize], strtod(data[index][averageResponseTimeCol],NULL));

    }

    fclose(filePointer);

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


    int serverNumber = 4;
    char* servers[] = {"webserverfifo","webserverfork","webserverthread","webserverprefork"};
    //char* servers[] = {"webserverfifo","webserverfifo","webserverfifo","webserverfifo"};

    char* fileForTest = "video.ogv";
    int threads = 5;
    int cycles = 5;
    
    //FILE* report = createReportFile("Final_Benchmark_Report_TMP.csv");
    FILE* report = createReportFile("Final_Benchmark_Report.csv");
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

        char* **dataFromFile = readCSV(threads,cycles,7,filename);

        char testName[1024] = "";
        
        strcat(testName, servers[i]);
        strcat(testName, " - ");
        strcat(testName, dataFromFile[1][test]);


        addToReport(report, testName, dataFromFile[1][requests],dataFromFile[1][initial_request_time], dataFromFile[1][fileType],dataFromFile[1][fileSize], averageResponseTime(threads,cycles,dataFromFile));
    

    }

    fclose(report);

    char command_rm_config[1035]="sudo rm benchmark.out";
    system(command_rm_config);
    char command_rm_config[1035]="sudo rm bclient.out";
    system(command_rm_config);

    //reorderReport(threads, cycles, serverNumber);

}