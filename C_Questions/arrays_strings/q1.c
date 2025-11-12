//W.A.P to rotate an array by q numbers
#include <stdio.h>

int main(){
	itn q = 0;
	int arr[] = {1,5,6,9,8,2};
	printf("Enter number by which you which to rotate an array: ");
	scanf("%d",&q);

	int len = sizeof(arr)/sizeof(arr[0]);

	int tmp = arr[0];
	for(int j = 0; j<k; j++){
		for(int i = 0; i<len; i++){
			if(i == 0){
				tmp = arr[i];
				arr[i] = arr[len - 1];

			}else{
				int tm = arr[i];
				if(i >1){
					tmp = tm;
				}
				arr[i] = tmp;

			}
		}
	}

}
