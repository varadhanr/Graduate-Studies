/**
 * How to run this code
 *
 * cc sample.c -o executable.out
 * ./executable.out 10 a.txt f.cpp g.docx resultFile
 *
 *
 *
 *
 */



#include<stdio.h>
#include<stdbool.h>
#include<sys/signal.h>
#include<stdlib.h>
int fork(void);
int wait(int*);

// function declaration
bool validateCommandLineArguments(int argc,char ** argv);
void invokeProcesses(int argc,char ** argv);
void executeChildProcess(char fileToBeRead[],char fileToBeWritten[]);

main(int argc, char ** argv){

//argc count of arguemnts including this.filename
//argv all the arguments including this.filenam
//argv[2] to argv[argc -1] are the file names that need to be read
//argv[argc] is the ouput/result filename where result needs to be written

//First Validate command Line arguments
	if(!validateCommandLineArguments(argc,argv)){
		return -1;
	}

	invokeProcesses(argc,argv);

}

void invokeProcesses(int argc,char ** argv){
	int numberOfInputFiles = atoi(argv[1]);
	printf("Original PID %d\n",getpid());
	int i,pid,stat=0,wpid;
	for(i = 2;i< argc - 1;i++){
		if(pid=fork()==0){
		//child process
			printf("Reading File %s\n",argv[i]);
	        	executeChildProcess(argv[i],argv[argc]);	
			exit(0);
		}	

	}		
	// parent process will wait till all the child process are done
	while((wpid = wait(&stat)) != -1);
	printf("Process terminated %d\n",getpid());

}

void executeChildProcess(char fileToBeRead[], char fileToBeWritten[]){
	FILE *fread;
	FILE *fwrite;
	char ch;int i;
	int frequency[26];
	//Open file
	fread = fopen(fileToBeRead,"r");
	if(fread == NULL){
 		printf("Error while opening the file, Please check whether the file exist\n");
		return;
	}	

	// printf("The contents of the file are \n",fileToBeRead);
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
	
	fwrite = fopen(fileToBeWritten,"w");
	for(i=0;i<26;i++){
	if(frequency[i] != 0){
		printf("'%c'= %d\n",(i+97),frequency[i]);
	}
	
	}


	fclose(fread);
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


