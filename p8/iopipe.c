/* iopipe.c */

#include <stdio.h>

#define BUFSIZE 256

int main()
{
	FILE *pin, *pout;
	char buf[BUFSIZE];
	pin = popen("who", "r");
	pout = popen("wc -l", "w");
	while (fgets(buf, BUFSIZE, pin) != NULL)	fputs(buf, pout);

	pclose(pin);
	pclose(pout);
}

