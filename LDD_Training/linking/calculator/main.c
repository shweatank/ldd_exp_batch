#include "add.h"
#include "sub.h"
#include "mul.h"
#include "div.h"


int main() {
	int a,b;
	printf("Enter a and b: ");
	scanf("%d %d", &a, &b);

	char op;
	printf("Enter oper: ");
	scanf("%c", &op);

	switch(op) {
		case '+':
			printf("Add: %d\n", add(a,b));
			break;
		case '-':
			printf("Add: %d\n", sub(a,b));
			break;
		case '*':
			printf("Add: %d\n", mul(a,b));
			break;
		case '/':
			printf("Add: %d\n", div(a,b));
			break;
		default:
			break;
	}
