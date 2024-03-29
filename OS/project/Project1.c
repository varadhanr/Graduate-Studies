/**
 *
 * Submission by Varadhan Ramamoorthy (vrr180003) and Humayoon Akhtar Qaimkhani (hxq190001)
 * 
 * How to run this code
 *
 * gcc -pthread Project.c -o executable.out
 * ./executable.out 6 file1.txt file2.txt file3.pdf file4.docx file5.xml file5.html result.txt
 *
 *
 *
 *
 */



#include<stdio.h>
#include<stdbool.h>
#include<sys/signal.h>
#include<stdlib.h>
#include<semaphore.h>

int fork(void);
int wait(int*);

//semaphore for mutex
sem_t mutex;

// function declaration
bool validateCommandLineArguments(int argc,char ** argv);
void invokeProcesses(int argc,char ** argv);
void executeChildProcess(char fileToBeRead[],char fileToBeWritten[]);

main(int argc, char ** argv){

//argc count of arguemnts including this.filename
//argv all the arguments including this.filename
//argv[2] to argv[argc -2] are the file names that need to be read
//argv[argc-1] is the ouput/result filename where result needs to be written

//First Validate command Line arguments
	//Initialize Semaphore
	sem_init(&mutex,0,1);
	
	if(!validateCommandLineArguments(argc,argv)){
		return -1;
	}

	invokeProcesses(argc,argv);

}

void invokeProcesses(int argc,char ** argv){

	int numberOfInputFiles = atoi(argv[1]);

	int i,pid,stat=0,wpid;
	for(i = 2;i<= argc - 2;i++){
		if(pid=fork()==0){
		//child process
			sem_wait(&mutex);
			printf("Reading File %s\n",argv[i]);
	        	executeChildProcess(argv[i],argv[argc-1]);
			sem_post(&mutex);
			exit(0);
		}	

	}		
	
	// parent process will wait till all the child process are done
	while((wpid = wait(&stat)) != -1);
	printf("Parent Process terminated %d\n",getpid());

}

void executeChildProcess(char fileToBeRead[], char fileToBeWritten[]){

	
	FILE *fread;
	FILE *fwrite;
	char ch;int i;
	int frequency[26];
	//Open file
	fread = fopen(fileToBeRead,"r");
	if(fread == NULL){
 		printf("Error while opening the file %s, Please check whether the file exist\n",fileToBeRead);
		return;
	}	


	//Initialize frquency of each character to 0
	for(i = 0; i<26;i++){
		frequency[i] = 0;
	}

	//read from fread to evaluate the frequency count
 	while((ch = fgetc(fread)) != EOF){
 		//printf("%c",ch);
		if((ch >='a' && ch <='z') || (ch >='A' && ch<='Z')){
		//ch is an alphabet
		if(ch >= 'a' && ch <='z'){
			frequency[ch - 97]++;		
		}else{
			frequency[ch - 65]++;
		}
		
		}
	
	}

	fwrite = fopen(fileToBeWritten,"a");
	if(fwrite == NULL){
		printf("Error while opening the result file\n");
		return;
	}

	printf("\nFile Name : %s, Process ID : %d\n",fileToBeRead,getpid());
	
	
	//write the frequency count to the result file
	for(i=0;i<26;i++){
		printf("Character :%c , count: %d \n",(i+97),frequency[i]);
	}
	
	fclose(fread);
	fclose(fwrite);
	
	return;

}


bool validateCommandLineArguments(int argc, char ** argv){
	// argv will contain this.filename,Command Line Argument : fileCount,file1,file2,file3,file4......,resultFile
	if(argc == 1){
		printf("No other extra comman line arguemnt passed other than program name\n");
		return false;
	}
	int numberOfInputFiles = atoi(argv[1]);
	if(numberOfInputFiles <= 0){
		printf("Please enter a valid number of existing files\n");
		return false;
	}
	if(argc - 3 != numberOfInputFiles){
		printf("Not enough file name are present in the command line argument\n");
		return false;
	}
	return true;
}
