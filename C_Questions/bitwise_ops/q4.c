//swap two bits using bit manipulation
#include <stdio.h>

int main(){
	int a = 5;
	int b = 6;
	printf("before a: %d b: %d\n",a,b);
	a = a^b;
	b = b^a;
	a = a^b;
	printf("after a: %d b: %d",a,b);
}
