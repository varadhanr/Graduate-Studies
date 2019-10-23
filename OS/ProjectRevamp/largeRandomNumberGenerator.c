/*
 *
 * Program to generate file a.txt and b.txt with specified number of digits 
 * 
 * Change maxDigits variable
 * 
 * gcc largeRandomNumberGenerator -o LargeNumberGenerator
 *
 *
 */



#include<stdio.h>
#include<stdlib.h>



main(){


FILE *fwrite;
fwrite = fopen("a.txt","w");

int i;
int randomInteger;
int maxDigits;

maxDigits = 100000;

srand(time(NULL));

fprintf(fwrite,"%d ",maxDigits);

for(i=0;i<maxDigits;i++){
	randomInteger = rand() % 10;
	fprintf(fwrite,"%d",randomInteger);
}

fclose(fwrite);

fwrite = fopen("b.txt","w");

fprintf(fwrite,"%d ",maxDigits);

for(i=0;i<maxDigits;i++){
	randomInteger = rand() % 10;
	fprintf(fwrite,"%d",randomInteger);
}

fclose(fwrite);
}
