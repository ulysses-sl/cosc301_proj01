#ifndef __LIST_H__
#define __LIST_H__

/* your list data structure declarations */

struct node {
    int num;
    struct node *next;
};


/* your function declarations associated with the list */

void list_clear(struct node *list);

void list_print(const struct node *list);

void list_insert(const int num, struct node **head);

void list_sort(struct node **head);


#endif // __LIST_H__
