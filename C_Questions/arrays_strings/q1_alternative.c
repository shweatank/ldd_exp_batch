//W.A.P to rotate an array by q numbers (Towards right)
#include <stdio.h>

int main(){
	int q = 0;
	int arr[] = {1,2,3,4};
	printf("Enter number by which you which to rotate an array: ");
	scanf("%d",&q);

	int len = sizeof(arr)/sizeof(arr[0]);

	int dup_arr[len];

	for(int i = 0; i < len; i++){
		int tmp = i+q;
		if(tmp > (len-1))
			dup_arr[tmp - len] = arr[i];
		else
			dup_arr[tmp] = arr[i];
	}
	printf("The right shifted array is: ");
	for(int i = 0; i < len; i++){
		printf("%d ",dup_arr[i]);
	}
	printf("\n");
}
