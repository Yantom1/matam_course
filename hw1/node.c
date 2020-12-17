#include <stdlib.h>

#include "node.h"

// Struct decleration

struct Node_t{
    PQElement pq_element;
    PQElementPriority pq_element_priority;
    Node next;
};

// Function implementations

Node nodeCreate(PQElement element, PQElementPriority priority,
                CopyPQElement copy_element, FreePQElement free_element,
                CopyPQElementPriority copy_priority, FreePQElementPriority free_priority)
{
    if(priority == NULL || element == NULL){
        return NULL;
    }
    
    Node ptr = (Node)malloc(sizeof(*ptr));
    if (ptr == NULL){
        return NULL;
    }
    ptr->next = NULL;

    PQElement element_copy = copy_element(element); 
    if (element_copy == NULL){
        free(ptr);
        return NULL;
    }
    ptr->pq_element = element_copy;
    PQElementPriority priority_copy = copy_priority(priority);
    
    if (priority_copy == NULL){
        free_element(ptr->pq_element);
        free(ptr);
        return NULL;
    }
    ptr->pq_element_priority = priority_copy;
    return ptr;
}

void nodeDestroy(Node node, FreePQElement free_element, FreePQElementPriority free_priority)
{
    if (node == NULL){
        return;
    }
    free_element(node->pq_element);
    free_priority(node->pq_element_priority);
    free(node);
}

PQElement nodeGetElement(Node node){
    if(node == NULL){
        return NULL;
    }

    return node->pq_element;
}

PQElementPriority nodeGetElementPriority(Node node)
{
    if(node == NULL){
        return NULL;
    }

    return node->pq_element_priority;
}

void nodeSetNext(Node node,Node next_node)
{
    if(node == NULL){
        return;
    }

    node->next = next_node;
}

Node nodeGetNext(Node node)
{
    if(node == NULL){
        return NULL;
    }

    return node->next;
}