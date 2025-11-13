#include<stdio.h>
int main()
{
    int s;
    //enter the size of an array.
    scanf("%d",&s);
    int a[s];
    //enter an array elements.
    for(int i=0;i<s;i++)
    scanf("%d",&a[i]);
    //print the array elements.
    for(int j=0;j<s;j++)
    printf("%d ",a[j]);

    return 0;
}

