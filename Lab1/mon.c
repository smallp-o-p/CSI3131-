// C program to demonstrate creating processes using fork()
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>


const char* makefilepath(char *file);
void myitoa(int n, char s[]);
void reverse(char s[]);
/* Assumes procmon and the program to launch are in the same directory.*/
int main(int argc, char **argv) // not very portable but it fulfills the requirements of the lab. 
{
	char *program;
	char temp[1000];
	if(argc!= 2){
		printf("wrong usage");
		exit(-1);
	}
	else{
		program = argv[1];
		printf("%s\n", program);
	
	}
	const char *filepath = makefilepath(program); 
	pid_t process1 = fork();
	pid_t process2; 
	if(process1 == 0){ // in the subprocess, process1 will have a value of 0 while the parent process will get an actual PID number. 
		pid_t process1pid = getpid();
		process2 = fork(); // make a sub child, this isn't visible by the parent, however we do know its PID will always be +1 of process1. 
		if(process2 != 0){ // process 1 does this, if it was process2 this would return 0. 
			printf("Launching %s with PID %lld \n", program, process1pid);
			execl(filepath, NULL); // replace process1 with argument
			
		}
		else{ // process 2 does this
			myitoa(process1pid, temp); // convert pid to a string
			printf("Launching procmon with PID %lld, monitoring process %lld \n", getpid(), process1pid);
			execl("./procmon", "procmon,", temp, NULL); // replace process2 with procmon
		}
	}
	else{ // parent process does this, messages might be out of order. 
		printf("Parent process with PID %lld \n", getpid());
		printf("Sleeping 20s before killing %s \n", program);
		sleep(20);
		kill(process1, 15); 
		printf("%lld Killed!\n", process1);
		printf("Sleeping 2s before killing procmon.\n");
		sleep(2);
		kill(process1+1, 15);
		printf("%lld Killed!\n", (process1+1)); 
	}
}


const char*  makefilepath(char *file){ // little helper function
	char *stuff;
	stuff = malloc(3 + strlen(file));
	strcpy(stuff, "./");
	strcat(stuff, file);  
	return stuff; 
}

void myitoa(int n, char s[]){ // K&R 1988 implementation of itoa
	int i, sign;
	if((sign = n)<0){
		n = -n;
	}
	i = 0; 
	do{
		s[i++] = n%10 + '0';
	}
	while((n/=10)>0);
	if(sign < 0){
		s[i++] = '-';
	}
	s[i]='\0';
	reverse(s);
}

void reverse(char s[]){ // K&R 1988 implementation of reverse
	int i, j;
	char c; 
	for(i=0, j= strlen(s)-1; i<j; i++, j--){
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}



