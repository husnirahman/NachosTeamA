#include "copyright.h"
#include "linkedlist.h"

//----------------------------------------------------------------------
// Node::Node
//      Initialize a node with the value "n".
//----------------------------------------------------------------------
Node::Node (int n)
{
	value = n;
	next = NULL;
	prev = NULL;
}

//----------------------------------------------------------------------
// Node::~Node
//		De-allocate node, when no longer needed. 
//----------------------------------------------------------------------
Node::~Node ()
{
	delete next;
	delete prev;
}

class LinkedList
{
	public:
		LinkedList (int n); // Initialize the linked list with number of integer "size". 
		~LinkedList (); // de-allocate the linked list
		void addNode (Node* node);
		void removeNode (Node* node);
	private:
		Node* next;
		int thdCount; // Number of threads in the list
		int listSize; 
}

//----------------------------------------------------------------------
// LinkedList::LinkedList
//		Initialize a linked list with a size of n
//----------------------------------------------------------------------
LinkedList::LinkedList (int n) 
{
	next = NULL;
	listSize = n;
	thhdCount = 0;
}

//----------------------------------------------------------------------
// LinkedList::~LinkedList
//		De-allocate list, when no longer needed.
//----------------------------------------------------------------------
LinkedList::~LinkedList (int n) 
{
	delete next;
}