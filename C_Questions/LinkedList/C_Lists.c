#include <stdio.h>
#include <stdlib.h>

typedef struct CNode{
	int data;
	struct CNode *next;
}CNode;

int main(){
	int len = 0;
	int data = 0;
	printf("sizeof circular linkedlist: %ld\n",sizeof(CNode));
	printf("Enter the length of the list: ");
	scanf("%d",&len);

	//int tmp = len;
	CNode *curr = NULL;
	CNode *prev = NULL;
	CNode *head = NULL;
	
	for(int i = 0; i < len; i++){
		curr = malloc(sizeof(CNode));
		printf("pos %d : ",i+1);
		scanf("%d",&data);		
		curr->data = data;

		if(!i){
			head = curr;
			prev = curr;
		}else{
			prev->next =curr;
			prev = curr;
		}

		if(i == len-1){
			curr->next = head;
		}
	}
	
	curr = head;
	do{
		printf("%d ",curr->data);
		curr = curr->next;
	}while(curr != head);

}
