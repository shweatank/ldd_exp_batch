#include <stdio.h>

typedef struct Operations {
	int (*add)(int, int);
	int (*sub)(int, int);
	int (*mul)(int, int);
	int (*div)(int, int);
}Operations_t;

int addition(int a, int b) {
	return a+b;
}

int subtraction(int a, int b) {
	return a-b;
}

int multiply(int a, int b) {
	return a*b;
}

int division(int a, int b) {
	return a/b;
}

Operations_t Op1 = {
	.add = addition,
	.sub = subtraction,
	.mul = multiply,
	.div = division
};


int main () {
	printf("2+3 = %d\n",Op1.add(2,3));
	printf("2-3 = %d\n",Op1.sub(2,3));
	printf("2*3 = %d\n",Op1.mul(2,3));
	printf("2/3 = %d\n",Op1.div(2,3));
	return 0;
}
