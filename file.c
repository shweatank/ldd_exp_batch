#include<stdio.h>

int main()
{
	FILE*fp;
	char ch;

	fp = fopen("file.txt", "w");
    	fputs("Hello World!\n", fp);
    	fclose(fp);


        fp = fopen("file.txt", "w");
	printf("File content:hello wold\n");
    while ((ch = fgetc(fp)) != EOF) {
        putchar(ch);
    }	

    fclose(fp);

    return 0;
}

