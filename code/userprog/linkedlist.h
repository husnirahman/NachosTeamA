#ifdef CHANGED
#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "copyright.h"
#include "system.h"

#define ACTIVE true
#define DEAD false

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
		int getID () {
			return value;
		}
		bool getStatus () {
			return status;
		}
	private:
		Node* next;
		int value;
		bool status;

};

class LinkedList
{
	public:
		LinkedList (); // Initialize a linked list with a size of n
		~LinkedList (); // de-allocate the linked list		
		Node* nextNode () {
			return next;
		}
		void setNext (Node* node) {
			next = node;
		}
		void addNode (Node* node);
		void removeNode (Node* node);
		Node* searchNode (int i);
	private:
		Node* next;
		int thdCount; // Number of threads in the list
};

#endif // LINKEDLIST_H
#endif //CHANGED