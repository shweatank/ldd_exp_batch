#include<stdio.h>
#include<stdlib.h>
int main()
{
int rows=3;
int coloums=3;

// memory allocation

int**arr=(int**)malloc(rows*sizeof(int));
for(int i=0;i<rows;i++)
{
arr[i]=(int*)malloc(coloums*sizeof(int));
}

int temp = 1;

// initialization

for(int i=0;i<3;i++)
{
for(int j=0;j<3;j++)
{
arr[i][j] =temp;
temp+=3;
}
}

// printing

for(int i=0;i<3;i++)
{
for(int j=0;j<3;j++){
printf("%d ", arr[i][j]);
 }
printf("\n");
}
}
