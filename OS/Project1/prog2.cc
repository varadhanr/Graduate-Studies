/*
 *
 * Operating System Concepts
 *
 * Project 2
 * ---------------
 *
 * Submission by Varadhan Ramamoorthy (vrr180003@utdallas.edu)
 *
 * Program 2 Large Number Multiplication using Multi Threading 
 *
 *
 */



#include <stdio.h>
#include <stdbool.h>
#include <sys/signal.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>
#include <pthread.h>
#include <iostream>
#include <unistd.h>
using namespace std;


void populateNumberArrayFromTxtFile();

void * computeMultiplicationAndWriteToFile(void *args);

void writeResultArrayToFile();

sem_t mutex3;
sem_t mutex4;

//GLOBAL Variables
int no_threads;
unsigned short num1[1000000];
unsigned short num2[1000000];
int numSize1;
int numSize2;
unsigned short result[2000000];
int stepsForMultiThreading;

//Driver Main Function
main(){

sem_init(&mutex3,0,1);
sem_init(&mutex4,0,1);

cout << "Enter Number of Threads for Multiplication:" << endl;

cin >> no_threads;

//defaulting number of threads to 2 if invalid number
if(no_threads <=0 ){
	no_threads = 2;
	cout<< "Defaulting the Thread count to 2" << endl;
}

pthread_t tid[no_threads];
stepsForMultiThreading = no_threads + 1;

populateNumberArrayFromTxtFile();

int i;
for(i=0;i<no_threads;i++){
	pthread_create(&tid[ i ],NULL,computeMultiplicationAndWriteToFile,(void *) i);

}


for( i = 0;i<no_threads;i++){
	pthread_join(tid[i],NULL);

}

writeResultArrayToFile();


return 0;


}

//function to Compute Multiplication
void * computeMultiplicationAndWriteToFile(void *args){


int i_n1=0;
int i_n2=0;
int i,j,carry,n1,n2,sum;
int jobPerThread = numSize1 / no_threads;
int threadObservor = --stepsForMultiThreading ;
//cout << endl << "Thread Observor Value:" << threadObservor << endl << "Thread count" << no_threads << endl;
for(i = (threadObservor * jobPerThread) - 1; i >= ((threadObservor - 1) * jobPerThread) && i>=0 ;i--){
	
	
	//cout << " Value of i : "<<i<<endl; 
	carry = 0;
	n1 = num1[i];
	i_n1= numSize1 - 1 - i;
	i_n2 = 0;

	for(j=numSize2-1;j>=0;j--){
	
		n2=num2[j];
		
//		sem_wait(&mutex3);
	//	usleep(1);
		sum = n1 * n2 + result[i_n1+i_n2]+carry;

		carry = sum/10;

		result[i_n1+i_n2] = sum % 10;
//		sem_post(&mutex3);

		i_n2 ++;

	}

	if(carry > 0){
//		sem_wait(&mutex4);
	//	usleep(1);
		result[i_n1+i_n2]  = result[i_n1+i_n2]+ carry;
//		sem_post(&mutex4);

	}
}


}

void writeResultArrayToFile(){

	FILE *fwrite;

	fwrite=fopen("c2.txt","w");
	int i;
	for(i=numSize1+numSize2 -1;i>=0;i--){
		
		fprintf(fwrite,"%d",result[i]);

	}

	fclose(fwrite);

}


// function that populates the array with number read from fileToBeRead 

void populateNumberArrayFromTxtFile(){

// file stream to read the file
FILE *fread1;
FILE *fread2;

//read file a.txt
fread1 = fopen("a.txt","r");
//read file b.txt
fread2 = fopen("b.txt","r");
// numSize1 and numSize2 are number of digits for A and B
//read numSIze1 and numSize2
fscanf(fread1,"%d",&numSize1);
fscanf(fread2,"%d",&numSize2);

int i;
i = 0;

// Store A in num1 array
for(i=0;i< numSize1;i++){
	fscanf(fread1,"%1d",&num1[i]);
}
// Store B in num2 array
for(i =0;i< numSize2;i++){
	fscanf(fread2,"%1d",&num2[i]);
}

fclose(fread1);
fclose(fread2);

}
