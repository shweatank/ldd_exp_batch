#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
    char w[]="sai varshath konduri";
    char r[100];
    FILE *fp=fopen("example.txt","w");
    fwrite(w,sizeof(char),strlen(w),fp);
    fclose(fp);
    
    fopen("example.txt","r");
    int n=fread(r,sizeof(char),sizeof(r)-1,fp);

    r[n]='\0';

    fclose(fp);
    printf("Data Read From File:--\n%s\n",r);
    return 0;
}
