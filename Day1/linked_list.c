#include<stdio.h>
#include<stdlib.h>
typedef struct stu{
int id;
struct stu *add;
}stu;

stu *create(stu *head);
stu *sort(stu *head);
void dis(stu *head);
stu *head=NULL,*new=NULL,*temp=NULL;
int main()
{
int a;
while(1)
{
printf("1.create a linked list\n");
printf("2.sort a linked list\n");
printf("3.display\n");
printf("4.exit\n");
scanf("%d",&a);
switch(a)
{
case 1:head=create(head);
        break;
case 2:head=sort(head);
        break;
case 3:dis(head);
        break;
case 4:exit(0);
        break;
default:printf("chooese correct option\n");
}
}
return 0;
}

stu *create(stu *head)
{
    char c;   
    while(1)
{       
    new=(stu *)malloc(sizeof(stu));
    scanf("%d",&new->id);
    new->add=NULL;
    if(head==NULL)
    {
        head=new;
        temp=new;
    }
    else
    {
    temp->add=new;
    temp=new;
    }
    printf("if u want to continue\n");
    scanf(" %c",&c);
    if(c!='y')
    return head;
}
}

stu *sort(stu *head)
{
    for(stu *i=head;i!=NULL;i=i->add)
    {
        for(stu *j=i->add;j!=NULL;j=j->add)
        {
            if(i->id > j->id)   
            {
                i->id^=j->id;
                j->id^=i->id;
                i->id^=j->id;
            }
        }
    }
    return head;
}

void dis(stu *head)
{
stu *y=head;
    while(y)
    {
        printf("%d--->",y->id);
        y=y->add;
    }
}
