//write a program to sort an array using bubble sort
#include <stdio.h>

int main(){
	
	int arr[] = {1,50,90,45,99,101,135,124,111};
	int arr_len = sizeof(arr)/sizeof(arr[0]);
	
	for(int i = 0; i < arr_len; i++){
		for(int j = 0; j < arr_len-1-i; j++){
			if(arr[j] > arr[j+1]){
				int tmp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = tmp;
			}
		}
	}

	printf("second largest element : %d\n",arr[arr_len -2]);

}
