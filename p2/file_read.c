#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSIZE 512 

int main(int argc, char *argv[])
{
	char buffer[BUFSIZE];
	int filedes;
	ssize_t nread;
	long total = 0;
	
	if (argc < 2){
		fprintf(stderr, "Usage: file_read filename\n"); 
		exit(1); 
	}

	if ((filedes = open (argv[1], O_RDONLY)) == -1)
	{
		printf ("error in opening anotherfile\n");
		exit (1);
	}

while ((nread = read(filedes, buffer, BUFSIZE)) > 0)
	total += nread;
	
printf ("total chars in anotherfile: %ld\n", total);
exit (0);
}
