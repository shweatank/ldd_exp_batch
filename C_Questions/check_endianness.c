#include <stdio.h>

int main(){
	union func{
		int a;
		char b[4];
	};

	union func tmp;
	tmp.a = 0xaabbccdd;

	printf("Size of a union: %ld\n",sizeof(tmp));
	printf("little endian representation: %x\n",(unsigned char)tmp.b[0]);
	
	int c = 1;

	char *ptr = (char *)&c;
	if(*ptr){
		printf("little endian\n");
	}else{
		printf("big endian\n");
	}

}
