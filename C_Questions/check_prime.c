#include <stdio.h>

int main(){
	//num = 7
	//7 = 7 * 1
	
	int num = 0;
	int flag = 0;
	printf("enter num : ");
	scanf("%d",&num);

	for(int i = 2; i*i <= num; i++){
		if((num%i) == 0){
			printf("%d ",(num/i));
			flag = 1;
			//break;
		}
		//flag = 0;
	}

	if(flag)
		printf("not prime \n");
	else
		printf("prime\n");
}
