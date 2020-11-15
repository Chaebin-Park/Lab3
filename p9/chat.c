/* chat.c */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define BUFSIZE 128

typedef struct msgq_data {
	long type;
	char text[BUFSIZE];
} msgq_data;

typedef struct tide{
	int number;
	int fd;
} tide;

void *snd_message(void *arg);
void *rcv_message(void *arg);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[])
{
	pthread_t snd_thread, rcv_thread;
	int fd;
	void *thread_result;
	tide num;
	printf("program start!\n");
	
	num.fd = msgget((key_t)12345, IPC_CREAT|0666);
	
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s [number]\n", argv[0]);
		exit(1);
	}
	
	num.number = atoi(argv[1]);
	
	printf("creat thread!\n");
	pthread_create(&snd_thread, NULL, snd_message, &num);
	pthread_create(&rcv_thread, NULL, rcv_message, &num);
	pthread_join(rcv_thread, &thread_result);
	pthread_join(snd_thread, &thread_result);

	return 0;
}

void *snd_message(void *arg)
{
	tide *num;
	num = (tide*) arg;
	pthread_t tid;
	char tmp_buf[BUFSIZE];
	char msg_buf[BUFSIZE];
	msgq_data datas;
	int c;
	
	printf("snd_message!\n");
	
	while(1)
	{
		memset(tmp_buf, '\0', BUFSIZE);
		memset(msg_buf, '\0', BUFSIZE);
		scanf("%d", &c);
		getchar();
		fgets(tmp_buf, BUFSIZE, stdin);
		*(tmp_buf+(strlen(tmp_buf)-1)) = '\0';
		
		datas.type = c;
		snprintf(msg_buf, BUFSIZE*2 ,"[%d send] : %s\n", (*num).number, tmp_buf);
		strcpy(datas.text, msg_buf);
		
		pthread_mutex_lock(&mutex);
		msgsnd((*num).fd, &datas, sizeof(datas)-sizeof(long), 0);
		pthread_mutex_unlock(&mutex);
	}
}

void *rcv_message(void *arg)
{
	tide* num;
	num = (tide*) arg;
	char buf[BUFSIZE];
	int i;
	msgq_data datas;

	printf("rcv_message!\n");
	
	while(1)
	{
		if((msgrcv((*num).fd, &datas, sizeof(datas)-sizeof(long), (*num).number, 0)) > 0)
		{
			printf("%s\n", datas.text);
		}
	}
}
