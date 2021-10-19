/****************
 * 2020/01/13   *
 * 허진우       *
 * DRA클라이언트*
 ****************/

#include "headers.h"

#define PORT 10003
#define IPADDR "127.0.0.1"

void *DRA_MGW_cla(void *);
void *DRA_MGW_ser(void *);

char quite[ ] = "exit";

pthread_t pid[2];

void *thread_client(){
	int			connSock;
	struct		sockaddr_in server_addr;
	char		*serverAddr;
	int			serverPort;

	int			thr_id, status;
	int			len;

	serverAddr = IPADDR;
	serverPort = PORT;
	printf("INFO : MGW IP = %s , MGW PORT = %d\n", serverAddr, serverPort );

	if((connSock = socket(PF_INET, SOCK_STREAM, 0)) < 0 ) {
		printf("client Can't open socket\n");
		return -1;
	}


	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(serverAddr);
	server_addr.sin_port=htons(serverPort);

	if(connect(connSock,(struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		printf("<%s> :client can't connect\n" , __func__ );
		return -1;
	}
	printf("<%s> : Client connect to server \n" , __func__ );

	thr_id = pthread_create(&pid[0], NULL, DRA_MGW_cla, (void *)connSock);
	thr_id = pthread_create(&pid[1], NULL, DRA_MGW_ser, (void *)connSock);

	pthread_join(pid[0], (void **) &status);
	pthread_join(pid[1], (void **) &status);

	close(connSock);

}

void *DRA_MGW_cla(void *data)
{
	int					n	 = 0;
	int					i	 = 0;
	char sbuf[BUFSIZ]		 = {0, };

	int connSock			= (int) data;
	reg_message rg;

	while (1)
	{
		switch(check)
		{
			case 1: printf("\n--dracla--con_reg---\n"); 
					printf("\nthread_client->%s\n",globalbuf);
					n = (int)globalbuf;			
					if(n > 0){
						write(connSock, globalbuf, sizeof(reg_message));
						memset(globalbuf, 0x00, sizeof(reg_message));
						check = 0;
					} 

					break;

					//			memset(&rg, 0x00, sizeof(reg_message));
					//			memcpy(&rg, &globalbuf, sizeof(reg_message));

					//			printf("TEST : rg.tag = %s\n", rg.tag) ;
					//			break;

			case 2: printf("\n---dracla-con_del----\n");

					if(n > 0){
						write(connSock, globalbuf, sizeof(reg_message));
						memset(globalbuf, 0x00, sizeof(reg_message));
						check = 0;
					} 

					break;

			case 3: printf("\n---dracla-con_qry----\n");
					
					if(n > 0){
						write(connSock, globalbuf, sizeof(reg_message));
						memset(globalbuf, 0x00, sizeof(reg_message));
						check = 0;
					} 

					break;
			
			case 4: printf("\n---dracla-con_up-session-id----------\n");

					if(n > 0){
						write(connSock, globalbuf, sizeof(reg_message));
						memset(globalbuf, 0x00, sizeof(reg_message));
						check = 0;
					} 
					break;

			case 41: printf("\n------dracla----con_up_IMSI----------\n");

					if(n > 0){
						write(connSock, globalbuf, sizeof(reg_message));
						memset(globalbuf, 0x00, sizeof(reg_message));
						check = 0;
					} 
					break;

			case 42: printf("\n----dracla---ocn_up_POLICY_CONTER_NUMBER-------\n");

					if(n > 0){
						write(connSock, globalbuf, sizeof(reg_message));
						memset(globalbuf, 0x00, sizeof(reg_message));
						check = 0;
					} 
					break;

			case 43: printf("\n---dracla--con_Policy_Counter_Identifier-----------\n");

					if(n > 0){
						write(connSock, globalbuf, sizeof(reg_message));
						memset(globalbuf, 0x00, sizeof(reg_message));
						check = 0;
					} 
					break;

			case 44: printf("\n---dracla--con_Policy_Counter_status--------\n");

					if(n > 0){
						write(connSock, globalbuf, sizeof(reg_message));
						memset(globalbuf, 0x00, sizeof(reg_message));
						check = 0;
					} 
					break;
		}
	}
}

void *DRA_MGW_ser(void *data)
{
	int n = 0;
	int i = 0;
	char rbuf[BUFSIZ] = {0, };
	int connSock = (int) data;
	int len = 0;

	recheck = 0;

	recv_message rm;
	reg_message rrg;
	rech rck;

	memset(&rm, 0x00,	sizeof(recv_message));
	memset(rbuf, 0x00,	sizeof(rbuf));
	memset(&rrg, 0x00, sizeof(reg_message));


	while(1){
		if((n = read(connSock, rbuf, BUFSIZ)) >0 ) {
				printf("\nMGW---> DRA.CLA--->>%s\n", rbuf);

				strncpy ( rrg.tag, rbuf+40, sizeof(rrg.tag) ) ;
				strncpy ( rrg.value, rbuf+48, sizeof(rrg.value) );
				rrg.tag[4] = '\0' ;
				rrg.value[4] = '\0' ;

				printf("rrg_value -> %s\n", rrg.value);
				i = atoi(rrg.value);

				if(strcmp("268 ", rrg.tag) == 0) {			//result code
					switch(i){
						case 2001 : printf("success code\n");
									strcpy(rck.recheck, "Ok_result_code");
									memcpy(&reglobalbuf, &rck, sizeof(recv_message));
									recheck = 1;
								break;

						case 5012 : printf("error code\n");
									strcpy(rck.recheck, "error_code");
									memcpy(&reglobalbuf, &rck, sizeof(recv_message));
									recheck = 1;
								break;

						default : printf("\nUndefined message format!\n");
								  
								  strcpy(rck.recheck,"Undefined Format TRY AGAIN");
								  write(connSock, &rck, sizeof(rech));
								  memset(&rck, 0x00, sizeof(rech));
								  break;
 
					}

				}else{

					return 0;	
					break;
				}
		}
	}
}

