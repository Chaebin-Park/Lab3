#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char* argv[])
{
	if(argc == 1)
	{
		printf("Usage: %s [command] [arg1] [arg2] ...\n", argv[0]);
		return 1;
	}

	pid_t pid;
	pid = fork();
	
	if(pid == 0)
	{
		execvp(argv[1], argv+1);
		perror("execl failed");	
	}
	else if(pid > 0)
	{
		wait((int*) 0);
		exit(0);
	}
	else
	{
		perror("fork failed");
	}
}
