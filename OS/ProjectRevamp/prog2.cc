/*
 *
 * Operating System Concepts
 *
 * Project 2
 * ---------------
 * FileName : prog2.cc
 *
 * Submission by Varadhan Ramamoorthy (vrr180003) and Humayoon Akhtar Qaimkhani (hxq190001)
 *
 * Class : Operating System Concepts OS 5348.001
 *
 * Naive Large Number Multiplication using Multi Threading
 *
 * g++ prog2.cc -pthread -o WithMultiThreading.out -mcmodel=medium
 *
 */



#include<stdio.h>
#include<stdbool.h>
#include<sys/signal.h>
#include<stdlib.h>
#include<semaphore.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<ctype.h>
#include<math.h>
#include <iostream>
#include <cstring>
using namespace std;

//Function Definitions
void populateNumberArrayFromTxtFile();

void * computeMultiplicationAndWriteToFile(void * args);

void writeResultArrayToFile();

void initializeArrays();

void * addTheColumnsOfResultArray(void * args);
 
void AddCarryToAddArray();

void populateSingleDigitMulTable();

#define MAXDIGITS 100000
unsigned short  result[MAXDIGITS][2*MAXDIGITS];
unsigned short toAdd[2*MAXDIGITS];
unsigned short toCarryForAdd[2 * MAXDIGITS];
unsigned short num1[MAXDIGITS];
unsigned short num2[MAXDIGITS];
unsigned short singleDigitMulTable[10][10];
int numSize1;
int numSize2;
int no_threads;
int stepsForMultiThreading;
int stepsForAdding;
sem_t mutex;

main(){

//Populate Single Digit Mul Table for lookup
populateSingleDigitMulTable();

sem_init(&mutex,0,1);

//Get number of threads from user
cout<<"Enter number of threads for Multiplication"<<endl;
cin>>no_threads;

if(no_threads <=0){
	no_threads = 2;
	cout<<"Defaulting number of threads to 2"<<endl;
}

//Array of thread identifiers
pthread_t tid[no_threads];

stepsForMultiThreading = no_threads + 1;
stepsForAdding = no_threads + 1;

//Read a.txt and b.txt to populate data structures
populateNumberArrayFromTxtFile();

//Dynamic Allocation for the 2D array
//result = (unsigned short **) malloc(MAXDIGITS * sizeof(unsigned short *));
//for(int i=0;i<MAXDIGITS;i++)
//	result[i] = (unsigned short *) malloc ((2*MAXDIGITS) * sizeof(unsigned short));

//memset(result,0,sizeof(result[0][0]) * (MAXDIGITS)*(2*MAXDIGITS));

//toAdd = malloc(sizeof(unsigned short) * (2*MAXDIGITS));

for(int t=0;t<no_threads;t++)
	pthread_create(&tid[t],NULL,computeMultiplicationAndWriteToFile,(void *) t);

for(int j=0;j<no_threads;j++)
	pthread_join(tid[j],NULL);

for(int c=0;c<no_threads;c++)
	pthread_create(&tid[c],NULL,addTheColumnsOfResultArray,(void *)c);

for(int z=0;z<no_threads;z++)
	pthread_join(tid[z],NULL);

//Cascade the carry
AddCarryToAddArray();

//Write result to file c.txt
writeResultArrayToFile();

//free(result);

return 0;


}

//Function to populate Single Digit Mul table for lookup
void populateSingleDigitMulTable(){

for(int i=0;i<10;i++){
	for(int j=0;j<10;j++){
		singleDigitMulTable[i][j] = i*j;
	
	}


}

}

//Cascading Carry 
void AddCarryToAddArray(){
int carry = 0;
for(int i= (numSize1 + numSize2 -1);i>=0;i--){
	
	int sum = toAdd[i]+ carry;
	toAdd[i] = sum % 10;
	carry = toCarryForAdd[i] + (sum / 10);

}

}


//function to Compute Multiplication
void * computeMultiplicationAndWriteToFile(void * args){


double jobPerThread = ceil(((double)numSize2)/no_threads);

//stepsForMultiThreading tells us which part need to be picked up by a single thread , we need semaphore to protect it
sem_wait(&mutex);
int threadObservor = --stepsForMultiThreading;
sem_post(&mutex);


int i,j,l = 0;

for(i = (threadObservor * jobPerThread) - 1;i>= (threadObservor - 1) * jobPerThread && i>=0 ; i--){
	
	
	if(i>=numSize2){

		continue;
	}
	
	int carry = 0;
	l = numSize2 - 1 - i;
	for(j = numSize1 -1; j>=0; j--){
	
		int mul = singleDigitMulTable[num1[j]][num2[i]];

		int sum = mul + carry;

		result[l][i+j + 1] = sum % 10;

		carry = sum/10;

	}
	if(carry > 0){
		result[l][i+j + 1] = carry;
	}

}

}

//Function to Convert the 2D array to 1D array
void * addTheColumnsOfResultArray(void * args){

double jobPerThread = ceil((double(numSize1+numSize2))/no_threads);

sem_wait(&mutex);
int threadObservor = --stepsForAdding;
sem_post(&mutex);

int carry = 0;
int i,j;
for(j= (threadObservor * jobPerThread) - 1;j>= (threadObservor - 1) * jobPerThread && j>=0;j--){
	

	if(j >= (numSize1+numSize2)){
		continue;

	}
	int addition = 0;
	for(i=0;i< numSize2;i++){
	
		addition = addition + result[i][j];

	}
	
	toAdd[j] = addition  % 10;
	toCarryForAdd[j] = addition / 10;
}

}


//Function to Write the result to c.txt
void writeResultArrayToFile(){

	FILE *fwrite;

	fwrite=fopen("c2.txt","w");
	int i;
	for(i=0;i<= (numSize1 + numSize2 -1);i++){
		
		fprintf(fwrite,"%d",toAdd[i]);

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
