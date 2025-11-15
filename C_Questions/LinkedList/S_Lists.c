//Singly LinkedList
//1. Init list
//2. print list
//3. reverse list
//4. removeduplicates
//5. insertatNpos

#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
	int data;
	struct Node *next;
}Node;

void initlist(void **,int);
void reverse(void **);
void sortt(void **,int);
void insertnode(Node **head, int pos, int data);
void removesingledup(Node **head, int data);

int main(){
	int list_len = 0;
	printf("Enter the length of the list: ");
	scanf("%d",&list_len);

	Node *head = NULL;
	int num = 0;
	
	initlist((void **)&head, list_len);
	Node *curr = head;
	//printing the list
	while(curr != NULL){
		printf("%d ",curr->data);
		curr = curr->next;
	}

	//adding the new node at head
	

	//reversing the node
	reverse((void **)&head);	
	printf("\n");
	curr = head;
	//printing the list
	while(curr != NULL){
		printf("%d ",curr->data);
		curr = curr->next;
	}

	//insertnode(&head,1,90);

	//printf("head data : %d\n",head->data);
	printf("\n");
	curr = head;
	//printing the list
	while(curr != NULL){
		printf("%d ",curr->data);
		curr = curr->next;
	}

	//removesingledup(&head, 3);
	printf("\n");
	curr = head;
	//printing the list
	while(curr != NULL){
		printf("%d ",curr->data);
		curr = curr->next;
	}
	//list_len += 1;	
	sortt((void **)&head,list_len);


	printf("\n");
	curr = head;
	//printing the list
	while(curr != NULL){
		printf("%d ",curr->data);
		curr = curr->next;
	}
}

void initlist(void **head, int len){
	
	Node *prev = NULL;
	Node *curr = NULL;
	Node *h = (Node*)*head;
	int num = 0;	
	//init the linked list
	for(int i = 0; i < len; i++){
		curr = malloc(sizeof(Node));
		printf("Enter at %d :",i+1);
		scanf("%d",&num);

		curr->data = num;
		if(i == 0){
			h = curr;
			prev = curr;
		}else{
			prev->next = curr;
			prev=curr;
		}
	}
	*head = (void *)h;

	curr->next = NULL;
	//curr = (Node **)head;

}

void reverse(void **head){
	
	Node *prev = NULL;
	Node *next = NULL;
	Node *curr = (Node *)*head;
	int i = 1;
	while(curr != NULL){
		next = curr->next;

		if(curr->next == NULL){
			*head = (void *)curr;
			i = 0;
		}
		curr->next = prev;
		prev = curr;
		curr = next;
	}
}

void sortt(void **head, int len){
	
	Node *prev = NULL;
	Node *curr = (Node*)*head;
	Node *tmp = (Node*)*head;
	Node *next = curr->next;
	//printf("\nsort\n");

	for(int i = 0; i < len; i++){
		for(int j = 0; j < len - 1 - i; j++){
			
			//printf("%d %d\n",curr->data,next->data);
			
			if(curr->data > next->data){
				
				//Swapping logic
				curr->next = next->next;
				next->next = curr;

				if(prev == NULL){
					//preserving the head pointer
					tmp = next;
					*head = (void *)next;
					
					//updating the prev
					prev = next;
				}else{
					prev->next = next;
					prev = next;
				}

				next = curr->next;
			}else{
				prev = curr;
				curr = next;
				next = next->next;

				//printf("No swapping : prev %d curr %d next %d\n",prev->data,curr->data,next->data);
				if(!next || !curr)
					break;
			}
		}
		
		prev = NULL;
		curr = tmp;
		next = tmp->next;
	}
		
}

void insertnode(Node** head, int pos, int data){
	Node *newNode = NULL;
	Node *curr = *head;
	int k = 1;

	newNode = malloc(sizeof(Node));
	//if(pos == 1){
		newNode->next = curr;
		newNode->data = data;
	//}
	*head = newNode;
	//printf("head data : %d\n",head->data);
	return;
	printf("returned\n");
	while(k != pos-1){
		curr = curr->next;
		k++;
	}

	newNode = malloc(sizeof(Node));
	Node *tmp = curr->next;
	curr->next = newNode;
	newNode->next = tmp;
	newNode->data = data;

}

void removesingledup(Node** head, int data){
	Node *curr = *head;
	Node *next = curr->next;

	while(curr->next != NULL){
		while(next->next != NULL){
			if(curr->data == next->data){
				Node *tmp = next->next;
				curr->next = tmp;
				//free(next);
				break;
			}else{
				next = next->next;
			}
		}
		curr = curr->next;
		next = curr->next;
	}
}
