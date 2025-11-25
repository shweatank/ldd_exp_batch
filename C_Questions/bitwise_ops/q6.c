//clear the last set bit 

#include <stdio.h>

int main(){
	
	int num = 166;
	//10100110
	for(int i = 0; i < 8; i++){
		if((1<<i) & num){
			printf("Bit is set\n");
			num = ~(1<<i) & num;
			printf("%d\n",num);
			break;
		}
	}
}
