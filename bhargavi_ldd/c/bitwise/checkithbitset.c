#include<stdio.h>
void  checkithbitsetreset(int n,int i)
{       
        int mask=1<<(i-1);
        if((n&mask)!=0)
        {       
                printf("set");
        }       
        else
        {       
                printf("reset");
        }       
}       
int main()
{       
        int n=13,i=2;
        checkithbitsetreset(n,i);
        return 0;
}   
