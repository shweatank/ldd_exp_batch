#include <stdio.h>


int main(){
	
	int num = 123;
	int tmp = 0; 
	int newtmp = 0;

	do{
		tmp = num%10;
		newtmp = newtmp * 10 + tmp;
	       	
	
	}while(num/=10);
	printf("rev number is : %d\n",newtmp);

}
