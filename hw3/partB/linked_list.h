#ifndef LIST_H
#define LIST_H

#include <iostream>
#include "../partA/exceptions.h"


using std::cout;
using std::endl;
using mtm::OutOfBoundaries;
#include "node.h"

namespace mtm
{
/*
    Description- Generic Sorted Linked List.
    T represent the type of the object that stored in the list.
    S is an object function user providing that compares the elements.
*/
template <typename T, typename S>
class List
{
    private:
    Node<T,S> *start_node; //stores the pointer of first object in the linked list
	Node<T,S> *end_node; //stores the pointer of the last object in the linked list
	void insertStart(T); 
	void insertEnd(T); 
    void removeStart();
    void removeEnd();
    S compare_function;
    
    public:
    // === Member Function === //
	List(S compare_function);
    List(const List<T,S>& copy_list);
    List<T,S>& operator=(const List<T,S>& copy_list);
	~List();
	void insert(T data); 
    void remove(T data);
    int getSize() const;
    bool isEmpty() const; 
	bool exists(T data) const; 
    T operator[](int index) const;	
};
// === Linked List Implementation === //

/*
    Copy Ctor- receives a list and copying it to this
*/
template <typename T, typename S>
List<T,S>::List(const List<T,S>& copy_list)
{
    compare_function = copy_list.compare_function;
    start_node = NULL;
    end_node = NULL;
    Node<T,S>* copy_node = copy_list.start_node;
    while(copy_node != NULL){
        insert(copy_node->node_data);
        copy_node = copy_node->next;
    }
}

/* 
    Builder Ctor- receives an object function that compares elements priority.
                  function creates list with start_node and end_node as NULL and stroing the provided function
*/
template <typename T, typename S>
List<T,S>::List(S compare):start_node(NULL), end_node(NULL), compare_function(compare)
{
}

/* 
    operator= - receives a list and assigning it to this
*/
template <typename T, typename S>
List<T,S>& List<T,S>::operator=(const List<T,S>& copy_list){
    if(!isEmpty()){
        Node<T,S>* current = start_node;
        Node<T,S>* temp;
        while (current != NULL ) // delete remaining nodes
        {  
            temp = current;
            current = current->next;
            delete temp;
        }
    }

    start_node = NULL;
    end_node = NULL;
    Node<T,S>* copy_node = copy_list.start_node;
    while(copy_node != NULL){
        insert(copy_node->node_data);
        copy_node = copy_node->next;
    }

    return *this;
}

/*  
    Destrcutor - destroying the nodes of the list
*/    
template <typename T, typename S>
List<T,S>::~List()
{
	if (!isEmpty())
   {    
      Node<T,S>* current = start_node;
      Node<T,S>* temp;

      while (current != NULL )
      {  
         temp = current;
         current = current->next;
         delete temp;
      }
      start_node = NULL;
      end_node = NULL;
   }
}

/*
    isEmpty - return true is list empty, false elsewhere
*/
template <typename T, typename S>
bool List<T,S>::isEmpty() const
{
	if(start_node == NULL){ 
        return true;
    } else {
		return false;
    }
}

/* 
    insertStart - inserting the received data iמ the first place of the list
*/
template <typename T, typename S>
void List<T,S>::insertStart(T data)
{
	if(isEmpty()) 
	{
		Node<T,S>* new_node = new Node<T,S>(data); 
		start_node = new_node; 
		end_node = new_node;
	}
    else 
	{
		Node<T,S>* new_node = new Node<T,S>(data);
		new_node->next = start_node; 
		start_node = new_node; 
	}
}
/* 
    insertEnd - inserting the received data in the last place of the list
*/
template <typename T, typename S>
void List<T,S>::insertEnd(T data)
{
	if(isEmpty()) 
	{
		Node<T,S>* new_node = new Node<T,S>(data);
		start_node = new_node;
		end_node = new_node;
	} else 
	{
		Node<T,S>* new_node = new Node<T,S>(data);
		end_node->next = new_node; 
		end_node = new_node; 
	}
}

/* 
    insert - inserting recived data to the list in the proper oreder defined by provided compare function
*/    
template <typename T, typename S>
void List<T,S>::insert(T data) 
{
	if(isEmpty()) 
	{
		insertStart(data);
        return;
	}

	if(compare_function(data,start_node->node_data)) 
	{
		insertStart(data);
        return;
	}
	if(!compare_function(data,end_node->node_data)) 
	{
		insertEnd(data);
        return;
	}
		
	Node<T,S>* current = start_node;
	Node<T,S>* new_node = new Node<T,S>(data); 
	while(current != end_node) 
	{
        // (new data < next data) && (new data >= current data)
		if((compare_function(new_node->node_data,current->next->node_data))
                && (!compare_function(new_node->node_data, current->node_data))) 
		{
			Node<T,S>* next = current->next; 
			current->next = new_node; 
			new_node->next = next; 
			break;
		}
		current = current->next; 	
	}
	
}

/*
    getSize - function returns size of the list
*/
template <typename T, typename S>
int List<T,S>::getSize() const
{
    if(isEmpty()){
        return 0;
    }
    int count = 0;
    Node<T,S>* temp = start_node;
    while(temp != NULL){
        count++;
        temp = temp->next;
    }

    return count;
}

/*
    removeStart - removing the first node in the list 
*/
template <typename T, typename S>
void List<T,S>::removeStart(){
    Node<T,S>* temp = start_node;
    start_node = temp->next;
    delete temp;
}

/*
    removeEnd - removing the last node in the list 
*/
template <typename T, typename S>
void List<T,S>::removeEnd(){
    Node<T,S>* temp = end_node;
    Node<T,S>* node_iterator = start_node;
    while(node_iterator != NULL){
        if(node_iterator->next == end_node){
            break;
        }
        node_iterator = node_iterator->next;
    }

    end_node = node_iterator;
    end_node->next = NULL;
    delete temp;
}

/*
    remove - removing the data in the list that indentical to the given key
*/
template <typename T, typename S>
void List<T,S>::remove(T key)
{
    if(isEmpty() || !exists(key)){
        return;
    }
    if(start_node->node_data == key){
        removeStart();
        return;
    }
    if(end_node->node_data == key){
        removeEnd();
        return;
    }
    
    Node<T,S>* node_ptr, *node_temp;
	node_ptr = start_node;
    node_temp = NULL;
    while(node_ptr->next !=NULL)
    {
        if(node_ptr->node_data == key){
            break;
        }
        node_temp = node_ptr;
        node_ptr = node_ptr->next;
    }

    node_temp->next =node_ptr->next;
    delete node_ptr;
}

/*
    exists- function that return true if the list contains data equal to the key, false elsewhere
*/
template <typename T, typename S>
bool List<T,S>::exists(T key) const 
{
	Node<T,S>* node_ptr;
	node_ptr = start_node;

	while(node_ptr != NULL) 
	{
		if(node_ptr->node_data == key){
            return true;
		} else {
            node_ptr = node_ptr->next;  
        }	
	}
	return false; 
}

/*
    operator[] - returns the object stored in received index
*/
template <typename T, typename S>
T List<T,S>::operator[](int index) const
{
    int size = getSize();
    if(index < 0 || index >= size){
        throw OutOfBoundaries();
    }
    if(index == 0){
        return start_node->node_data;
    }
    if(index == size - 1){
        return end_node->node_data;
    }
    Node<T,S>* temp = start_node;
    for(int i = 0; i < index; i++){
        temp = temp->next;
    }

    return temp->node_data;
}
}
#endif 