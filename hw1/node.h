#ifndef NODE_H
#define NODE_H

#include "priority_queue.h"

/** Type for defining the Node */
typedef struct Node_t *Node;

/*
* nodeCreate: Allocates a new node with element and priority
*
* gets element, priority, and the functions: copy_element,free_element,copy_priority,free_priority
* return - the Node if successful and NULL for any kind of failure(NULL arguments, memory full...)
*/
Node nodeCreate(PQElement element,
                PQElementPriority priority,
                CopyPQElement copy_element,
                FreePQElement free_element,
                CopyPQElementPriority copy_priority,
                FreePQElementPriority free_priority
                );

/*
* nodeDestroy: Deallocates an existsing node, including it's element and priority
* gets the node and the functions: free_element ,free_priority
*/
void nodeDestroy(Node node,
                 FreePQElement free_element,
                 FreePQElementPriority free_priority
                 );

/*
* nodeGetElement: Getter for the element inside the node
*
* gets the node
* return - NULL if the node is NULL and the element otherwise
*/
PQElement nodeGetElement(Node node);

/*
* nodeGetElementPriority: Getter for the priority inside the node
*
* gets the node
* return - NULL if the node is NULL and the priority otherwise
*/
PQElementPriority nodeGetElementPriority(Node node);

/*
* nodeSetNext: Sets the pointer next of node to next node
*
* gets node to chagne it's next pointer, and next_node to point to
*/
void nodeSetNext(Node node,Node next_node);

/*
* nodeGetNext: Getter for the next node the current node points to
*
* gets the node
* return - NULL if the node is NULL and the next node otherwise
*/
Node nodeGetNext(Node node);

#endif