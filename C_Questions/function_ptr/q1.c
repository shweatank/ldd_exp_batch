#include <stdio.h>

/* These are the function signature
 * or
 * Function prototype
 * */

int add (int a, int b);
int sub (int a, int b);
int mul (int a, int b);
int div (int a, int b);

/*
 * Using Function pointer as a callback
 * */

int calc(int a, int b, int (*funcp)(int,int));

int main(){
	int a,b,op;
	a = b = op = 0;
	//array of function pointer
	int (*arrf[])(int, int) = {add, sub, mul, div};
	printf("Enter two numbers: ");
	scanf("%d %d",&a,&b);

	printf("Select the operation you want to perform\n1.Add\n2.Sub\n3.Mul\n4.Div\n");
	scanf("%d",&op);

	if(op == 1){
		calc(a,b,add);
	}else if(op == 2){
		calc(a,b,sub);
	}else if(op == 3){
		calc(a,b,mul);
	}else if(op == 4){
		calc(a,b,div);
	}
	printf("Result using function pointers : %d\n",arrf[op-1](a,b));
	//Function pointer declaration
//	void (*fptr)(int, int) = add;
//	//fptr = &add;
//	printf("%d\n",fptr(2,3));
}

int calc(int a, int b, int (*funcp)(int c, int d)){
	printf("%d\n",funcp(a,b));
	return 0;
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
