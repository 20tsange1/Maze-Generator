#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>

// Creating a node
typedef struct node
{
    int x;
    int y;
    struct node *next;
} node_t;

void push(node_t *head, int x, int y)
{
    node_t *current = head;
    while (current->next != NULL)
    {
        current = current->next;
    }

    /* now we can add a new variable */
    current->next = (node_t *)malloc(sizeof(node_t));
    current->next->x = x;
    current->next->y = y;
    current->next->next = NULL;
}

void del(node_t *head)
{
    node_t *current = head;
    node_t *after = head->next;

    while (current->next != NULL)
    {
        while (after->next != NULL)
        {
            printf("%i, %i -- %i, %i\n", current->x, current->y, after->x, after->y);
            if ((current->x == after->x) && (current->y == after->y))
            {
                current->next = after->next;
            }
            after = after->next;
        }

        current = current->next;
        after = current->next;
    }
}

// print the linked list value
void print_list(node_t *head)
{
    node_t *current = head;

    while (current != NULL)
    {
        printf("%d - %d\n", current->x, current->y);
        current = current->next;
    }
}

int main()
{
    node_t *head = NULL;
    head = (node_t *)malloc(sizeof(node_t));
    if (head == NULL)
    {
        return 1;
    }

    head->x = 0;
    head->y = 0;
    head->next = NULL;

    push(head, 1, 1);
    push(head, 2, 2);
    push(head, 3, 3);
    push(head, 4, 4);
    push(head, 5, 4);
    push(head, 6, 4);
    push(head, 4, 4);
    push(head, 7, 4);
    push(head, 8, 4);
    push(head, 4, 4);

    del(head);

    // printing node-value
    print_list(head);
}