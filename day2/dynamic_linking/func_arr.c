#include <stdio.h>

//Function prototype
int add(int a, int b);
int sub(int a, int b);
int div(int a, int b);
int mul(int a, int b);

int main(){
	
	int a, b;
	int op = 0;

	printf("Enter Two numbers:");
	scanf("%d %d",&a,&b);
	
	printf("Enter the operation:\n1.To add the numbers\n2.To subtract the numbers\n3.To divide the numbers\n4.To Multiply the numbers\n");
	scanf("%d",&op);

	int (*ptr1)(int, int) = add; 
	int (*ptr2)(int, int) = sub; 
	int (*ptr3)(int, int) = mul; 
	int (*ptr4)(int, int) = div; 
	int (*func_arr[])(int, int) = {add,sub,mul,div};
	//int tmp = 0;

	//iif(op == 0){
	printf("res %d\n",func_arr[op](a,b));
	//}else if(op == 2){
	//	printf("res %d\n",func_arr[op](a,b));
	//}else if(op == 3){
	//	printf("res %d\n",func_arr[op](a,b));
	//}else if(op == 4){
	//	printf("res %d\n",func_arr[op](a,b));
	//}
}

