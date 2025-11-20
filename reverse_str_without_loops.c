#include <stdio.h>
#include <string.h>

void revStr(char *str, int l, int r)
{
    if (l >= r)
        return;

    char temp = str[l];
    str[l] = str[r];
    str[r] = temp;

    revStr(str, l+1,r-1);

}

int main()
{
    char str[6] = "yawar";

    printf("%s\n",str);

    int len = strlen(str);
    revStr(str, 0, len - 1);

    printf("%s\n",str);

    return 0;
}
