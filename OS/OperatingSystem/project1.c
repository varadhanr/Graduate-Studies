#include <stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <semaphore.h>
sem_t sem;

main(int argc, char* argv[])
{
	int i, status=0;
//	FILE *f;
	char check = 'a';
	int pid, wpid;
	sem_init(&sem, 0, 1);	

	
	for (i = 2; i < 2+atoi(argv[1]); i++){
		
		pid = fork();
		printf("fork %d \n", pid );

		if(pid  == 0){
			
//			printf("parent\n"); 
//        		printf("%d \n", pid); 
        		printf(" my id is %d \n", getpid()); 
//      		printf(" my parentid is %d \n", getppid());

			sem_wait(&sem);
			FILE *f;
			int count[256] = {0};
			f = fopen(argv[i], "r");
			if(f == NULL)
			{
				printf("No file");
			}
		
			else{

				char c = fgetc(f);
				while(c != EOF){
					c = tolower(c);
					count[c]++;			
					printf("%c", c);
					c = fgetc(f);
				
				}
			
			}

			
	
			printf("\nfile ---> %d \n", i-1);	
			int k;		
			for(k=0; k<256; k++) {
			
   				 if(count[k] > 0) {
      					printf("char %c: %d times\n", k, count[k]);
    				}

  			}
				fclose(f);
				sem_post(&sem);

				exit(0);			
			
	//		return 0;
		}
		
		
	}
		

	while ((wpid=wait(&status))!=-1);
  	printf("Process %d terminated\n",pid);
	
	
	
}

