/* getpidproc.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int num = 0;
static char namebuf[20];

char *gentemp(char *prefix);
int itoa(int i, char *string);

int main(int argc, char* argv[])
{
	if (argc < 2) { 
		fprintf(stderr, "Usage: %s [path]\n", argv[0]); 
		exit(1); 
	} 	
	
	printf("%s\n", gentemp(argv[1]));	
}

char *gentemp(char *prefix)
{
	int length;
	pid_t pid;
	pid = getpid();
	
	strcpy (namebuf, prefix);
	length = strlen(namebuf);
	itoa (pid, &namebuf[length]);
	strcat (namebuf, ".");
	length = strlen (namebuf);
	 
	do 
	{
		itoa (num++, &namebuf[length]);
	} while (access(namebuf, F_OK) != -1) ;
	return (namebuf);
}


int itoa(int i, char *string)
{
	int power, j;
	j = i;
	for (power = 1; j >= 10; j /= 10)	power *= 10;	
	for ( ; power > 0; power /= 10)
	{
		*string++ = '0' + i/power;
		i %= power;
	}
	*string = '\0';
}

