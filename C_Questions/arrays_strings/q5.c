//Reverse an array
#include <stdio.h>
#include <string.h>

int main(){
	char arr[10];

	printf("enter the string:");
	scanf("%s",arr);

	printf("entered string is : %s\n",arr);

	int a = strlen(arr);
	int b = sizeof(arr);
	printf("len is %d %d\n",a,b);
	b = a;
	for(int i = 0; i < a/2; i++){
		char tmp = arr[i];
		arr[i] = arr[b - 1 - i];
		arr[b - 1 - i] = tmp;      
	}
	printf("entered string is : %s\n",arr);
}
