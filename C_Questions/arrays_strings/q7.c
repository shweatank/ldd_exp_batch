//Reversing the arrays in groups
#include <stdio.h>

int main(){
	
	int arr[] = {1,4,5,2,3,4,3,7,8};
	int k = 4;
	int grp_size = 0;
	int arr_len = sizeof(arr)/sizeof(arr[4]);
	if(arr_len%k == 0){
		grp_size = (arr_len/k);
	}else{	
		grp_size = ((arr_len/k) + 1);
	}
	int div = k/2;
	int next = 0;
	int hold = 0;
	int nk = k;
	int t = 0;
	//printf
	for(int i = 0; i<grp_size; i++){
		t = 0;
		printf("enter\n");
		for(int j=next; j<(div+next-hold); j++){
			printf("%d %d\n",arr[j], arr[nk - 1 -t]);
			int tmp = arr[j];
			arr[j] = arr[(nk) - 1 -t];
			arr[(nk) - 1 - t] = tmp;
			t++;
			
		}
		next+=k;
		nk+=4;
		if(div+next > arr_len){
			hold = div+next - arr_len;
		}
	}

	for(int i = 0; i < arr_len; i++){
		printf("%d ",arr[i]);
	}
}
