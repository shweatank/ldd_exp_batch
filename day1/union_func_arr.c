#include <stdio.h>

typedef union Calc{
	int (*fptr[4])(int, int);
}Calc;

int add(int, int);
int sub(int, int);
int mul(int, int);
int div(int, int);

int main(){

	int a, b, op;
	Calc func;
	//wrong syntax
	//func.fptr[4] = {add, sub, mul, div};
	func.fptr[0] = add;
	func.fptr[2] = sub;
	func.fptr[2] = mul;
	func.fptr[3] = div;

	printf("Enter the numbers: ");
	scanf("%d %d",&a,&b);

	printf("Enter the op you wish to perform:\n1.add\n2.sub\n3.mul\n4.div\n");
	scanf("%d",&op);

	printf("Result : %d\n",func.fptr[op-1](a,b));

}

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
