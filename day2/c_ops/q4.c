#include <stdio.h>

int main(){
	int x = 3;
	printf("The evaluated exp is : %d",(x+=x*=x+1));
}
