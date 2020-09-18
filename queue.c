#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;

    q->head = q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    while (q->head) {
        list_ele_t *tmp = q->head;

        q->head = q->head->next;

        free(tmp->value);
        free(tmp);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false; /* TODO: What should you do if the q is NULL? */

    char *news = malloc(strlen(s) + 1);
    if (!news) {
        free(news);
        return false;
    } else
        strcpy(news, s);

    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        free(newh);
        free(news);
        return false;
    }

    newh->next = q->head; /* insert head */
    newh->value = news;

    q->head = newh;
    q->size++;

    if (q->size == 1)
        q->tail = newh;

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;

    char *news = malloc(strlen(s) + 1);
    if (!news) {
        free(news);
        return false;
    } else
        strcpy(news, s);

    list_ele_t *newt = malloc(sizeof(list_ele_t));
    if (!newt) {
        free(newt);
        free(news); /* */
        return false;
    }

    newt->next = NULL; /* Remember: It should operate in O(1) time */
    newt->value = news;

    if (!q->tail) {
        q->head = newt;
        q->tail = newt;
    } else {
        q->tail->next = newt;
        q->tail = q->tail->next;
    }
    q->size++;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !(q->head))
        return false;

    list_ele_t *tmp = q->head;

    if (sp) {
        strncpy(sp, tmp->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    q->head = q->head->next;
    if (q->size)
        q->size--;

    free(tmp->value);
    free(tmp);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q)
        return;
    list_ele_t *cursor = NULL;
    list_ele_t *node_head = q->head;
    list_ele_t *node_tail = q->tail;

    while (node_head) {
        list_ele_t *next = node_head->next;
        node_head->next = cursor;
        cursor = node_head;

        if (!(cursor->next))
            node_tail = cursor;

        node_head = next;
    }
    q->head = cursor;
    q->tail = node_tail;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
list_ele_t *SortedMerge(list_ele_t *a, list_ele_t *b)
{
    if (a == NULL)
        return (b);
    else if (b == NULL)
        return (a);

    list_ele_t *result = NULL;

    if (strcasecmp(a->value, b->value) < 0) {
        result = a;
        result->next = SortedMerge(a->next, b);
    } else {
        result = b;
        result->next = SortedMerge(a, b->next);
    }
    return (result);
}

void MergeSort(list_ele_t **headRef)
{
    list_ele_t *head = *headRef, *a, *b;

    if ((head == NULL) || (head->next) == NULL)
        return;

    // FrontBackSplit
    list_ele_t *fast = head->next;
    list_ele_t *slow = head;

    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    a = head;
    b = slow->next;
    slow->next = NULL;

    MergeSort(&a);
    MergeSort(&b);

    *headRef = SortedMerge(a, b);
}

void q_sort(queue_t *q)
{
    if (!q)
        return;
    MergeSort(&q->head);
}
