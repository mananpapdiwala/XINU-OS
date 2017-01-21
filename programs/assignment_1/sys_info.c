#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int nargs, char *args[]){
	int pipefd[2];
	pid_t pid;
	char readbuffer[80];
	if (nargs < 2){
		printf("No argument passed. Please pass one valid argument.\n");
		return 1;
	}
	if(pipe(pipefd) == -1){
		fprintf(stderr, "The pipe process failed\n");
		return 1;
	}
	pid = fork();
	if (pid < 0){
		fprintf(stderr, "The fork process failed to create child\n");
		return 1;
	}
	else if (pid > 0){
		close(pipefd[0]);
		printf("Parent Process Id: %d\n", getpid());
		
		write(pipefd[1], args[1], strlen(args[1])+1);
		wait(NULL);
	}
	else{
		close(pipefd[1]);
		printf("Child Process Id: %d\n", getpid());
		
		read(pipefd[0], readbuffer, sizeof(readbuffer));
		char* path = "/bin/";
		char* fullpath = (char *)malloc(strlen(path)+strlen(readbuffer)+1);
		strcpy(fullpath, path);
		strcat(fullpath, readbuffer);
		if (strcmp(readbuffer, "echo") == 0){
			execl(fullpath, readbuffer, "Hello World!", NULL);
		}
		else{
			execl(fullpath, readbuffer, NULL);	
		}
	}
	return 0;
}