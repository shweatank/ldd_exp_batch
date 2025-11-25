#include <stdio.h>

int main(){
	int a = 167;
	int sum = 0;
	while(a){
		sum+=(a%10);
		a/=10;
	}

	printf("sum of a number: %d\n",sum);
}
