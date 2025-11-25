#include <stdio.h>

int main(){
	int arr[] = {1,9,9,9};
	int carry = 0;
	int len = 4;
	int tmp = arr[3] + 1;
	if(tmp == 10){
		carry = 1;
		arr[3] = 0;

		for(int i = len-2; i >= 0; i--){
			int check = arr[i] + carry;
			if(check > 9){
				arr[i] = 0;
				carry = 1;
			}else{
				arr[i] = check;
				break;
			}
		}
	}else{
		carry = 0;
	}


	for(int j = 0; j < len; j++){
		printf("%d ",arr[j]);
	
	}
}
