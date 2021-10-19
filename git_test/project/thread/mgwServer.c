/************
 *2021/01/14*
 * 허진우   *
 * mgwServer*
 ************/
#include "headers.h"

#define PORT 10002

void *do_keyboard(void*);
void *do_socket(void*);

char quit[ ] = "exit";

pthread_t pid[2];
int checkMgw = 0;

int main(){
	int thr_id;
	int status;

	int listenSock, connSock;
	struct sockaddr_in client_addr, server_addr;

	int len;

	if(PORT<2){
		printf("Usage: talk_server port_num");
		return -1;
	}

	if((listenSock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Server: Can't open socket\n");
		return -1;
	}

	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);
	
	if(bind(listenSock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		printf("talk Server Can't bind \n");
		return -1;
	}
	listen(listenSock, 1);

	len = sizeof(client_addr);
	if((connSock = accept(listenSock,(struct sockaddr *) &client_addr, &len)) < 0){

		printf("Talk Server failed in accepting\n"); //여기서 못넘어간다.
		return -1;
	}

	printf("Talk Server accept new request\n");
	thr_id = pthread_create(&pid[0], NULL, do_keyboard, (void *)connSock);

	thr_id = pthread_create(&pid[1], NULL, do_socket, (void *)connSock);

	pthread_join(pid[0], (void **) &status);

	pthread_join(pid[1], (void **) &status);

	close(listenSock);
	close(connSock);
}

void *
do_keyboard(void *data)
{
	int n = 0;
	char sbuf[BUFSIZ] = {0, };
	int	connSock = (int) data;



	while((n = read(0, sbuf,BUFSIZ)) > 0) {
		printf("%s", sbuf);
		if(write(connSock, sbuf, n) != n) {
			printf("Talk Server fail in sending\n");
		}
		if(strncmp(sbuf, quit, 4) == 0) {
			pthread_kill(pid[1], SIGINT);
			break;
		}
	}
}

void *
do_socket(void *data)
{
	printf("MGW Socket\n");
	int		n				=	0;
	int		i				=	0;
	int		j				=	0;
	int		k				=	0;
	int		l				=	0;
	int		m				=	0;
	int		o				=	0;
	char	rbuf[BUFSIZ]	= {0, };	
	char	sbuf[BUFSIZ]	= {0, };
	char	res[BUFSIZ]		= {0, };
	char	resu[BUFSIZ]	= {0, };
	int		connSock		= (int) data;
	int		z				=	0;

	recv_message rm;
	reg_message test_data ;
	memset ( &test_data,	0x00, sizeof(reg_message) );
	memset ( &rm,			0x00, sizeof(recv_message) );

	while(1){
		if((n = read(connSock, rbuf, BUFSIZ)) > 0) {

			strncpy ( test_data.op_code, rbuf+33,	sizeof(test_data.op_code) ) ;
			test_data.op_code[3] = '\0' ;

			z = atoi(test_data.op_code);

			if(n > 0){

				switch(z) {
					case(001): printf("\n001\n");

							   printf("test len1-> %d\n",				sizeof(test_data.proto_sp));
							   printf("test len2-> %d\n",				sizeof(test_data.interface_ver));
							   printf("test len3-> %d\n",				sizeof(test_data.ssid));
							   printf("test len4-> %d\n",				sizeof(test_data.ms_sp));

							   strncpy ( test_data.proto_sp, rbuf+0,	sizeof(test_data.proto_sp) ) ;
							   test_data.proto_sp[3] = '\0' ;

							   strncpy ( test_data.interface_ver, rbuf+3, sizeof(test_data.interface_ver) ) ;
							   test_data.interface_ver[3] = '\0' ;

							   strncpy ( test_data.ssid, rbuf+6,		sizeof(test_data.ssid) ) ;
							   test_data.ssid[10] = '\0' ;

							   strncpy ( test_data.ms_sp, rbuf+16,		sizeof(test_data.ms_sp) ) ;
							   test_data.ms_sp[1] = '\0' ;

							   strncpy ( test_data.ts_id, rbuf+17,		sizeof(test_data.ts_id) ) ;
							   test_data.ts_id[8] = '\0' ;

							   strncpy ( test_data.hope_id, rbuf+25,	sizeof(test_data.hope_id) ) ;
							   test_data.hope_id[8] = '\0' ;

							   strncpy ( test_data.op_code, rbuf+33,	sizeof(test_data.op_code) ) ;
							   test_data.op_code[3] = '\0' ;

							   strncpy ( test_data.h_length, rbuf+36,	sizeof(test_data.h_length) ) ;
							   test_data.h_length[4] = '\0' ;



							   strncpy ( test_data.tag, rbuf+40,		sizeof(test_data.tag) ) ;
							   test_data.tag[4] = '\0' ;

							   strncpy ( test_data.b_length, rbuf+44,	sizeof(test_data.b_length) ) ;
							   test_data.b_length[4] = '\0' ;

							   strncpy ( test_data.value, rbuf+48,		sizeof(test_data.value) ) ;
							   test_data.value[BUFSIZ] = '\0' ;


							   printf("test 0-> %s\n",					test_data.proto_sp);
							   printf("test 1-> %s\n",					test_data.interface_ver);
							   printf("test 2-> %s\n",					test_data.ssid);
							   printf("test 3-> %s\n",					test_data.ms_sp);
							   printf("test 4-> %s\n",					test_data.ts_id);
							   printf("test 5-> %s\n",					test_data.hope_id);
							   printf("test 6-> %s\n",					test_data.op_code);
							   printf("test 7-> %s\n",					test_data.h_length);
							   printf("test 8-> %s\n",					test_data.tag);
							   printf("test 9-> %s\n",					test_data.b_length);
							   printf("test 10-> %s\n",				test_data.value);


							   printf("mgw-> %s\n",					rbuf);


							   if (strcmp(									"MGW",			test_data.proto_sp)			== 0) {
								   if(strcmp(								"VO1",			test_data.interface_ver)	== 0) {
									   if(strcmp(							"DRA-JA    ",	test_data.ssid)				== 0) {
										   if(strcmp(						"1",			test_data.ms_sp)			== 0) {
											   if(strcmp(					"12345678",		test_data.ts_id)			== 0) {
												   if(strcmp(				"dd      ",		test_data.hope_id)			== 0) {
													   if(strcmp(			"001",			test_data.op_code)			== 0) {

														   strcpy(rm.proto_sp,			"MGW");
														   strcpy(rm.interface_ver,	"VO1");
														   strcpy(rm.ssid,				"DRA-JA    ");
														   strcpy(rm.ms_sp,			"1");
														   strcpy(rm.ts_id,			"12345678");
														   strcpy(rm.hope_id,          "dd      ");
														   strcpy(rm.op_code,			"001");        
														   strcpy(rm.h_length,			"1234");
														   strcpy(rm.tag,				"268 ");		//reg -> result code
														   strcpy(rm.b_length,         "4   ");
														   strcpy(rm.value,			"2001");

														   memcpy(&sbuf, &rm, sizeof(recv_message));
														   write(connSock, sbuf, sizeof(reg_message));

													   }else if(strcmp("1", test_data.ms_sp) == NULL){
														   printf("\nNo Invoke\n");
														   return 0;

													   }else if(strcmp("DRA-JA    ", test_data.ssid) == NULL){
														   printf("\nNo Source ID\n");
														   return 0;

													   }else if(strcmp("VO1", test_data.interface_ver) == NULL){
														   printf("\nNo Match VERSION\n");
														   return 0;

													   }else if(strcmp("MGW", test_data.proto_sp) == NULL){
														   printf("\nCheck PROTOCOL\n");
														   return 0;

													   }else if(strcmp("MGW", test_data.ts_id) == NULL){
														   printf("\nNO Transaction ID\n");
														   return 0;

													   }else if(strcmp("MGW", test_data.hope_id) == NULL){
														   printf("\nNO HOPE ID\n");
														   return 0;

													   }else if(strcmp("MGW", test_data.op_code) == NULL){
														   printf("\nNO OP_CODE\n");
														   return 0;
													   }
												   }
											   }
										   }
									   }
								   }
							   }
							   break;


							   printf("MGW->DRA,cla succeess\n");
					case 002: printf("\n002\n"); 
							  strncpy ( test_data.proto_sp, rbuf+0,			sizeof(test_data.proto_sp) ) ;
							  test_data.proto_sp[3] = '\0' ;

							  strncpy ( test_data.interface_ver, rbuf+3,	sizeof(test_data.interface_ver) ) ;
							  test_data.interface_ver[3] = '\0' ;

							  strncpy ( test_data.ssid, rbuf+6,				sizeof(test_data.ssid) ) ;
							  test_data.ssid[10] = '\0' ;

							  strncpy ( test_data.ms_sp, rbuf+16,			sizeof(test_data.ms_sp) ) ;
							  test_data.ms_sp[1] = '\0' ;

							  strncpy ( test_data.ts_id, rbuf+17,			sizeof(test_data.ts_id) ) ;
							  test_data.ts_id[8] = '\0' ;

							  strncpy ( test_data.hope_id, rbuf+25,			sizeof(test_data.hope_id) ) ;
							  test_data.hope_id[8] = '\0' ;

							  strncpy ( test_data.op_code, rbuf+33,			sizeof(test_data.op_code) ) ;
							  test_data.op_code[3] = '\0' ;

							  strncpy ( test_data.h_length, rbuf+36,		sizeof(test_data.h_length) ) ;
							  test_data.h_length[4] = '\0' ;



							  strncpy ( test_data.tag, rbuf+40,				sizeof(test_data.tag) ) ;
							  test_data.tag[4] = '\0' ;

							  strncpy ( test_data.b_length, rbuf+44,		sizeof(test_data.b_length) ) ;
							  test_data.b_length[4] = '\0' ;

							  strncpy ( test_data.value, rbuf+48,			sizeof(test_data.value) ) ;
							  test_data.value[BUFSIZ] = '\0' ;


							  printf("mgw-> %s\n",					rbuf);


							  if (strcmp(									"MGW",			test_data.proto_sp)			== 0) {
								  if(strcmp(								"VO1",			test_data.interface_ver)	== 0) {
									  if(strcmp(							"DRA-JA    ",	test_data.ssid)				== 0) {
										  if(strcmp(						"1",			test_data.ms_sp)			== 0) {
											  if(strcmp(					"12345678",		test_data.ts_id)			== 0) {
												  if(strcmp(				"dd      ",		test_data.hope_id)			== 0) {
													  if(strcmp(			"002",			test_data.op_code)			== 0) {	//연결 해지 요청

														  strcpy(rm.proto_sp,				"MGW");
														  strcpy(rm.interface_ver,			"VO1");
														  strcpy(rm.ssid,					"DRA-JA    ");
														  strcpy(rm.ms_sp,					"1");
														  strcpy(rm.ts_id,					"12345678");
														  strcpy(rm.hope_id,				"dd      ");
														  strcpy(rm.op_code,				"002");        
														  strcpy(rm.h_length,				"1234");
														  strcpy(rm.tag,					"268 ");		//reg -> result code
														  strcpy(rm.b_length,				"4   ");
														  strcpy(rm.value,					"2001");

														  memcpy(&sbuf, &rm, sizeof(recv_message));
														  write(connSock, sbuf, sizeof(reg_message));

													  }else if(strcmp("1", test_data.ms_sp) == NULL){
														  printf("\nNo Invoke\n");
														  return 0;

													  }else if(strcmp("DRA-JA    ", test_data.ssid) == NULL){
														  printf("\nNo Source ID\n");
														  return 0;

													  }else if(strcmp("VO1", test_data.interface_ver) == NULL){
														  printf("\nNo Match VERSION\n");
														  return 0;

													  }else if(strcmp("MGW", test_data.proto_sp) == NULL){
														  printf("\nCheck PROTOCOL\n");
														  return 0;

													  }else if(strcmp("MGW", test_data.ts_id) == NULL){
														  printf("\nNO Transaction ID\n");
														  return 0;

													  }else if(strcmp("MGW", test_data.hope_id) == NULL){
														  printf("\nNO HOPE ID\n");
														  return 0;

													  }else if(strcmp("MGW", test_data.op_code) == NULL){
														  printf("\nNO OP_CODE\n");
														  return 0;
													  }
												  }
											  }
										  }
									  }
								  }
							  }
							  break;

					case 003: printf("\n003\n"); 
							  strncpy ( test_data.proto_sp, rbuf+0,			sizeof(test_data.proto_sp) ) ;
							  test_data.proto_sp[3] = '\0' ;

							  strncpy ( test_data.interface_ver, rbuf+3,	sizeof(test_data.interface_ver) ) ;
							  test_data.interface_ver[3] = '\0' ;

							  strncpy ( test_data.ssid, rbuf+6,				sizeof(test_data.ssid) ) ;
							  test_data.ssid[10] = '\0' ;

							  strncpy ( test_data.ms_sp, rbuf+16,			sizeof(test_data.ms_sp) ) ;
							  test_data.ms_sp[1] = '\0' ;

							  strncpy ( test_data.ts_id, rbuf+17,			sizeof(test_data.ts_id) ) ;
							  test_data.ts_id[8] = '\0' ;

							  strncpy ( test_data.hope_id, rbuf+25,			sizeof(test_data.hope_id) ) ;
							  test_data.hope_id[8] = '\0' ;

							  strncpy ( test_data.op_code, rbuf+33,			sizeof(test_data.op_code) ) ;
							  test_data.op_code[3] = '\0' ;

							  strncpy ( test_data.h_length, rbuf+36,		sizeof(test_data.h_length) ) ;
							  test_data.h_length[4] = '\0' ;



							  strncpy ( test_data.tag, rbuf+40,				sizeof(test_data.tag) ) ;
							  test_data.tag[4] = '\0' ;

							  strncpy ( test_data.b_length, rbuf+44,		sizeof(test_data.b_length) ) ;
							  test_data.b_length[4] = '\0' ;

							  strncpy ( test_data.value, rbuf+48,			sizeof(test_data.value) ) ;
							  test_data.value[BUFSIZ] = '\0' ;


							  printf("mgw-> %s\n",					rbuf);


							  if (strcmp(									"MGW",			test_data.proto_sp)			== 0) {
								  if(strcmp(								"VO1",			test_data.interface_ver)	== 0) {
									  if(strcmp(							"DRA-JA    ",	test_data.ssid)				== 0) {
										  if(strcmp(						"1",			test_data.ms_sp)			== 0) {
											  if(strcmp(					"12345678",		test_data.ts_id)			== 0) {
												  if(strcmp(				"dd      ",		test_data.hope_id)			== 0) {
													  if(strcmp(			"003",			test_data.op_code)			== 0) {

														  strcpy(rm.proto_sp,				"MGW");
														  strcpy(rm.interface_ver,			"VO1");
														  strcpy(rm.ssid,					"DRA-JA    ");
														  strcpy(rm.ms_sp,					"1");
														  strcpy(rm.ts_id,					"12345678");
														  strcpy(rm.hope_id,				"dd      ");
														  strcpy(rm.op_code,				"003"); 
														  strcpy(rm.h_length,				"1234");
														  strcpy(rm.tag,					"268 ");		//reg -> result code
														  strcpy(rm.b_length,				"4   ");
														  strcpy(rm.value,					"2001");

														  memcpy(&sbuf, &rm, sizeof(recv_message));
														  write(connSock, sbuf, sizeof(reg_message));

													  }else if(strcmp("1", test_data.ms_sp) == NULL){
														  printf("\nNo Invoke\n");
														  return 0;

													  }else if(strcmp("DRA-JA    ", test_data.ssid) == NULL){
														  printf("\nNo Source ID\n");
														  return 0;

													  }else if(strcmp("VO1", test_data.interface_ver) == NULL){
														  printf("\nNo Match VERSION\n");
														  return 0;

													  }else if(strcmp("MGW", test_data.proto_sp) == NULL){
														  printf("\nCheck PROTOCOL\n");
														  return 0;

													  }else if(strcmp("MGW", test_data.ts_id) == NULL){
														  printf("\nNO Transaction ID\n");
														  return 0;

													  }else if(strcmp("MGW", test_data.hope_id) == NULL){
														  printf("\nNO HOPE ID\n");
														  return 0;

													  }else if(strcmp("MGW", test_data.op_code) == NULL){
														  printf("\nNO OP_CODE\n");
														  return 0;
													  }
												  }
											  }
										  }
									  }
								  }
							  }
							  break;
				}
				if(strncmp(rbuf, quit, 4) == 0) {
					pthread_kill(pid[0], SIGINT);
					break;
				}
			}
		}
	}
}
