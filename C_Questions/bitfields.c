#include <stdio.h>
struct func{
	int a:1;
	int b:2;
	int c:3;
	char d;
}func;

int main(){
	struct func f1;
	//f1.a = 2;
	printf("%d",sizeof(func));
	int a = 1;
	char *ptr = (char *)&a;

	if(*ptr){
		printf("little endian\n");
	}else{
		printf("big endian\n");
	}
	
}

// -2 
// 10
// 01*
