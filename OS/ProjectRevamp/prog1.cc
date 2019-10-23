/*
 *
 * Operating System Concepts
 *
 * Project 2
 * ---------------
 * FileName : prog1.cc
 *
 * Submission by Varadhan Ramamoorthy (vrr180003) and Humayoon Akthar Qaimkhani (hxq190001)
 *
 * Class : Operating System Concepts OS 5348.001
 * 
 * Naive Large Number Multiplication
 * 
 * g++ prog1.cc -o WithoutMultiThreading.out -mcmodel=medium
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

//Function declarations
void populateNumberArrayFromTxtFile();

void computeMultiplicationAndWriteToFile();

void writeResultArrayToFile();

void populateSingleDigitMulTable();

#define MAXDIGITS 100000


unsigned short  result[MAXDIGITS][2*MAXDIGITS];

unsigned short toAdd[2 * MAXDIGITS];

unsigned short num1[MAXDIGITS];

unsigned short num2[MAXDIGITS];

int numSize1;

int numSize2;

//Table for multiplication of two single digit
unsigned short singleDigitMulTable[10][10];

//Driver main function
main() {

//Populate Single Digit Multiplication Table
populateSingleDigitMulTable();

//Read the Files a.txt and b.txt
populateNumberArrayFromTxtFile();

//Dynamic Allocation of 2D array to store Multiplication
//result = (unsigned short **)malloc(MAXDIGITS * sizeof(unsigned short *));
//for(int i=0;i<MAXDIGITS;i++)
//	result[i] = (unsigned short *)malloc((2*MAXDIGITS) * sizeof(unsigned short));

//Computed the Multiplication of A and B and writes it to a file
computeMultiplicationAndWriteToFile();

//free(result);

return 0;

}

//Function Definition to Populcate Single Digit Multiplication table
void populateSingleDigitMulTable(){

for(int i=0;i<10;i++){

	for(int j=0;j<10;j++){
	
		singleDigitMulTable[i][j] = i*j;

	}
}

}

//Function to Compute Multiplication and Write it to File c.txt
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

//Write to the File c.txt
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


// Function that populates the array with number read from fileToBeRead 

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
