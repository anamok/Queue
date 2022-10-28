/* 
 * Developed by R. E. Bryant, 2017
 * Extended to store strings, 2018
 * Altered by Anastasia Mokhon
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

queue_t *q_new()
{
    // Allocate space for the queue and return false if malloc fails
    queue_t *q =  malloc(sizeof(queue_t));
    if (q == NULL) return NULL;

    // Set head and tail pointers to NULL and size to 0
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    // Base case: queue in NULL
    if (q == NULL) return;
    list_ele_t *pointer = q->head;

    // Free the space allocated for the queue, then iterate from node
    // to node and free the space allocated for the current node and its value
    free(q);
    while (pointer != NULL) {
        list_ele_t *prev = pointer;
        pointer = pointer->next;
        free(prev->value);
        free(prev);
    }
}

bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    // Base case: queue in NULL
    if (q == NULL) return false;

    // Allocate space for the node and the string, then copy the string into it
    // If either malloc fails, return false
    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL) return false;
    newh->value = (char*) malloc(sizeof(char) * (strlen(s)+1));
    if (newh->value == NULL) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, strlen(s)+1);

    // In O(1) time, use head pointer to insert the new node
    newh->next = q->head;
    if (q->head == NULL) q->tail = newh;
    q->head = newh;

    // Increment size
    q->size++;
    return true;
}

bool q_insert_tail(queue_t *q, char *s)
{
    // Base case: queue is NULL
    if (q == NULL) return false;
    list_ele_t *newt;

    // Allocate space for the node and the string, then copy the string into it
    // If either malloc fails, return false
    newt = malloc(sizeof(list_ele_t));
    if (newt == NULL) return false;
    newt->value = (char*) malloc(sizeof(char) * (strlen(s)+1));
    if (newt->value == NULL){
        free(newt);
        return false;
    }
    strncpy(newt->value, s, strlen(s)+1);

    // In O(1) time, use tail pointer to insert the new node
    newt->next = NULL;
    if (q->tail != NULL) {
        q->tail->next = newt;
        q->tail = newt;
    }
    if (q->head == NULL) q->head = newt;

    // Increment size
    q->size++;
    return true;
}

bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* 
        Attempt to remove head, record its stored value, and return true. Return
        false if queue is null or empty
    */

    // Base case: queue is null or empty
    if (q == NULL || q->head == NULL) return false;
    list_ele_t *oldHead = q->head;

    // If size is 0, set head and tail to null, otherwise remove head
    if (q->size == 0) {
        q->head = NULL;
        q->tail = NULL;
    }
    else {
        // If sp is non-NULL, copy the removed string to *sp
        // (up to a maximum of bufsize-1 characters, plus a null terminator)
        if (!(sp == NULL && bufsize == 0)) {
            memset(sp, '\0', bufsize);
            strncpy(sp, q->head->value, bufsize-1);
        }
        q->head = q->head->next;
    }

    // Free the space used by the list element and the string
    free(oldHead->value);
    free(oldHead);

    // Decrement size
    q->size--;
    return true;
}

int q_size(queue_t *q)
{
    /* 
        Using size variable declared in the queue struct to return the
        number of element in the queue in O(1) time
    */

    if (q == NULL) return 0;
    return q->size;
}

void q_reverse(queue_t *q)
{
    /*
        Starting from the first element of the queue reverse the next
        pointers by making them point to previous node instead of next
    */

    // Base case: queue is NULL or empty
    if (q == NULL || q->head == NULL) return;
    list_ele_t *pointer = q->head;
    list_ele_t *prev = NULL;
    list_ele_t *next = pointer->next;
    while (pointer->next != NULL) {
        pointer->next = prev;
        prev = pointer;
        pointer = next;
        next = pointer->next;
    }
    pointer->next = prev;
    q->tail = q->head;
    q->head = pointer;
}