//W.A.P to find second largest element in an array

#include <stdio.h>

int main(){
	int arr[] = {1,6,9,3,8,10};
	int arr_len = sizeof(arr)/sizeof(arr[0]);
	for(int i = 0; i<arr_len; i++){
		if(arr[i] > arr[i+1]){
			int tmp = arr[i];
			arr[i] = arr[i+1]
		}
	}
}
