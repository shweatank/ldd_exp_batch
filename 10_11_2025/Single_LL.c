#include <stdio.h>
#include <stdlib.h>

struct Node{
	int data;
	struct Node *next;
};

struct Node *createNode(int data) {
	struct Node *newNode = malloc(sizeof(struct Node));
	if(newNode == NULL)	{
		printf("Error in creating Node\n");
		return newNode;
	}
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}

void print_list(struct Node *head) {
	if (head == NULL) {
		printf("Empty List...\n");
		return;
	}
	printf("Head -> ");
	while(head) {
		printf("%d -> ", head->data);
		head = head->next;
	}
	printf("Null\n");
	return;
}

struct Node *insert_first(struct Node *head, int data) {
	struct Node *newNode = createNode(data);
	if(newNode == NULL) return head;

	newNode->next = head;
	head = newNode;
	return head;
}

struct Node *insert_last(struct Node *head, int data) {
	struct Node *newNode = createNode(data);
	if(newNode == NULL) return head;
	
	if (head == NULL) {
		head = newNode;
		return head;
	}
	struct Node *temp = head;
	while(temp->next) temp=temp->next;

	temp->next = newNode;
	return head;
}

struct Node *delete_first(struct Node *head) {
	if (head == NULL) return head;
	struct Node *temp = head;
	head = head->next;
	free(temp);
	return head;
}

struct Node *delete_last(struct Node *head) {
	if (head == NULL) return head;
	struct Node *curr = head;
	struct Node *prev = NULL;
	while(curr->next) {
		prev = curr;
		curr=curr->next;
	}
	free(curr);
	prev->next = NULL;
	return head;
}

struct Node *delete_node(struct Node *head, int data) {
	struct Node *temp = head;
	struct Node *prev = NULL;

	while(temp && temp->data != data) {
		prev = temp;
		temp = temp->next;
	}
	if(temp == NULL) 
		printf("Data not found\n");
	else if(temp == head) {
		head = temp->next;
		free(temp);
	}
	else {
		prev->next = temp->next;
		free(temp);
	}
	return head;
}

void find_mid_node(struct Node *head) {
	if(head == NULL) {
		printf("Empty list...\n");
		return;
	}

	struct Node *slow = head;
	struct Node *fast = head;
	while(fast && fast->next) {
		slow = slow->next;
		fast = fast->next->next;
	}
	printf("Mid node is %d\n", slow->data);
	return;
}

void create_loop(struct Node *head, int position) {
	if(position<0) return;
	struct Node *loop_node = NULL;
	int count = 0;
	struct Node *temp = head;
	while(temp->next) {
		if (count == position) 
			loop_node = temp;
		temp=temp->next;
		count++;
	}
	temp->next = loop_node;
	return;
}

void detect_loop(struct Node *head) {
	if(head == NULL) {
		printf("Empty List..\n");
		return;
	}
	struct Node *slow = head;
	struct Node *fast = head;
	while(fast && fast->next) {
		slow = slow->next;
		fast = fast->next->next;
		if(slow == fast) {
			printf("Loop detected\n");
			return;
		}
	}
	printf("Loop Not detected\n");
	return;
}

void remove_loop(struct Node *head) {
	if(head == NULL) {
		printf("Empty List..\n");
		return;
	}
	struct Node *slow = head;
	struct Node *fast = head;
	while(fast && fast->next) {
		slow = slow->next;
		fast = fast->next->next;
		if(slow == fast) {
			slow = head; 	//reset slow to head and fast is at loop
			if (slow == fast)  //if loop is at head special case
				while(slow != fast->next) fast=fast->next;			//slow is at head(where loop is) move fast till next of fast is slow
			else		//loop is not at head
				while(slow->next != fast->next) {	//slow is at head and fast is at loop. 
					slow = slow->next;				//Move slow and fast one step till next of fast is slow
					fast = fast->next;
				}
			fast->next = NULL;
		}
	}
}

struct Node *reverse_list_it(struct Node *head) {
	if(head == NULL) {
		printf("Empty List..\n");
		return head;
	}
	struct Node *curr = head;
	struct Node *prev = NULL;
	struct Node *next = NULL;
	
	while(curr) {
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}
	head = prev;
	return head;
}

struct Node *reverse_list_rec(struct Node *head) {
	if (head == NULL || head->next == NULL) return head;

	struct Node *revHead = reverse_list_rec(head->next); //recursively call till we get last node(make it head)

	head->next->next = head; //pointing istelf to the next of its next node (reversing link)
	head->next = NULL;		//making it last node

	return revHead;
}

struct Node *Delete_List(struct Node *head) {
	if(head == NULL) {
		printf("Empty List..\n");
		return head;
	}
	while(head) {
		struct Node *temp = head;
		head = head->next;
		free(temp);
	}
	return head;
}
	
int main() {
	struct Node *head = NULL;
	
	head = insert_first(head, 30);
	head = insert_first(head, 20);
	head = insert_first(head, 10);

	print_list(head);

	head = insert_last(head, 40);
	head = insert_last(head, 50);
	print_list(head);

//	head = delete_first(head);
//	print_list(head);

//	head = delete_last(head);
//	print_list(head);

//	head = delete_node(head, 5);
//	print_list(head);
	
	find_mid_node(head);

	detect_loop(head);
	create_loop(head,2);
	detect_loop(head);
	
	remove_loop(head);
	detect_loop(head);
	print_list(head);

	head = reverse_list_rec(head);
	print_list(head);
	
	head = Delete_List(head);
	print_list(head);

	return 0;
}



