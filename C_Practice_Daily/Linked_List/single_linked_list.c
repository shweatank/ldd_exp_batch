#include<stdio.h>
#include<stdlib.h>
typedef struct stu{
    int id;
    struct stu *add;
}stu;

stu *create(stu *head);
stu *addbeg(stu *head);
stu *addend(stu *head);
stu *addbef(stu *head);
stu *addaft(stu *head);
stu *addpos(stu *head);
stu *addpos(stu *head);
stu *rev(stu *head);
void dis(stu *head);

int main()
{
int a;
while(1)
{
    printf("1.create a linked list\n");
    printf("2.add a node at the beggining\n");
    printf("3.add a node at the end\n");
    printf("4.add a node at the 
