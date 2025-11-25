#include <stdio.h>

int main(){
	
	int a = 46;

	while(a > 1){
		int tmp = a%4;

		if(tmp){
			printf("not pwr of 4\n");
			//break;
			return 0;

		}else{
			//printf("pwr of 4\n");
			a/=4;
		}
	}

	printf("pwr of 4\n");
}
