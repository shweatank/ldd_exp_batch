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
	struct Node *prev;
}Node;

void initlist(void **,int);
void reverse(void **);
void sortt(void **);
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

	curr = head;
	int d = 3;


	//Removing data from list
	while(curr){
		
		if(curr->data == d){
			if(curr->prev == NULL){
				head = curr->next;
				free(curr);
				break;
			}
			curr->prev->next = curr->next;
			free(curr);
			break;
		}else{
			curr = curr->next;
		}
	}
	printf("\nremoving data \n");
	curr = head;
	
	//printing the list
	while(curr != NULL){
		printf("%d ",curr->data);
		curr = curr->next;
	}
	reverse((void **)&head);
	
	printf("\nreversing the list\n");
	curr = head;
	
	//printing the list
	while(curr != NULL){
		printf("%d ",curr->data);
		curr = curr->prev;
	}


	return 0;
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
			curr->prev = NULL;
		}else{
			curr->prev = prev;
			prev->next = curr;
			prev=curr;
		}
	}
	*head = (void *)h;

	curr->next = NULL;

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
			//i = 0;
		}
		curr->prev = prev;
		curr->next = next;
		prev = curr;
		curr = next;
	}
}

