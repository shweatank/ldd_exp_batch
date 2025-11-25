//predict the output

#include <stdio.h>
struct flag {
    //int a : 1;
    //int y : 2;
    int a;
    int y;
}flag;

int main(){
	struct flag a;
	printf("size of a : %d\n",sizeof(a));
}

//size of struct if bit field is used is 4 bytes
//size of struct if bit field is not used is 8 bytes
