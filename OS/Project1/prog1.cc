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

//Driver main function

void populateNumberArrayFromTxtFile(unsigned short num1[],unsigned short num2[],int *numSize1,int *numSize2);

void computeMultiplicationAndWriteToFile(unsigned short num1[],unsigned short num2[],int numSize1,int numSize2);

void writeResultArrayToFile(unsigned short result[],int numSize1,int numSize2);


main(){

unsigned short num1[1000000];

unsigned short num2[1000000];


int numSize1;
int numSize2;

populateNumberArrayFromTxtFile(num1,num2,&numSize1,&numSize2);

//Computed the Multiplication of A and B and writes it to a file
computeMultiplicationAndWriteToFile(num1,num2,numSize1,numSize2);
return 0;


}

//function to Compute Multiplication
void computeMultiplicationAndWriteToFile(unsigned short num1[],unsigned short num2[],int numSize1,int numSize2){

unsigned short result[2000000];
int i_n1=0;
int i_n2=0;
int i,j,carry,n1,n2,sum;

for(i = numSize1-1;i>=0;i--){
	
	carry = 0;
	n1 = num1[i];
	i_n1= numSize1 - 1 - i;
	i_n2 = 0;
	for(j=numSize2-1;j>=0;j--){
	
		n2=num2[j];

		int sum = n1 * n2 + result[i_n1+i_n2]+carry;

		carry = sum/10;

		result[i_n1+i_n2] = sum % 10;

		i_n2 ++;

	}

	if(carry > 0){
		result[i_n1+i_n2]  = result[i_n1+i_n2]+ carry;
	
	}

	//i_n1++;
}

writeResultArrayToFile(result,numSize1,numSize2);

}

void writeResultArrayToFile(unsigned short result[],int numSize1,int numSize2){

	FILE *fwrite;

	fwrite=fopen("c1.txt","w");
	int i;
	for(i=numSize1+numSize2 -1;i>=0;i--){
		
		fprintf(fwrite,"%d",result[i]);

	}

	fclose(fwrite);

}


// function that populates the array with number read from fileToBeRead 

void populateNumberArrayFromTxtFile(unsigned short num1[],unsigned short num2[],int * numSize1,int * numSize2){

// file stream to read the file
FILE *fread1;
FILE *fread2;

//read file a.txt
fread1 = fopen("a.txt","r");
//read file b.txt
fread2 = fopen("b.txt","r");
// numSize1 and numSize2 are number of digits for A and B
//read numSIze1 and numSize2
fscanf(fread1,"%d",numSize1);
fscanf(fread2,"%d",numSize2);

int i;
i = 0;

// Store A in num1 array
for(i=0;i< *numSize1;i++){
	fscanf(fread1,"%1d",&num1[i]);
}
// Store B in num2 array
for(i =0;i< *numSize2;i++){
	fscanf(fread2,"%1d",&num2[i]);
}

fclose(fread1);
fclose(fread2);

}
