#ifndef LIST_NODE_H
#define LIST_NODE_H

namespace mtm
{
template <typename T,typename S> 
class List;

/*
	Class Node - stores the date of the elements in the generic linked list
*/
template <typename T,typename S>
class Node 
{
	private:
    T node_data; 
	Node* next; //pointer to the next node in list
    friend class List<T,S>;

    public:
	// === Member Functions === //
	Node(T node_data);
	T getData();
};
// === Class Node Implementation ===//

/*
	Builder Ctor - creating new node storing the received data and pointing to NULL
*/
template <typename T,typename S>
Node<T,S>::Node(T data)
{
	node_data = data; 
	next = NULL; 
}

/*
	getData - return the data  stored in the node
*/
template <typename T,typename S>
T Node<T,S>::getData()
{
	return node_data;
}
}
#endif