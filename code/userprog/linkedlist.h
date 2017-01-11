#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "copyright.h"

class Node 
{
	public:		
		Node (int value); // Initialize the linked list with number of integer "size". 
		~Node ();
		Node* nextNode () {
			return next;
		}
		void setNext (Node* node) {
			next = node;
		}
	private:
		Node* next;
		int value;

}

class LinkedList
{
	public:
		LinkedList (int n); // Initialize a linked list with a size of n
		~LinkedList (); // de-allocate the linked list		
		Node* nextNode () {
			return next;
		}
		void setNext (Node* node) {
			next = node;
		}
		void addNode (Node* node);
		void removeNode (Node* node);
		bool searchThread (int i);
	private:
		Node* next;
		int thdCount; // Number of threads in the list
		int listSize;  // Maximum number of threads
}

#endif // LINKEDLIST_H