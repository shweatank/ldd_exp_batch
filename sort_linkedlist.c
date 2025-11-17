#include <stdio.h>
#include <stdlib.h>

struct node
{
    int data;
    struct node *next;
};



int main()
{
    struct node *head = (struct node *)malloc(sizeof(struct node));
    struct node *second = (struct node *)malloc(sizeof(struct node));
    struct node *third = (struct node *)malloc(sizeof(struct node));
    struct node *fourth = (struct node *)malloc(sizeof(struct node));

    head->data = 40;
    head->next = second;

    second->data = 20;
    second->next = third;

    third->data = 10;
    third->next = fourth;

    fourth->data = 30;
    fourth->next = NULL;

    return 0;
}
