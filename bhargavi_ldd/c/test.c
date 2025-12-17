#include<stdio.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
    char filename[100];
    char buffer[256];
	printf("Enter file name to open: ");
    scanf("%s", filename);
	FILE *fp= fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Error: Cannot open file %s\n", filename);
        return 1;
    }
    printf("\nFile Content\n");
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        printf("%s", buffer);
    }

    fclose(fp);
    return 0;
}

