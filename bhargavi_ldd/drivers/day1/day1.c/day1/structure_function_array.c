#include <stdio.h>

typedef struct Calc{
	int (*padd)(int,int);
	int (*psub)(int,int);
	int (*pmul)(int,int);
	int (*pdiv)(int,int);
}Calc;

int add(int, int);
int sub(int, int);
int mul(int, int);
int div(int, int);

int main(){

	int a, b, op;
	Calc func = {add, sub, mul, div};
	printf("Enter the numbers: ");
	scanf("%d %d",&a,&b);

	printf("Enter the op you wish to perform:\n1.add\n2.sub\n3.mul\n4.div\n");
	scanf("%d",&op);

	if(op == 1){

		printf("Result of addition is : %d\n",func.padd(a,b));
	
	}else if(op == 2){
	
		printf("Result of subtraction is : %d\n",func.psub(a,b));
	
	}else if(op == 3){
	
		printf("Result of multiplication is : %d\n",func.pmul(a,b));
	
	}else if(op == 4){
	
		printf("Result of division is : %d\n",func.pdiv(a,b));
	}

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
