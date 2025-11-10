#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student
{
    int roll;
    char name[50];
    float marks;
    struct student *next;
};

typedef struct student node;

node *head = NULL;

void add();
void display();

int main()
{
    while (1)
    {
        int n;
        printf("\n--- MENU ---\n");
        printf("1. Add student\n");
        printf("2. Display students\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &n);

        switch (n)
        {
        case 1:
            add();
            break;
        case 2:
            display();
            break;
        case 3:
            exit(0);
        default:
            printf("Invalid choice\n");
        }
    }
}

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

    if (head == NULL)
    {
        head = new;
    }
    else
    {
        node *temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = new;
    }

    printf("Student added successfully!\n");
}

void display()
{
    if (head == NULL)
    {
        printf("List is empty\n");
        return;
    }

    node *temp = head;
    printf("\n STUDENT LIST \n");

    while (temp != NULL)
    {
        printf("Roll: %d\n", temp->roll);
        printf("Name: %s\n", temp->name);
        printf("Marks: %.2f\n\n", temp->marks);
        temp = temp->next;
    }
}

