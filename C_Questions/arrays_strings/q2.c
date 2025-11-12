// W.A.P to reverse an array
#include <stdio.h>

int main(){
	int arr[] = {1,2,3,4,5,6,7};

	/*
	 * Finding the size of the array
	 * */
	int arr_len = sizeof(arr)/sizeof(arr[1]);

	for(int i = 0; i < arr_len/2; i++){
		int tmp = arr[i];
		//printf("%d\n",arr[arr_len - 1 - i]);
		arr[i] = arr[arr_len - 1 - i];
		arr[arr_len - 1 - i] = tmp;
		
	}
	printf("The reversed array is : ");
	int j = 0;
	while(j != (arr_len)){
		printf("%d ",arr[j]);
		j++;
	}
	printf("\n");
}
