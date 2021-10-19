#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
extern int checkMgw;

typedef struct{
	char proto_sp[3];		//프로토콜 구분자
	char interface_ver[3];	//인터페이스 버전
	char ssid[10];			//소스 시스템아이디
	char ms_sp[1];			//메신저 구분자 *decimal char
	char ts_id[8];			//트랜잭션 아이디
	char hope_id[8];			//invok return ms
	char op_code[3];		//operation sp  *decimal char					
	char h_length[4];		//?		//header를 제외한 body의 전체 길이 *decimal

	char tag[4];			//operation별 TAG  *decimal
	char b_length[4];			//value의 길이
	char value[BUFSIZ];			//값들...
}recv_message;


typedef struct {
	char proto_sp[4];		//프로토콜 구분자
	char interface_ver[4];	//인터페이스 버전
	char ssid[11];			//소스 시스템아이디
	char ms_sp[2];			//메신저 구분자 *decimal char
	char ts_id[9];			//트랜잭션 아이디
	char hope_id[9];			//invok return ms
	char op_code[4];		//operation sp  *decimal char					
	char h_length[5];		//?		//header를 제외한 body의 전체 길이 *decimal

	char tag[5];			//operation별 TAG  *decimal
	char b_length[5];			//value의 길이
	char value[BUFSIZ];			//값들...

}reg_message;

#if 0
	char session_id[128];
	char imsi[15];
	char policy_conter_number[2];
	char policy_counter_identifier[16];

#endif
