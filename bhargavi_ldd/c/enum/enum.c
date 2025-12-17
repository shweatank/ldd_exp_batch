#include<stdio.h>
typedef enum week
{
	monday,
	tuesday,
	wednesday,
	thursday,
	friday,
	saturday,
}week;
int main()
{
	week today=friday;
	printf("today is %d\n",friday);
	switch(today)
	{
		case monday:
			printf("week day\n");
			break;
		case friday:
			printf("weekend\n");
			break;
		case saturday:
			printf("relax\n");
			break;
	}
}
