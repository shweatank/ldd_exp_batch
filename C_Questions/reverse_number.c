//arr of int 
#include <stdio.h>

int main(){
	int a = -21;	
	int newtmp = 0;
	int sign = 0;
	printf("actual num : %d\n",a);
	do{
			if(a < 0){
				a *= -1;
				sign = 1;
			}
			int tmp = a%10;
			newtmp = newtmp * 10 + tmp;
			
	}while((a/=10) >= 1);
	if(sign)
		newtmp *= -1;
	printf("reversed num :%d\n",newtmp);
}
