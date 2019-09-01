#include<stdio.h>
#include<unistd.h>
int fork(void);
main(){
int p;
if((p=fork())==0){
printf("Child Process \n");
printf("Process ID: %d\n",getpid());
printf("Parents Process ID: %d\n",getppid());
}
else{
sleep(0);
printf("Parents Process \n");
printf("Process ID: %d\n",getpid());
printf("Parents Process ID: %d\n",getppid());

}
}
