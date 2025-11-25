//explain and implement constant pointer

#include <stdio.h>
#include <stdlib.h>

int main(){
	int a  = 5;
	int *const ptr = &a;
	ptr++;
}
