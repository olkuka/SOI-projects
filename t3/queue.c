/* © 2019 Aleksandra Kukawka */

#include <stdlib.h>
#include <stdio.h>

typedef struct node{
    char* val;
    struct node *next;
} node;

node* init(char* msg){
    node *head = NULL;
    head = malloc(sizeof(node));

    head->val = msg;
    head->next = NULL;
    return head;
}

void print_queue(node *head) {
    node *current = head;

    while (current != NULL) {
        printf("message: %s\n", current->val);
        current = current->next;
    }
}

void push_end(node *head, char* val) {
    node *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = malloc(sizeof(node));
    current->next->val = val;
    current->next->next = NULL;
}

void push_begin(node **head, char* val) {
    node *new_node;
    new_node = malloc(sizeof(node));

    new_node->val = val;
    new_node->next = *head;
    *head = new_node;
}

void remove_queue(node **head) {
    node *current = NULL;
    while (*head != NULL) {
        current = (*head)->next;
        free(*head);
        *head = current;
    }
}
