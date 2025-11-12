#include <stdio.h>

typedef union FuncU{
	struct FuncS{
		int (*ptr1)(int, int);
		int (*ptr2)(int, int);
		int (*ptr3)(int, int);
		int (*ptr4)(int, int);
	}f2;

	int (*arrp[4])(int, int);	
}FuncU;

int add(int a, int b);
int sub(int a, int b);
int mul(int a, int b);
int div(int a, int b);

int main(){
	/*one way of init. array of func ptrs
	 * */
	//FuncU f1 =  { .arrp = {add,sub,mul,div}};
	FuncU f1;
	int a,b,op;
	a = b = op = 0;
	
	printf("Enter the number :");
	scanf("%d %d",&a,&b);
	
	printf("Enter the op\n1.add\n2.sub\n3.mul\n4.div\n");
	scanf("%d",&op);

	f1.f2.ptr1 = add;
	f1.f2.ptr2 = sub;
	f1.f2.ptr3 = mul;
	f1.f2.ptr4 = div;
	
	f1.arrp[0] = add;
	f1.arrp[1] = sub;
	f1.arrp[2] = mul;
	f1.arrp[3] = div;
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
