/**
* super_simple_userspace_driver_struct.c
*
* Same simple open/read/write/close "driver",
	* but now the APIs are stored in a struct and called via that struct.
	*/

#include <stdio.h>
#include <string.h>

/* -------------------------------
Device: simple buffer + state
------------------------------- */
char device_buffer[64];
int  device_is_open = 0;

/* -------------------------------
   Driver API functions
   ------------------------------- */
int add(int a, int b){
	return a+b;
}

int sub(int a, int b){
	return a-b;
}

int mul(int a, int b){
	return a*b;
}

int div(int a, int b){
	return a/b;
}

struct calc_ops {
	int (*padd)(int, int);
	int (*psub)(int, int);
	int (*pmul)(int, int);
	int (*pdiv)(int, int);
};

/* Initialize the ops table */
struct calc_ops calculator = {
	.padd   = add,
	.psub   = sub,
	.pmul   = mul,
	.pdiv   = div
};

/* -------------------------------
   Demo / test
   ------------------------------- */

int main(void)
{
	char temp[64];
	int a, b;
	int op;
	printf("enter the numbers : ");
	scanf("%d %d",&a, &b);

	printf("Enter the op\n1.add\n2.sub\n3.mul\n4.div\n");
	scanf("%d",&op);

	if(op == 1){
		printf("The result is : %d\n",calculator.padd(a,b));
	}else if(op == 2){

		printf("The result is : %d\n",calculator.psub(a,b));
	
	}else if(op == 3){

		printf("The result is : %d\n",calculator.pmul(a,b));
	
	}else if(op == 4){

		printf("The result is : %d\n",calculator.pdiv(a,b));
	
	}

	return 0;
}
