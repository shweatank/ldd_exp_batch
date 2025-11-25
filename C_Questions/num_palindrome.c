#include <stdio.h>

int main(){
	int a = 121;
	int hold = a;
	int rev = 0;
	int count;

	do{
		count++;
		int tmp = a%10;
		rev = rev * 10 + tmp;
	}while((a/=10) >= 1);
	
	a = hold;

	while(count--){
		if((a%10) == (rev%10)){
			a/=10;
			rev/=10;
		}else{
			printf("not palindrome\n");
			return 0;
		}
	}

	printf("palindrome\n");
}
