#include <stdio.h>

typedef struct Funcs{
	int (*ptr1)(int, int);
	int (*ptr2)(int, int);
	int (*ptr3)(int, int);
	int (*ptr4)(int, int);
}Funcs;

int add (int a, int b);
int sub (int a, int b);
int mul (int a, int b);
int div (int a, int b);

int main(){
	int a,b,op;
	// Creating an instance of the struct Funcs
	Funcs f1;
	printf("Enter two numbers: ");
	scanf("%d %d",&a,&b);
	printf("Select the operation you want to perform\n1.Add\n2.Sub\n3.Mul\n4.Div\n");
        scanf("%d",&op);

	f1.ptr1 = add;
	f1.ptr2 = sub;
	f1.ptr3 = mul;
	f1.ptr4 = div;

	if(op == 1){
		printf("The result of addition is : %d\n",f1.ptr1(a,b));
	}else if(op == 2){
		printf("The result of subtraction is : %d\n",f1.ptr2(a,b));
	}else if(op == 3){
		printf("The result of multiplication is : %d\n",f1.ptr3(a,b));
	}else if(op == 4){	
		printf("The result of division is : %d\n",f1.ptr4(a,b));
	}
}

/*Function definitions for creating the calculator*/
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

