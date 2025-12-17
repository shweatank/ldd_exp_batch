#include<stdio.h>
int main()
{
	int a=10;
	int*ptr;
	ptr=&a;
	printf("the value of a and a+1 is %d %d\n",a,a+1);
	printf("the value of a is %d\n",a+1);
	printf("the value of ptr is %p\n",ptr);
	printf("the value of ptr is %d\n",*ptr);
	printf("the value of ptr is %p\n",(ptr+1));
	*ptr=20;
	printf("value of a os %d\n",*ptr);
	return 0;
}
