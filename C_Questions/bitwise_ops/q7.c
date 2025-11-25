//Set bit using preprocessor macro

#include <stdio.h>

#define SETBIT(a,n)(1<<(n-1) | a)
#define CHECKBIT(a,n)(1<<(n-1) & a)

int main(){
	printf("set bit %d\n",SETBIT(5,2));
	if(CHECKBIT(5,2)){
		printf("bit is set\n");
	}else{
		printf("bit not set\n");
	}
	
}
