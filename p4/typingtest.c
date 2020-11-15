/*
* 7장 파일 처리
* 파일 이름: typingtest.c
*/
#include <stdio.h>
#include <termios.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define PASSWORDSIZE 12
#define LINE 3
#define MAX 255

int main(void)
{
	time_t start_time;
	time_t end_time;
	FILE* file;
	int i=0, cnt=0, err_cnt=0, type_cnt=0;
	int fd, nread;
	double elapsed, type_speed;
	char ch;
	char text[LINE][MAX];

	struct termios init_attr, new_attr;

	file = fopen("./typing.txt", "r");
	
	if (file == NULL)
	{
		fprintf(stderr, "failed file open\n");
		return 1;
	}
	
	while(fgets(text[i], MAX, file) != NULL)	i++;
	
	fd = open(ttyname(fileno(stdin)), O_RDWR);
	tcgetattr(fd, &init_attr);
	new_attr = init_attr;
	new_attr.c_lflag &= ~ICANON;
	new_attr.c_lflag &= ~ECHO; /* ~(ECHO | ECHOE | ECHOK | ECHONL); */
	new_attr.c_cc[VMIN] = 1;
	new_attr.c_cc[VTIME] = 0;
	if (tcsetattr(fd, TCSANOW, &new_attr) != 0) 
	{
		fprintf(stderr, "터미널 속성을 설정할 수 없음.\n");
	}
	
	start_time = time(NULL);
	
	for (i=0; i<LINE; i++)
	{
		cnt=0;
		printf("\nV 다음 문장을 그대로 입력하세요.\n%s\n", text[i]);
		while ((nread=read(fd, &ch, 1)) > 0 && ch != '\n') 
		{
			if (ch == text[i][cnt++])
			{
				write(fd, &ch, 1);
			}
			else 
			{
				write(fd, "*", 1);
				err_cnt++;
			}
			type_cnt++;
		}
	}
	
	end_time = time(NULL);
	elapsed = (double)(end_time - start_time);
	type_speed = (type_cnt/elapsed)*60;
	
	printf("\n┌───────────────────────┐\n");
	printf("│     타자연습 결과     │\n");
	printf("├───────────────────────┤\n");
	printf("│총 타수   : %3d\t│\n", type_cnt);
	printf("│오타 수   : %3d\t│\n", err_cnt);
	printf("│걸린시간  : %4.2f\t│\n", elapsed);
	printf("│타자속도  : %4.2f\t│\n", type_speed);
	printf("└───────────────────────┘\n");
	
	tcsetattr(fd, TCSANOW, &init_attr);
	close(fd);
}
