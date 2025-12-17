#include<stdio.h>
#include<string.h>
int main()
{
	char str[]="hello bhargavi";
	int count[256]={0};

	//count
	for(int i=0;str[i]!=0;i++)
	{
		count[str[i]]++;
	}
	printf("character frequencies are :\n");
	for(int i=0;i<256;i++)
	{
		if(count[i]>0)
		{
			printf("%c    %d     ",i,count[i]);
		}
	}
}





