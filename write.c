#include<stdio.h>
#include<unistd.h>
int main()
{
	int fd;

	write(1,"hello\n",6);
 	return 0;
}

