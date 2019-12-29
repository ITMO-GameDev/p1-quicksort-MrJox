#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

#include "pch.h"

template <class T>
class LinkedList {
public:
	struct Node {
		T data;
		Node* pNext;
	};

public:
	LinkedList() : pHead(nullptr) {}
	void insert(Node*, Node*);
	void remove(Node*, Node*);

public:
	Node* pHead;
};

template <class T>
void LinkedList<T>::insert(Node* prevNode, Node* newNode) {
	if (prevNode == nullptr) {
		// Is the first node
		if (pHead != nullptr) {
			// The list has more elements
			newNode->pNext = pHead;
		} else {
			newNode->pNext = nullptr;
		}
		pHead = newNode;
	} else {
		if (prevNode->pNext == nullptr) {
			// Is the last node
			prevNode->pNext = newNode;
			newNode->pNext = nullptr;
		} else {
			// Is a middle node
			newNode->pNext = prevNode->pNext;
			prevNode->pNext = newNode;
		}
	}
}

template <class T>
void LinkedList<T>::remove(Node* prevNode, Node* deleteNode) {
	if (prevNode == nullptr) {
		if (deleteNode->pNext == nullptr) {
			pHead = nullptr;
		} else {
			pHead = deleteNode->pNext;
		}
	} else {
		prevNode->pNext = deleteNode->pNext;
	}
}

#endif
