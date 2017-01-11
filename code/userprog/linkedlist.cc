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
}

//----------------------------------------------------------------------
// Node::~Node
//		De-allocate node, when no longer needed. 
//----------------------------------------------------------------------
Node::~Node ()
{
	delete next;
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
	thdCount = 0;
}

//----------------------------------------------------------------------
// LinkedList::~LinkedList
//		De-allocate list, when no longer needed
//----------------------------------------------------------------------
LinkedList::~LinkedList (int n) 
{
	delete next;
}

//----------------------------------------------------------------------
// LinkedList::addNode
//		Add a node to the linked list
//----------------------------------------------------------------------
void LinkedList::addNode (Node* node) 
{
	switch(thdCount) {
		case (0) :
			next = node;
			thdCount++;
			break;
		case (listSize) :
			fprintf(stderr, "Number of threads created have reached its limit.\n" );
			break;
		default :			
			Node* tmp = next;
			while(tmp->nextNode() != NULL) {
				tmp = tmp->nextNode();
			}
			tmp->setNext(node);
			thdCount++;
	}
}

//----------------------------------------------------------------------
// LinkedList::removeNode
//		Add a node to the linked list
//----------------------------------------------------------------------
void LinkedList::removeNode (Node* node) 
{
	if (next == node) {		
		next = next->nextNode();
		// not sure about the delete
		thdCount--;
	} else {
		Node* tmp;
		tmp = next;
		while((tmp->nextNode() != NULL) && (tmp->nextNode() != node)) {
			tmp = tmp->nextNode();
		}
		if(tmp->nextNode() == NULL) {
			fprintf(stderr, "The node to be removed is not in the list.\n" );
		} else {
			Node* x = tmp->nextNode();
			tmp->setNext(x->nextNode());
			thdCount--;
		}
	} 

}