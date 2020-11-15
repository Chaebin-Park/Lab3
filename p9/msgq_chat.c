/* msgq_chat.c */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define BUFSIZE 128

typedef struct{
	long type;
	char text[BUFSIZE];
} MSGQ_DATA;

typedef struct{
	int num;
	int fd;
} ID;

void *send_message(void *arg);
void *recv_message(void *arg);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[])
{
	int fd;
	void *rest_thread;
	pthread_t send_thread, recv_thread;
	ID id;
	
	id.fd = msgget((key_t)12345, IPC_CREAT|0666);
	
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s [number]\n", argv[0]);
		exit(1);
	}
	
	id.num = atoi(argv[1]);
	printf("Login ID %d\n", id.num);
	
	pthread_create(&send_thread, NULL, send_message, &id);
	pthread_create(&recv_thread, NULL, recv_message, &id);
	pthread_join(recv_thread, &rest_thread);
	pthread_join(send_thread, &rest_thread);

	return 0;
}


void *send_message(void *arg){
	ID *num = (ID*) arg;
	pthread_t tid;
	MSGQ_DATA data;
	char tmp_buf[BUFSIZE];
	char msg_buf[BUFSIZE];
	int id;
	
	while(1)
	{
		memset(tmp_buf, '\0', BUFSIZE);
		memset(msg_buf, '\0', BUFSIZE);
		scanf("%d", &id);
		getchar();
		fgets(tmp_buf, BUFSIZE, stdin);
		*(tmp_buf+(strlen(tmp_buf)-1)) = '\0';
		
		data.type = id;
		snprintf(msg_buf, BUFSIZE*2 ,"[%d send] : %s\n", (*num).num, tmp_buf);
		strcpy(data.text, msg_buf);
		
		pthread_mutex_lock(&mutex);
		msgsnd((*num).fd, &data, sizeof(data)-sizeof(long), 0);
		pthread_mutex_unlock(&mutex);
	}
}

void *recv_message(void *arg){
	ID* num;
	num = (ID*) arg;
	char buf[BUFSIZE];
	int i;
	MSGQ_DATA data;

	while(1)
	{
		if((msgrcv((*num).fd, &data, sizeof(data)-sizeof(long), (*num).num, 0)) > 0)
		{
			printf("%s\n", data.text);
		}
	}
}
