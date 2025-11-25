#include <stdio.h>

int main(){
	char *s = "42";

	int tmpp = 0;
	int  i = 0;

//	do{
//		
//		int store = *s - '0';
//		
//		printf("store : %d\n",store);
//		tmpp = tmpp * 10 + store;
//		printf("tmp : %d\n",tmpp);
//
//	}while(*++s != '\0');
	
	while(*s != '\0'){
		int store = *s - '0';
		tmpp = tmpp * 10 + store;
		s++;
	}
	printf("num: %d\n",tmpp);
}
