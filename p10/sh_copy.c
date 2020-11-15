#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <errno.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

#define SEM	1
#define MAX	20

int main (int argc, char **argv){
	key_t shmkey;
	sem_t *sem;
	pid_t pid, child;
	FILE *file, *dest;
	int shmid;
	void *shared_memory;
	char *data;

	shmkey = ftok ("/dev/null", 5);
 	
 	// creat shared memory
	shmid = shmget (shmkey, sizeof (int), 0644 | IPC_CREAT);
 	if (shmid < 0)
 	{
		perror ("shmget failed\n");
		exit (1);
	}
	
	shared_memory = shmat(shmid, (void*) 0, 0);
	if (shared_memory < 0)
	{
		perror ("shmat failed\n");
		exit (1);
	}

	sem = sem_open ("pSem", O_CREAT | O_EXCL, 0644, SEM); 

	printf ("semaphores initialized.\n\n");

	pid = fork ();
	if (pid < 0) 
	{
		sem_unlink ("pSem");   
   		sem_close(sem);  
   		printf ("Fork error.\n");
   	}
     
	if (pid != 0)	// parent
	{
		while (pid = waitpid (-1, NULL, 0)){
			if (errno == ECHILD)
			break;
		}
		
		printf ("\nParent: All children have exited.\n");
		shmdt (shared_memory);
		shmctl (shmid, IPC_RMID, 0);
		sem_unlink ("pSem");
		sem_close(sem);
		exit (0);
	}
	else	// child
	{
		file = fopen("./test.txt", "r");
		if (file == NULL)
		{
			fprintf(stderr, "failed file open\n");
			return 1;
		}
		
		while(fgets(data, MAX, file) != NULL)
		
		strcpy(shared_memory, data);
		printf("copy text : %s\n", (char*) shared_memory);
		
		// P operation
		sem_wait (sem);
		printf ("  Child is in critical section.\n");
		sleep (2);
		
		dest = fopen("./copy.txt", "w");
		fputs((char*) shared_memory, dest);
		
		printf("write copy text : %s\n", (char*) shared_memory);
	
		// V operation
		sem_post (sem);
		exit (0);
	}
}
