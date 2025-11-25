#include <stdio.h>

int main(){
	int i = 0;	
	int num = 0;
	char num_arr[10];

	printf("Enter num: ");
	scanf("%d",&num);
	
	do{
		int tmp = num%16;
		if(tmp > 9){
			num_arr[i++] = (tmp-10) + 'a';
		}else
			num_arr[i++] = tmp + '0';

	
	}while((num /= 16) > 0);
	
	printf("Hex Number\n");	
	int j = i;
	while(i--){
		printf("%c",num_arr[i]);
		//j++;
	}
	printf("\n");
	
	printf("Binary Number\n");
	do{
		//tmp = num_arr[j] - '0'
		printf("%d",num_arr[j]%2);
		j--;
	}while((num_arr[j]/2) != 0);
	printf("\n");
}
