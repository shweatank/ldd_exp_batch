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

	printf("Printing the list : ");
	do{
		printf("%d ",curr->data);
		curr = curr->next;
	}while(curr->next != NULL);
	printf("%d\n",curr->data);
	curr = head;
	int count = 0;
	while(curr != NULL){
		curr = curr->next;
		count++;
	}
	printf("c %d\n",count);
	curr = head->next;
	prev = head;
	Node *store = NULL;
	Node *new_curr = NULL;
	Node *new_prev = NULL;
	new_curr = head->next;
	int fg = 1;
	for(int i = 0; i<len; i++){
		
		if(new_curr == NULL)
			break;
		
		for(int j = 1; j <len; j++){
			if(new_prev == NULL)
				break;
			if(new_prev->data > new_curr->data){
					
				//store = prev->next;
				prev->next = curr->next;
				curr->next = prev;
				
				new_prev = curr;
				fg = 0;
				//int tmp = prev->data;
				//prev->data = curr->data;
				//curr->data = tmp;
			}

			if(fg == 1){
				prev = prev->next;
				//fg = 0
			}else{
				prev = new_prev;
			}
		}

		fg = 1;
		prev = head;
		new_curr = new_curr->next;
		curr = new_curr;
	}

	printf("Printing the reversed list : ");
	curr = head;
	do{
		printf("%d ",curr->data);
		curr = curr->next;
	}while(curr->next != NULL);
	printf("%d\n",curr->data);

}
