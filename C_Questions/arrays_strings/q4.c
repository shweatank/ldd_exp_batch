//W.A.P to fix the address of a variable
//not possible
#include <stdio.h>
#include <stdlib.h>

//int * const nptr = NULL;

const int a  = 5;
int main(){
	int *const nptr = malloc(sizeof(int));

	int a  = 5;
	int b  = 10;

	int *ptr = NULL;
	ptr = (int *)0x7ffc45e58d98;
	//ptr = &a;

	printf("ptr : %p\n",ptr);
	a = b;
	printf("add : %p\n",&a);
	printf("add : %p\n",&a);
}
