//swap msb and lsb of a num
#include <stdio.h>

int main(){
	int num = 171;

	int lsb = (num & 1);
	int msb = (num & 128);
	printf("lsb %d msb %d\n",lsb,msb);

	int res = (lsb << 7 | num) | (msb >> 7 | num);
	printf("%d\n",res);
}
