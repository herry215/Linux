#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 256

typedef struct {
	char val[BUFSIZ];
}value;


int main()
{
	FILE *fp;
	char buffer[BUFFER_SIZE +1];
	int i = 0;
	char num[BUFSIZ]= {0, }; //일반적인 캐릭터 형 말고, 배열로 선언을 해라.
	int len = 0;
	int sum = 0;
	int sum1 = 0;
	int sum2 = 0;
	int sum3 = 0;

	value vv;


	if((fp = fopen("/heoju/project/kTtxt/list/ktv.txt", "r")) != NULL) {
		memset(buffer, 0x00, sizeof(buffer));
		while(fread(buffer , 1, BUFFER_SIZE, fp) != 0)
			printf("%s\n", buffer);
		
//		strcpy(vv.val, buffer);
		i = strlen(buffer);
		printf("%d\n", i);

		len = sizeof(i);
		printf("%d\n", len);

		printf("결 과 %d\n", i+len);
		
		sum = i+len;
		printf("결과2: %d\n", sum);

		sprintf(num,"%d\n", i);
		printf("1 %s\n", num);
		strcpy(vv.val, num);
		printf("%-*s\n",4, vv); //<- 이것.




//		for(i = 0; i < sizeof(value); i++){
//		printf("value->>%d", buffer);
//		}
		fclose(fp);
	}
}
