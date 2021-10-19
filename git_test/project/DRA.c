#include "headers.h"

//전역변수 선언
//
//void *thread_client(void *);

//스레드를 돌면서 사용할 메모리

char globalbuf[BUFSIZ];
char reglobalbuf[BUFSIZ];
int check;
int recheck;

int tcheck;

pthread_t pid[2];

void *ocs_dra(){
	thread_server();
}
void *ocs_cla(){
	thread_client();
}

int main()
{
	int thr_id = 0;
	int status = 0;


	thr_id = pthread_create(&pid[0], NULL, ocs_dra, NULL);
	thr_id = pthread_create(&pid[1], NULL, ocs_cla, NULL);


	pthread_join(pid[0], (void **) &status);
	pthread_join(pid[1], (void **) &status);

	return 0;
}




