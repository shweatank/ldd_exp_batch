#include <stdio.h>
int fact(int n){
	if(n == 0)
		return 1;
	return n * fact(n-1);
}

int main(){
	int a = 9;
	
	int res = fact(9);	
	printf("the factorial of the number is : %d\n",res);
}
