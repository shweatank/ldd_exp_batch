#include <stdio.h>

int main()
{
    int num;

    printf("Enter num: ");
    scanf("%d", &num);

    int res = 10 * ((num + 10) / 10);
    printf("%d\n",res);
    
    return 0;
}
