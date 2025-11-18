#include<stdio.h>

int main()
{
    FILE *file;
    file = fopen("file2.txt","w");
    
    fputs("Hello World!\n",file);
    fputs("Another line",file);
    
    fclose(file);
    
    file = fopen("file2.txt","r");
    char buff[100];

    while (fgets(buff,sizeof(buff),file) != NULL)
    {
        printf("%s",buff);
    }

    fclose(file);

    return 0;
}
