/************
 *2020/01/13*
 *허진우    *
 *DRA서버   *
 ************/

#include "headers.h"

#define PORT 10002
#define IPADDR "127.0.0.1"
#define PORTNUM 10001

void *do_send(void*);
void *do_recv(void*);

char quit[ ] = "exit";

pthread_t pid[4];

void *thread_server()
{
	int		thr_id;
	int		status;	

	int		listenSock, connSock;
	struct	sockaddr_in client_addr, server_addr;

	int len;

	// START SERVER
	if(PORTNUM < 2){
		printf("Usage: talk_server port_num\n");
		return -1;
	}

	if((listenSock = socket(PF_INET, SOCK_STREAM, 0)) < 0) { 
		printf("Server: Can't open socket\n");
		return -1;
	}

	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORTNUM);

	if(bind(listenSock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		printf("talk Server Can't bind\n");
		return -1;
	}

	listen(listenSock, 1);

	len = sizeof(client_addr);
	if((connSock = accept(listenSock, (struct sockaddr *) &client_addr, &len)) < 0) {
		printf("Talk Server failed in accepting\n");
		return -1;
	}

	printf("Talk Server Success in accepting\n");
	// END SERVER

	// START CLIENT 
	int     connSockCli;
	char    *serverAddr;
	int     serverPort;

	printf("55555555555555555555555555555555");

	serverAddr = IPADDR;
	serverPort = PORT;
	printf("INFO : MGW IP = %s , MGW PORT = %d\n", serverAddr, serverPort );

	if((connSockCli = socket(PF_INET, SOCK_STREAM, 0)) < 0 ) {
		printf("Talk client Can't open socket\n");
		return -1;
	}

	printf("666666666666666666666666666666666");
	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(serverAddr);
	server_addr.sin_port=htons(serverPort);

	if(connect(connSockCli,(struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		printf("talk client can't connect\n");
		return -1;
	}
	printf("Talk Client connect to Talk server \n");
	// END CLIENT

	thr_id = pthread_create(&pid[0], NULL, do_recv, (void *)connSock);
	thr_id = pthread_create(&pid[1], NULL, do_recv, (void *)connSock);
	thr_id = pthread_create(&pid[2], NULL, DRA_MGW_cla, (void *)connSock);
	thr_id = pthread_create(&pid[3], NULL, DRA_MGW_ser, (void *)connSock);

	pthread_join(pid[0], (void **) &status);
	pthread_join(pid[1], (void **) &status);
	pthread_join(pid[2], (void **) &status);
	pthread_join(pid[3], (void **) &status);

	close(listenSock);
	close(connSock);
	close(connSockCli);
}

void *do_send(void *data)
{
	int n;
	char sbuf[BUFSIZ];
	int connSock = (int) data;
	int len;

	printf("DRA->client server\n");

#if 0
		while((n = write(0, sbuf, BUFSIZ)) > 0) {    무한으로 클라이언트한테 전송됨
		if(write(connSock, sbuf, n) != n) {
			printf("Talk Server fail in sending\n");
		}
		}
#else
		while((n = read(0, sbuf, BUFSIZ)) > 0) {
		if(write(connSock, sbuf, n) != n) {
			printf("Talk Server fail in sending\n");
		}

#endif
		if(strncmp(sbuf, quit, 4) == 0) {
			pthread_kill(pid[1],SIGINT);
			break;
		}
	}

}

void *do_recv(void *data)
{
	int n;
	char rbuf[BUFSIZ];
	char sbuf[BUFSIZ];
	int connSock = (int) data;

	char globalbuf[BUFSIZ];

	reg_message rg;
	memset(&rg, 0x00, sizeof(reg_message));
	strcpy(rg.proto_sp, "MGW");
	strcpy(rg.interface_ver, "VO1");
	strcpy(rg.ssid, "DRA-JA    ");
	strcpy(rg.ms_sp, "1");
	strcpy(rg.ts_id, "12345678");	//Trasaction-ID 중복안되는 랜덤으로 뽑아 전송하기. 8자리
	strcpy(rg.hopeid, "dd");		//	hopeid?? 8자리
	strcpy(rg.op_code, "001");
	strcpy(rg.h_length, "  ");		//body의 길이 계산해서 집어넣기.

	strcpy(rg.tag, "100 ");
	strcpy(rg.b_length, " ");		//value의 값을 구해야하는데..
	strcpy(rg.value, " ");			//tag의 값들에따른 value들이 달라지게.
	//memcpy(&globalbuf, &rg, sizeof(reg_message));
	//복사한 값들은 전역 변수에 집어넣어 thread_client로 넘겨주기.


	printf("client->DRA client\n");

	while(1){
		if((n = read(connSock, rbuf, BUFSIZ)) > 0) {
			printf("%s\n", rbuf);
			if(strcmp(rbuf, "1")){
				memcpy(&globalbuf, &rg, sizeof(reg_message));
				//write(connSock, sbuf, sizeof(reg_message));
				printf("%s\n",globalbuf);
				printf("success!\n");
			} else if(rbuf != NULL){
				printf("Fail!\n");
				exit(-1);
			}

			if(strncmp(rbuf, quit, 4) == 0) {
				pthread_kill(pid[0], SIGINT);
				break;
			}
		}
	}
}


