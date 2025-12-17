#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct student
{
	int roll;
	char name[20];
	float marks;
	struct node*next;
}node;
void add()
{
    node *new = (node *)malloc(sizeof(node));
    if (new == NULL)
    {
        printf("Memory not allocated\n");
        return;
    }

    printf("Enter roll: ");
    scanf("%d", &new->roll);

    printf("Enter name: ");
    scanf("%s", new->name);

    printf("Enter marks: ");
    scanf("%f", &new->marks);

    new->next = NULL;
    if (head == NULL || new->roll < head->roll)
    {
        new->next = head;
        head = new;
        return;
    }
    node *temp = head;
    while (temp->next != NULL && temp->next->roll < new->roll)
    {
        temp = temp->next;
    }

    new->next = temp->next;
    temp->next = new;

    printf("Student inserted in sorted order!\n");
}

