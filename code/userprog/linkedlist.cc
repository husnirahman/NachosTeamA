#include "copyright.h"
#include "system.h"
#include "linkedlist.h"

//----------------------------------------------------------------------
// Node::Node
//      Initialize a node with the value "n".
//----------------------------------------------------------------------
Node::Node (int n)
{
	value = n;
	next = NULL;
	status = ACTIVE;
}

//----------------------------------------------------------------------
// Node::~Node
//		De-allocate node, when no longer needed. 
//----------------------------------------------------------------------
Node::~Node ()
{
	delete next;
}

//----------------------------------------------------------------------
// LinkedList::LinkedList
//		Initialize a linked list with a size of n
//----------------------------------------------------------------------
LinkedList::LinkedList () 
{
	next = NULL;
	thdCount = 0;
}

//----------------------------------------------------------------------
// LinkedList::~LinkedList
//		De-allocate list, when no longer needed
//----------------------------------------------------------------------
LinkedList::~LinkedList () 
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
		case (MAX_THREAD) :
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

//----------------------------------------------------------------------
// LinkedList::searchNode
//		Returns a node with the value "i"
//----------------------------------------------------------------------
Node* LinkedList::searchNode (int i) 
{
	if (next->getID() == i) {		
		return next;
	} else {
		Node* tmp;
		tmp = next->nextNode();
		while((tmp != NULL) && (tmp->getID() != i)) {
			tmp = tmp->nextNode();
		}
		if(tmp == NULL) {
			fprintf(stderr, "The node is not in the list.\n" );
		} 
		return tmp;
	} 
}