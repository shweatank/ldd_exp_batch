#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
	int data;
	struct Node *next;
}Node;

int main(){
	int len = 0;
	int num = 0;
	Node *head = NULL;
	Node *curr = NULL;
	Node *prev = NULL;

	printf("Enter the length list: ");
	scanf("%d", &len);
	//printf("Enter data:");
	for(int i = 0; i<len; i++){
		curr = malloc(sizeof(Node));
		printf("enter data: ");
		scanf("%d",&num);
		curr->data = num;

		if(i == 0){
			head = curr;
			prev = curr;
		}else{
			prev->next = curr;
			prev = curr;
		}
	}
	
	curr->next = NULL;
	curr = head;
	do{
		printf("%d ",curr->data);
		curr = curr->next;
	}while(curr->next != NULL);
	printf("%d\n",curr->data);

	curr = head->next;
	prev = head;

	for(int i = 0; i<len; i++){
		if(curr == NULL)
			break;
		for(int j = 1; j <len; j++){
			if(prev == NULL)
				break;
			if(prev->data > curr->data){
				int tmp = prev->data;
				prev->data = curr->data;
				curr->data = tmp;
			}
			prev = prev->next;
		}
		prev = curr;
		curr = curr->next;
		
	}

	curr = head;
	
	do{
		printf("%d ",curr->data);
		curr = curr->next;
	}while(curr->next != NULL);
	printf("%d ",curr->data);

}
