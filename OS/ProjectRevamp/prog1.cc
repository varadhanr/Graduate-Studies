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
#include <iostream>
#include <cstring>
using namespace std;
//Driver main function

void populateNumberArrayFromTxtFile();

void computeMultiplicationAndWriteToFile();

void writeResultArrayToFile();

void populateSingleDigitMulTable();

#define MAXDIGITS 1000000
unsigned short  ** result;
unsigned short toAdd[2 * MAXDIGITS];
unsigned short num1[MAXDIGITS];
unsigned short num2[MAXDIGITS];
int numSize1;
int numSize2;
unsigned short singleDigitMulTable[10][10];
main(){

populateSingleDigitMulTable();
populateNumberArrayFromTxtFile();
result = (unsigned short **)malloc(numSize2 * sizeof(unsigned short *));
for(int i=0;i<numSize2;i++)
	result[i] = (unsigned short *)malloc((numSize1+numSize2) * sizeof(unsigned short));
//Computed the Multiplication of A and B and writes it to a file
computeMultiplicationAndWriteToFile();

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

//function to Compute Multiplication
void computeMultiplicationAndWriteToFile(){

int i,j,l = 0;

for(i = numSize2 - 1;i>=0 ; i--){

	int carry = 0;

	for(j = numSize1 -1; j>=0; j--){
	
		int mul = singleDigitMulTable[num1[j]][num2[i]];

		int sum = mul + carry;

		result[l][i+j + 1] = sum % 10;

		carry = sum/10;

	}
	if(carry > 0){
		result[l][i+j + 1] = carry;
	}
	l++;

}


int carry = 0;
for(j=(numSize1 + numSize2 - 1);j>=0;j--){
	
	int addition = 0;
	for(i=0;i< numSize2;i++){
	
		addition = addition + result[i][j];

	}
	
	addition = addition + carry;
	toAdd[j] = addition  % 10;
	carry = addition /10;
}

writeResultArrayToFile();

}

void writeResultArrayToFile(){

	FILE *fwrite;

	fwrite=fopen("c1.txt","w");
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
