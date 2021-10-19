/************
 *2020/01/13*
 *허진우    *
 *DRA서버   *
 ************/

#include "headers.h"

#define PORTNUM 10002

void *do_send(void *);
void *do_recv(void *);

int pushClient(int);
int popClient(int);


char quit[ ] = "exit";

pthread_t pid[2];
pthread_mutex_t mutex;

#define INVALID_SOCK -1	
#define MAX_CLIENT 5

int list_c[MAX_CLIENT];

void *thread_server()
{
	int		thr_id =			0;
	int		status =			0;	

	int		listenSock, connSock;
	struct	sockaddr_in client_addr, server_addr;

	int		len =				0;
	int		les =				0;
	int		i =					0;
	int		j =					0;
	int		n =					0;


	if(PORTNUM < 2){
		printf("Usage: server port_num\n");
		return -1;
	}

	if(pthread_mutex_init(&mutex, NULL) != 0) {
		printf("Can not create mutex\n");
		return -1;
	}

	if((listenSock = socket(PF_INET, SOCK_STREAM, 0)) < 0) { 
		printf("Server: Can't open socket\n");
		return -1;
	}
	printf("INFO : MGW IP = %d , MGW PORT = %d\n",INADDR_ANY , PORTNUM );  
	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORTNUM);

	if(bind(listenSock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		printf("Server Can't bind\n");
		return -1;
	}

	if(listen(listenSock, MAX_CLIENT == -1)) {
		printf("listen Fail\n");	
		return -1;
	}

	for(i = 0; i < MAX_CLIENT; i++)
		list_c[i] = INVALID_SOCK;

	listen(listenSock, 1);

	

	while(1) {
		len = sizeof(client_addr);
		if((connSock = accept(listenSock, (struct sockaddr *) &client_addr, &len)) < 0) {

			printf("Server failed in accepting\n");
			return -1;
		}

			printf("Server Success in accepting\n");

			//thr_id = pthread_create(&pid[0], NULL, do_send, (void *)connSock);
			thr_id = pthread_create(&pid[0], NULL, do_send, (void *)connSock);
			thr_id = pthread_create(&pid[1], NULL, do_recv, (void *)connSock);

			pthread_join(pid[0], (void **) &status);
			pthread_join(pid[1], (void **) &status);

			close(listenSock);
			close(connSock);
	//	}
	}
}

void *do_send(void *data)
{
	int			n	=	0;
	int			i	=	0;
	char sbuf[BUFSIZ];
	int connSock	= (int) data;

	recv_message rm;
	rech rr;
	memset(sbuf,	0x00,	sizeof(sbuf));
	memset(&rm,		0x00,	sizeof(recv_message));


	while(1)
	{
		if(recheck == 1)
		{	
			n = write(connSock, reglobalbuf, sizeof(rech));
			recheck = 0;
			printf("\nDRA->OCS message sending\n");
			memset(reglobalbuf, 0x00, sizeof(recv_message));
				for(i = 0; i < MAX_CLIENT; i++) {
					if(list_c[i] != INVALID_SOCK) {
						write(list_c[i], reglobalbuf, sizeof(recv_message));
					}
				}
				if(strstr(reglobalbuf, quit) != NULL) {
					popClient(connSock);
					break;
				}
		}
		if(strncmp(sbuf, quit, 4) == 0) {
			pthread_kill(pid[1],SIGINT);
			break;
		}
	}
}

void *do_recv(void *data)
{
	int		n				=	 0;
	int		i				=	 0;
	int		len				=	 0;
	char	rbuf[BUFSIZ]	=	{0, };
	int		connSock		=	(int) data;
	char	let[20]			=	{0, };

	FILE	*fp;
	char	buffer[BUFSIZ];
	int		j;
	char	blen[BUFSIZ]	=	{0, };

	int		sum				=	0;
	int		sum1			=	0;
	int		sum2			=	0;
	int		sum3			=	0;
	int		sum4			=	0;
	char	sum5[BUFSIZ]	=	{0, };


	reg_message		rg;
	reg_message		*mglobalbuf;
	length			lg;
	rech			rr;

	memset(&rg,		0x00,	sizeof(reg_message));
	memset(rbuf,	0x00,	sizeof(rbuf));
	memset(buffer,	0x00,	sizeof(buffer));
	memset(&rr,		0x00,	sizeof(rech));



	while(1){ 
		n = read(connSock, rbuf, BUFSIZ);

		if(n > 0){
			int rest = 0;
			rest = atoi(rbuf);
			printf("rest->%d\n",rest);

			switch(rest){
				case 1 : printf("\n-----draser----------------reg------------------------------\n");

						 if((fp = fopen("/heoju/project/kTtxt/ktval.txt","r")) != NULL) {
							 while(fread(buffer, 1, BUFFER_SIZE, fp) != 0);

							 j = strlen(buffer); //buffer의 길이
							 sprintf(blen, "%-4d", j); 

							 sum = strlen(blen); //4

							 sum2 = 4 + 4 + j;

							 sprintf(sum5, "%-4d", sum2);

							 strcpy(rg.proto_sp,			"MGW");
							 strcpy(rg.interface_ver,		"V01");
							 strcpy(rg.ssid,				"DRA-JA    ");
							 strcpy(rg.ms_sp,				"1");
							 strcpy(rg.ts_id,				"12345678");	
							 strcpy(rg.hopeid,				"dd      ");	
							 strcpy(rg.op_code,				"001");			//연결 요청
							 strcpy(rg.h_length,			sum5);		

							 strcpy(rg.tag,					"100 ");
							 strcpy(rg.b_length,			blen);		
							 strcpy(rg.value,				buffer);		


							 mglobalbuf = &m1;
							 fclose(fp);
						 }
						 check = 0;
						 memcpy(&globalbuf, &rg, sizeof(reg_message));
						 check = 1;

						 break;

				case 2: printf("\n---------draser-----------del------------------------------\n");
						
						memset(&rg, 0x00, sizeof(reg_message));
						memset(rbuf, 0x00, sizeof(rbuf));
						memset(globalbuf, 0x00, sizeof(reg_message));

							 strcpy(rg.proto_sp,			"MGW");
							 strcpy(rg.interface_ver,		"VO1");
							 strcpy(rg.ssid,				"DRA-JA    ");
							 strcpy(rg.ms_sp,				"1");
							 strcpy(rg.ts_id,				"12345678");	
							 strcpy(rg.hopeid,				"dd      ");	
							 strcpy(rg.op_code,				"002");				//연결 해지요청
							 strcpy(rg.h_length,			"28  ");		

							 strcpy(rg.tag,					"100 ");
							 strcpy(rg.b_length,			"22   ");		
							 strcpy(rg.value,				"4583206733641305437856");			//TCP-CONNECTION별 고유한 값

							 check = 0;
							 memcpy(&globalbuf, &rg, sizeof(reg_message));
							 check = 2;


						break;

				case 3: printf("\n---------draser------------qry-----------------------------\n");

						memset(&rg, 0x00, sizeof(reg_message));
						memset(rbuf, 0x00, sizeof(rbuf));
						memset(globalbuf, 0x00, sizeof(reg_message));

							 strcpy(rg.proto_sp,			"MGW");
							 strcpy(rg.interface_ver,		"VO1");
							 strcpy(rg.ssid,				"DRA-JA    ");
							 strcpy(rg.ms_sp,				"1");
							 strcpy(rg.ts_id,				"12345678");	
							 strcpy(rg.hopeid,				"dd      ");	
							 strcpy(rg.op_code,				"003");				//연결 상태확인
							 strcpy(rg.h_length,			"46  ");		

							 strcpy(rg.tag,					"100 ");
							 strcpy(rg.b_length,			"38  ");		
							 strcpy(rg.value,				"34237839637657821748302147485823465781");		

							 check = 0;
							 memcpy(&globalbuf, &rg, sizeof(reg_message));
							 check = 3;
						
						break;

				case 4: printf("\n----------draser-----------up_session_id-----------------------------\n");

						memset(&rg, 0x00, sizeof(reg_message));
						memset(rbuf, 0x00, sizeof(rbuf));
						memset(globalbuf, 0x00, sizeof(reg_message));

							 strcpy(rg.proto_sp,			"MGW");
							 strcpy(rg.interface_ver,		"VO1");
							 strcpy(rg.ssid,				"DRA-JA    ");
							 strcpy(rg.ms_sp,				"1");
							 strcpy(rg.ts_id,				"12345678");	
							 strcpy(rg.hopeid,				"dd      ");	
							 strcpy(rg.op_code,				"112");				// QOS 제어 요청 메시지
							 strcpy(rg.h_length,			"50  ");		

							 strcpy(rg.tag,					"300 ");			//session-id
							 strcpy(rg.b_length,			"41  ");		
							 strcpy(rg.value,				"37489237483674893784378256473854378374589");		

							 check = 0;
							 memcpy(&globalbuf, &rg, sizeof(reg_message));
							 check = 4;

						break;

				case 41: printf("\n--------draser-----------up_IMSI---------------\n");

						memset(&rg, 0x00, sizeof(reg_message));
						memset(rbuf, 0x00, sizeof(rbuf));
						memset(globalbuf, 0x00, sizeof(reg_message));

							 strcpy(rg.proto_sp,			"MGW");
							 strcpy(rg.interface_ver,		"VO1");
							 strcpy(rg.ssid,				"DRA-JA    ");
							 strcpy(rg.ms_sp,				"1");
							 strcpy(rg.ts_id,				"12345678");	
							 strcpy(rg.hopeid,				"dd      ");	
							 strcpy(rg.op_code,				"112");				// QOS 제어 요청 메시지
							 strcpy(rg.h_length,			"22  ");		

							 strcpy(rg.tag,					"443 ");			
							 strcpy(rg.b_length,			"14  ");		
							 strcpy(rg.value,				"48343789278690");		

							 check = 0;
							 memcpy(&globalbuf, &rg, sizeof(reg_message));
							 check = 41;

						  break;

				case 42: printf("\n-------draser-------up_Policy_Conter_Number----------\n");

						memset(&rg, 0x00, sizeof(reg_message));
						memset(rbuf, 0x00, sizeof(rbuf));
						memset(globalbuf, 0x00, sizeof(reg_message));

							 strcpy(rg.proto_sp,			"MGW");
							 strcpy(rg.interface_ver,		"VO1");
							 strcpy(rg.ssid,				"DRA-JA    ");
							 strcpy(rg.ms_sp,				"1");
							 strcpy(rg.ts_id,				"12345678");	
							 strcpy(rg.hopeid,				"dd      ");	
							 strcpy(rg.op_code,				"112");				// QOS 제어 요청 메시지
							 strcpy(rg.h_length,			"36  ");		

							 strcpy(rg.tag,					"2900");			
							 strcpy(rg.b_length,			"28  ");		
							 strcpy(rg.value,				"3493247582367834578329475895");		

							 check = 0;
							 memcpy(&globalbuf, &rg, sizeof(reg_message));
							 check = 42;
						  break;

				case 43: printf("\n-----draser--------up_Policy_Counter_Identifier----------------\n");

						memset(&rg, 0x00, sizeof(reg_message));
						memset(rbuf, 0x00, sizeof(rbuf));
						memset(globalbuf, 0x00, sizeof(reg_message));

							 strcpy(rg.proto_sp,			"MGW");
							 strcpy(rg.interface_ver,		"VO1");
							 strcpy(rg.ssid,				"DRA-JA    ");
							 strcpy(rg.ms_sp,				"1");
							 strcpy(rg.ts_id,				"12345678");	
							 strcpy(rg.hopeid,				"dd      ");	
							 strcpy(rg.op_code,				"112");				// QOS 제어 요청 메시지
							 strcpy(rg.h_length,			"    ");		

							 strcpy(rg.tag,					"2901");			
							 strcpy(rg.b_length,			"    ");		
							 strcpy(rg.value,				"     ");		

							 check = 0;
							 memcpy(&globalbuf, &rg, sizeof(reg_message));
							 check = 43;
						  break;

				case 44: printf("\n------draser-------up_Policy_Counter_Status--------------------\n");

						memset(&rg, 0x00, sizeof(reg_message));
						memset(rbuf, 0x00, sizeof(rbuf));
						memset(globalbuf, 0x00, sizeof(reg_message));

							 strcpy(rg.proto_sp,			"MGW");
							 strcpy(rg.interface_ver,		"VO1");
							 strcpy(rg.ssid,				"DRA-JA    ");
							 strcpy(rg.ms_sp,				"1");
							 strcpy(rg.ts_id,				"12345678");	
							 strcpy(rg.hopeid,				"dd      ");	
							 strcpy(rg.op_code,				"112");				// QOS 제어 요청 메시지
							 strcpy(rg.h_length,			"    ");		

							 strcpy(rg.tag,					"2902");			
							 strcpy(rg.b_length,			"    ");		
							 strcpy(rg.value,				"     ");		

							 check = 0;
							 memcpy(&globalbuf, &rg, sizeof(reg_message));
							 check = 44;
						  break;


				default: printf("\nUndefined message format!\n");

						 strcpy(rr.recheck,				"Undefined Format TRY AGAIN");
						 memcpy(&rbuf, &rr, sizeof(rech));
						 write(connSock, rbuf, sizeof(rech));
						 memset(&rbuf, 0x00, sizeof(rbuf));
						 break;
			}
			if(strncmp(rbuf, quit, 4) == 0) {
				pthread_kill(pid[0], SIGINT);
				break;
			}
		}else if(rbuf != 1){
			printf("Fail!\n");
			exit(-1);
		}
	}
}


int
pushClient(int connSock){
	int i = 0;

	for(i = 0; i < MAX_CLIENT; i++){
		pthread_mutex_lock(&mutex);
		if(list_c[i] == INVALID_SOCK) {
			list_c[i] = connSock;
			pthread_mutex_unlock(&mutex);
			return i;
		}

		pthread_mutex_unlock(&mutex);
	}
	if(i == MAX_CLIENT)
		return i;
}

int
popClient(int s){
	int i = 0;

	close(s);	

	for(i = 0; i < MAX_CLIENT; i++) {
		pthread_mutex_lock(&mutex);
		if(s == list_c[i]) {
			list_c[i] = INVALID_SOCK;
			pthread_mutex_unlock(&mutex);
			break;
		}

		pthread_mutex_unlock(&mutex);
	}
}


