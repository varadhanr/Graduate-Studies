/*
 *
 * Operating System Concepts
 *
 * Project 2
 * ---------------
 *
 * Submission by Varadhan Ramamoorthy (vrr180003@utdallas.edu)
 * Naive Large Number Multiplication
 *
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
//Driver main function

void populateNumberArrayFromTxtFile();

void * computeMultiplicationAndWriteToFile(void * args);

void writeResultArrayToFile();

void initializeArrays();

void * addTheColumnsOfResultArray(void * args);
 
void AddCarryToAddArray();

void populateSingleDigitMulTable();

#define MAXDIGITS 1000000
unsigned short ** result;
unsigned short toAdd[2 * MAXDIGITS];
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
populateSingleDigitMulTable();
//result = (unsigned short **)malloc(numSize2 * sizeof(unsigned short *));
//for(int i=0;i<numSize2;i++)
//	result[i]=(unsigned short *)malloc((numSize1+numSize2)* sizeof(unsigned short));

sem_init(&mutex,0,1);

cout<<"Enter number of threads for Multiplication"<<endl;
cin>>no_threads;

if(no_threads <=0){
	no_threads = 2;
	cout<<"Defaulting number of threads to 2"<<endl;
}

pthread_t tid[no_threads];
stepsForMultiThreading = no_threads + 1;
stepsForAdding = no_threads + 1;

populateNumberArrayFromTxtFile();
result = (unsigned short **) malloc(numSize2 * sizeof(unsigned short *));
for(int i=0;i<numSize2;i++)
	result[i] = (unsigned short *) malloc ((numSize1+numSize2) * sizeof(unsigned short));
//initializeArrays();

for(int t=0;t<no_threads;t++)
	pthread_create(&tid[t],NULL,computeMultiplicationAndWriteToFile,(void *) t);

for(int j=0;j<no_threads;j++)
	pthread_join(tid[j],NULL);

for(int c=0;c<no_threads;c++)
	pthread_create(&tid[c],NULL,addTheColumnsOfResultArray,(void *)c);

for(int z=0;z<no_threads;z++)
	pthread_join(tid[z],NULL);

AddCarryToAddArray();

writeResultArrayToFile();

free(result);
return 0;


}

void populateSingleDigitMulTable(){

for(int i=0;i<10;i++){
	for(int j=0;j<10;j++){
		singleDigitMulTable[i][j] = i*j;
	
	}


}

}

void AddCarryToAddArray(){
int carry = 0;
for(int i= (numSize1 + numSize2 -1);i>=0;i--){
	//cout<<i<<endl;
	int sum = toAdd[i]+ carry;
	toAdd[i] = sum % 10;
	carry = toCarryForAdd[i] + (sum / 10);

}

}


void initializeArrays(){

	for(int l=0;l<numSize2;l++)
		for(int k=0;k<numSize1+numSize2;k++)
			result[l][k]=0;

	for(int x=0;x<numSize1+numSize2;x++)
		toAdd[x] = 0;
}

//function to Compute Multiplication
void * computeMultiplicationAndWriteToFile(void * args){


double jobPerThread = ceil(((double)numSize2)/no_threads);
//cout<<jobPerThread<<endl;
sem_wait(&mutex);
int threadObservor = --stepsForMultiThreading;
sem_post(&mutex);

//cout<<threadObservor<<endl;
int i,j,l = 0;

for(i = (threadObservor * jobPerThread) - 1;i>= (threadObservor - 1) * jobPerThread && i>=0 ; i--){
	//cout<<i<<endl;
	
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


void * addTheColumnsOfResultArray(void * args){

double jobPerThread = ceil((double(numSize1+numSize2))/no_threads);
sem_wait(&mutex);
int threadObservor = --stepsForAdding;
sem_post(&mutex);

int carry = 0;
int i,j;
for(j= (threadObservor * jobPerThread) - 1;j>= (threadObservor - 1) * jobPerThread && j>=0;j--){
	
	//cout<<j<<endl;
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
