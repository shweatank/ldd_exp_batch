#include<stdio.h>
int main()
{
    int s;
    //enter the size of an array.
    scanf("%d",&s);
    int a[s];  
    //enter elements.
    for(int i=0;i<s;i++)
    scanf("%d",&a[i]);
    int max=a[0],min=a[0];
    //finding the maximum and minimum elements in an array.
    for(int i=0;i<s;i++)
    {
        if(a[i]>max)
            max=a[i];
        if(a[i]<min)
            min=a[i];
    }
    printf("maximum:--%d minimum:--%d\n",max,min);
    return 0;
}
    
