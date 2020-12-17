#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "priority_queue.h"
#include "node.h"

#define SIZE_NULL_CONST -1

struct PriorityQueue_t
{
    CopyPQElement copy_element;
    FreePQElement free_element;
    EqualPQElements equal_elements;
    CopyPQElementPriority copy_priority;
    FreePQElementPriority free_priority;
    ComparePQElementPriorities compare_priority;
    Node list;
    Node iterator;
};

// Function Declarations

/*
   pqCheckElementExists - Check if element with certain priority exists in pq
                          return - Element node if exists and NULL otherwise
*/
static Node pqCheckElementExists(PriorityQueue queue, PQElement element, PQElementPriority priority);

/*
   pqRemoveElementPriority - Remove an element with certain priority from pq
                             return - PQ_NULL_ARGUMENT if one of the parameters is NULL
                                      PQ_ELEMENT_DOES_NOT_EXISTS if element not in pq
                                      PQ_SUCCESS if successful
*/
static PriorityQueueResult pqRemoveElementPriority(PriorityQueue queue, PQElement element, PQElementPriority priority);


// Function implementations

PriorityQueue pqCreate(CopyPQElement copy_element, FreePQElement free_element,
                       EqualPQElements equal_elements, CopyPQElementPriority copy_priority,
                       FreePQElementPriority free_priority, ComparePQElementPriorities compare_priority)
{
    if (!copy_element || !free_element || !equal_elements || !copy_priority || !free_priority || !compare_priority)
    {
        return NULL;
    }
    PriorityQueue priority_queue = (PriorityQueue) malloc(sizeof(*priority_queue));
    if (priority_queue == NULL){
        return NULL;
    }

    priority_queue->copy_priority = copy_priority;
    priority_queue->free_priority = free_priority;
    priority_queue->compare_priority = compare_priority;
    priority_queue->copy_element = copy_element;
    priority_queue->free_element = free_element;
    priority_queue->equal_elements = equal_elements;
    priority_queue->list = NULL;
    priority_queue->iterator = NULL;

    return priority_queue;
}

void pqDestroy(PriorityQueue queue)
{
    if (queue == NULL)
    {
        return;
    }
    pqClear(queue);
    free(queue);
}

PriorityQueue pqCopy(PriorityQueue queue)
{
    if (queue == NULL)
    {
        return NULL;
    }
    PriorityQueue new_queue = pqCreate(queue->copy_element, queue->free_element, queue->equal_elements, 
        	                            queue->copy_priority, queue->free_priority, queue->compare_priority);

    if (new_queue == NULL)
    {
        return NULL;
    }
    pqGetFirst(queue);
    pqGetFirst(new_queue);

    while (queue->iterator)
    {
        pqInsert(new_queue, nodeGetElement(queue->iterator), nodeGetElementPriority(queue->iterator));
        pqGetNext(queue);
    }

    queue->iterator = NULL;
    new_queue->iterator = NULL;
    return new_queue;
}

int pqGetSize(PriorityQueue queue)
{
    if (queue == NULL)
    {
        return SIZE_NULL_CONST;
    }

    Node temp_iterator = queue->iterator;
    int size = 0;
    if (pqGetFirst(queue) == NULL){
        queue->iterator = temp_iterator;
        return size;
    }

    while(queue->iterator != NULL)
    {
        size++;   
        pqGetNext(queue);
    }

    queue->iterator = temp_iterator;
    return size;
}

bool pqContains(PriorityQueue queue, PQElement element)
{
    if (queue == NULL || element == NULL)
    {
        return false;
    }
    Node temp_iterator = queue->iterator;
    pqGetFirst(queue);
    while (queue->iterator)
    {
        if (queue->equal_elements(element, nodeGetElement(queue->iterator)))
        {
            queue->iterator = temp_iterator;
            return true;
        }
        pqGetNext(queue);
    }
    queue->iterator = temp_iterator;
    return false;
}

PriorityQueueResult pqInsert(PriorityQueue queue, PQElement element, PQElementPriority priority)
{
    queue->iterator = NULL;
    if (queue == NULL || element == NULL || priority == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }
    bool node_added = false;
    Node temp_node = NULL, new_node = nodeCreate(element, priority, queue->copy_element, queue->free_element,
                                                                 queue->copy_priority, queue->free_priority);
    if (new_node == NULL){
        return PQ_OUT_OF_MEMORY;
    }
    // Handles the case of pq is empty
    if (queue->list == NULL) {
        queue->list = new_node;
        return PQ_SUCCESS;
    }
    pqGetFirst(queue);
    while (queue->iterator){
        if (queue->compare_priority(priority, nodeGetElementPriority(queue->iterator)) > 0){
            // Handles the case when needed to insert to first place
            if (temp_node == NULL){
                queue->list = new_node;
                nodeSetNext(new_node, queue->iterator);
            }
            else{
                nodeSetNext(temp_node, new_node);
                nodeSetNext(new_node, queue->iterator);
            }
            node_added = true;
            break;
        }
        temp_node = queue->iterator;
        pqGetNext(queue);
    }
    if (!node_added){
        nodeSetNext(temp_node, new_node);
    }
    queue->iterator = NULL;
    return PQ_SUCCESS;
}

PriorityQueueResult pqChangePriority(PriorityQueue queue, PQElement element,
                                     PQElementPriority old_priority, PQElementPriority new_priority)
{
    if (queue == NULL || element == NULL || old_priority == NULL || new_priority == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }
    Node temp_element = pqCheckElementExists(queue, element, old_priority);
    if (temp_element == NULL)
    {
        return PQ_ELEMENT_DOES_NOT_EXISTS;
    }
    
    if (pqInsert(queue, element, new_priority) == PQ_OUT_OF_MEMORY)
    {
        return PQ_OUT_OF_MEMORY;
    }

    pqRemoveElementPriority(queue, nodeGetElement(temp_element), nodeGetElementPriority(temp_element));
    queue->iterator = NULL;
    return PQ_SUCCESS;
}

PriorityQueueResult pqRemove(PriorityQueue queue)
{
    if (queue == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }
    pqGetFirst(queue);
    Node temp_node = queue->iterator;
    pqGetNext(queue);
    queue->list = queue->iterator;
    nodeDestroy(temp_node, queue->free_element, queue->free_priority);

    queue->iterator = NULL;
    return PQ_SUCCESS;
}

PriorityQueueResult pqRemoveElement(PriorityQueue queue, PQElement element)
{
    if (queue == NULL || element == NULL){
        return PQ_NULL_ARGUMENT;
    }
    bool found = false;
    Node temp_node = NULL, node_to_remove = NULL;
    pqGetFirst(queue);

    while (queue->iterator){
        if (queue->equal_elements(nodeGetElement(queue->iterator), element)){
            found = true;
            if (temp_node == NULL){
                node_to_remove = queue->iterator;
                pqGetNext(queue);
                queue->list = queue->iterator;
                nodeDestroy(node_to_remove, queue->free_element, queue->free_priority);
                break;
            }
            else{
                node_to_remove = queue->iterator;
                pqGetNext(queue);
                nodeDestroy(node_to_remove ,queue->free_element, queue->free_priority);
                nodeSetNext(temp_node, queue->iterator);
                break;
            }
        }
        temp_node = queue->iterator;
        pqGetNext(queue);
    }

    if (!found){
        return PQ_ELEMENT_DOES_NOT_EXISTS;
    }
    queue->iterator = NULL;
    return PQ_SUCCESS;
}

PQElement pqGetFirst(PriorityQueue queue)
{
    if (queue == NULL)
    {
        return NULL;
    }

    if (queue->list == NULL)
    {
        return NULL;
    }
    

    queue->iterator = queue->list;
    return nodeGetElement(queue->list);
}

PQElement pqGetNext(PriorityQueue queue)
{
    if (queue == NULL || queue->iterator == NULL)
    {
        return NULL;
    }
    queue->iterator = nodeGetNext(queue->iterator);
    if(queue->iterator == NULL){
        return NULL;
    }

    return nodeGetElement(queue->iterator);
}

PriorityQueueResult pqClear(PriorityQueue queue)
{
    if (queue == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }

    pqGetFirst(queue);
    while(queue->iterator != NULL){
        pqRemove(queue);
        pqGetFirst(queue);
    }

    return PQ_SUCCESS;
}

static Node pqCheckElementExists(PriorityQueue queue, PQElement element, PQElementPriority priority)
{
    if(queue == NULL || element == NULL || priority == NULL){
        return NULL;
    }

    pqGetFirst(queue);
    while (queue->iterator)
    {
        if(queue->equal_elements(element, nodeGetElement(queue->iterator)) && 
                queue->compare_priority(priority, nodeGetElementPriority(queue->iterator)) == 0)
        {
            return queue->iterator;
        }
        pqGetNext(queue);
    }
    return NULL;
    
}

static PriorityQueueResult pqRemoveElementPriority(PriorityQueue queue, PQElement element, PQElementPriority priority)
{
    if (queue == NULL || element == NULL){
        return PQ_NULL_ARGUMENT;
    }
    bool found = false;
    Node temp_node = NULL, node_to_remove = NULL, current_node;
    pqGetFirst(queue);

    while (queue->iterator){
        current_node = queue->iterator;
        if (queue->equal_elements(nodeGetElement(current_node), element) 
                && queue->compare_priority(nodeGetElementPriority(current_node), priority) == 0)
            {
            found = true;
            if (temp_node == NULL){
                // When the element to remove is the first element in the list
                node_to_remove = queue->iterator;
                pqGetNext(queue);
                queue->list = queue->iterator;
                nodeDestroy(node_to_remove, queue->free_element, queue->free_priority);
                break;
            }
            else{
                node_to_remove = queue->iterator;
                pqGetNext(queue);
                nodeSetNext(temp_node, queue->iterator);
                nodeDestroy(node_to_remove, queue->free_element, queue->free_priority);
                break;
            }
        }
        temp_node = queue->iterator;
        pqGetNext(queue);
    }

    if (!found){
        return PQ_ELEMENT_DOES_NOT_EXISTS;
    }
    return PQ_SUCCESS;
}