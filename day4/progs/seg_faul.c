#include <stdio.h>

int main(){
	int *ptr = NULL;
	*ptr = 2;
	printf("seg faul: %d\n",*ptr);
}
