#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 256

extern char globalbuf[BUFSIZ];
extern char reglobalbuf[BUFSIZ];
extern int check;
extern int recheck;

extern int tcheck;

typedef struct {
	char proto_sp[3];		//프로토콜 구분자
	char interface_ver[3];	//인터페이스 버전
	char ssid[10];			//소스 시스템아이디
	char ms_sp[1];			//메신저 구분자 *decimal char
	char ts_id[8];			//트랜잭션 아이디
	char hopeid[8];			//invok return ms
	char op_code[3];		//operation sp  *decimal char
	char h_length[4];		//?		//header를 제외한 body의 전체 길이 *decimal
	char tag[4];			//operation별 TAG  *decimal
	char b_length[4];			//value의 길이
	char value[BUFSIZ];			//값들...
}reg_message;

typedef struct {
	char proto_sp[3];		//프로토콜 구분자
	char interface_ver[3];	//인터페이스 버전
	char ssid[10];			//소스 시스템아이디
	char ms_sp[1];			//메신저 구분자 *decimal char
	char ts_id[8];			//트랜잭션 아이디
	char hopeid[8];			//invok return ms
	char op_code[3];		//operation sp  *decimal char
	char h_length[4];		//?		//header를 제외한 body의 전체 길이 *decimal
	char tag[4];			//operation별 TAG  *decimal
	char b_length[4];			//value의 길이
	char value[BUFSIZ];			//값들...
}recv_message;

typedef struct {
	char length[BUFSIZ];
}length;

typedef struct {
	char recheck[17];
}rech;

reg_message m1;
reg_message *mglobalbuf;

extern void *thread_client() ;
extern void *DRA_MGW_cla(void *data) ;
extern void *DRA_MGW_ser(void *data) ;
