/* waitpidprocess.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void fatal(char *error_name);

int main()
{
	pid_t pid;
	int status, exit_status;

	if ((pid = fork()) < 0)	fatal("fork failed");
	if (pid == 0)
	{
		sleep (4);
		exit (5);
	}

	while (waitpid (pid, &status, WNOHANG) == 0) 
	{
		printf ("still waiting… \n");
		sleep (1);
	}
	
	if (WIFEXITED(status))
	{
		exit_status = WEXITSTATUS (status);
		printf ("Exit status from %d was %d\n", pid, exit_status);
	}
	
	exit (0);
}

void fatal(char *error_name)
{
    perror(error_name);
    exit(1);
}
