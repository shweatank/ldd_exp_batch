#include<stdio.h>
#include<string.h>
struct student
{
	int roll;
	float marks;
	char name[20];
};
int main()
{
	struct student s1={1,24,"bhargavi"};
	struct student *ptr=&s1;
	printf("roll=%d\n",s1.roll);
	printf("marks=%.2f\n",s1.marks);
	printf("name=%s\n",s1.name);
	return 0;
}

