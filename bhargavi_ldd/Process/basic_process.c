#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
int main(int argc,char*argv[])
{
	int id=fork();
	if(id != 0)
	{
		fork();
	}
	printf("hello world\n");
	if(id==0)
	{
		printf("hello from child\n");
	}
	else
	{
		printf("hello from parent\n");
	}
}

	
