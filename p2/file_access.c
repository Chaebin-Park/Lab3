/* 
 * file_access.c
 */
  
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <errno.h> 

int main(int argc, char *argv[]) 
{ 
	int ret_val; 
	int mode_array[4] = {F_OK, R_OK, W_OK, X_OK}; 
	const char *mode_str[4] = {"existent", "readble", "writable", "executable"}; 
	const char *mode_err[4] = {"Existence check", "Readability check", "Writability check", "Executability check"}; 
	int i; 
	
	if (argc < 2) { 
		fprintf(stderr, "Usage: file_access filename\n"); 
		exit(1); 
	} 

	printf("File \"%s\" : ", argv[1]); 
	
	for (i=0; i<4; i++) { 
		ret_val = access(argv[1], mode_array[i]); 
			if (ret_val == 0) printf("%s ", mode_str[i]); 
			else if (ret_val == -1) { 
				fprintf(stderr, "%s: %s\n", mode_err[i], 
				strerror(errno)); 
			} 
	} 
	printf("\n"); 
} 
