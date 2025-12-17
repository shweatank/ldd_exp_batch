//enter firdt 5 numbers in a child and next 5 numbers in parent


#include<stdio.h>
#include<string.h>
#include<string.h>
#include<time.h>
int main(int argc,char* argv[])
{
	int id=fork();
	int n;
	if(id==0)
	{
		n=1;
	}
	else
	{
		n=6;
	}
	if (id!=0)
	{
	//	wait();
	}
	int i;
	for(int i=n;i<n+5;i++)
	{
		printf("%d\n",i);
		fflush(stdout);
	}
	if(id!=0)
	{
		printf("\n");
	}
}

