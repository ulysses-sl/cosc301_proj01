#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/* your list function definitions */

void list_clear(struct node *list) {
    while (list != NULL) {
        struct node *tmp = list;
        list = list->next;
        free(tmp);
    }
}


void list_print(const struct node *list) {
    printf("*** List Contents Begin ***\n");
    while (list != NULL) {
        printf("%d\n", list->num);
        list = list->next;
    }
    printf("*** List Contents End ***\n");
}


void list_insert(const int num, struct node **head) {
    struct node *newnode = malloc(sizeof(struct node));
    newnode->num = num;
    newnode->next = *head;
    *head = newnode;
}


void list_sort(struct node **head) {
    if (*head == NULL) {
        return;
    }
    struct node *curr = *head;
    struct node *newhead = curr;
    curr = curr->next;
    newhead->next = NULL;
    while (curr != NULL) {
        if (newhead->num > curr->num) {
            struct node *temp = curr->next;
            curr->next = newhead;
            newhead = curr;
            curr = temp;
        }
        else {
            struct node *newpos = newhead;
            while (newpos->next != NULL && (newpos->next->num <= curr->num)) {
                newpos = newpos->next;
            }
            struct node *tail = NULL;
            if (newpos->next != NULL) {
                tail = newpos->next;
            }
            newpos->next = curr;
            curr = curr->next;
            newpos->next->next = tail;
        }
    }
    *head = newhead;
}
