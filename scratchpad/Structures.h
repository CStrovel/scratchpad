#pragma once
#include <limits.h>
#include <iostream>
#include <string>
#include <exception>

using namespace std;

template <typename T> class DLLNode {
private:
	// ========== data members ==========
	DLLNode<T>* previous; // the node one closer to the start than this node
	DLLNode<T>* next; // the node one closer to the end than this node
	T payload; // value contained by node

protected:
	// ========== mutators ==========
	// setPayload() protected as DLLNode is immutable
	void setPayload(const T& payload) { 
		this->payload = payload;
	}

public:
	// ========== constructors ==========
	DLLNode() {
		this->payload = 0;
		this->next = nullptr;
		this->previous = nullptr;
	}

	DLLNode(const T& newPayload) {
		this->setPayload(newPayload);
		this->setPrevious(nullptr);
		this->setNext(nullptr);
	}

	DLLNode(DLLNode<T>* previous, const T& payload) {
		this->setPayload(payload);
		this->setPrevious(previous);
		this->setNext(nullptr);
	}

	DLLNode(const T& payload, DLLNode<T>* next) {
		this->setNext(next);
		this->setPayload(payload);
		this->setPrevious(nullptr);
	}

	DLLNode(DLLNode<T>* previous, const T& payload, DLLNode<T>* next) {
		this->setPrevious(previous);
		this->setNext(next);
		this->setPayload(payload);
	}

	// ========== setters ==========
	void setPrevious(DLLNode<T>* previous) {
		this->previous = previous;
	}

	void setNext(DLLNode<T>* next) {
		this->next = next;
	}

	// ========== getters ==========
	DLLNode<T>* getPrevious() {
		return this->previous;
	}

	DLLNode<T>* getNext() {
		return this->next;
	}
	
	T getPayload() {
		return this->payload;
	}
};

template <typename T, typename L = unsigned int> class List {
private:
	DLLNode<T>* end;
	DLLNode<T>* start;
	L len;
	L maxLen;

protected:
	// ========== accessors ==========
	DLLNode<T>* getStart() {
		return this->start;
	}

	DLLNode<T>* getEnd() {
		return this->end;
	}

	L getMaxLen() {
		return this->maxLen;
	}

	// Lengh accessor is in public section. "unsigned int length()"

	// ========== mutators ==========
	void setStart(DLLNode<T>* start) {
		this->start = start;
	}

	void setEnd(DLLNode<T>* end) {
		this->end = end;
	}

	void setLength(L len) {
		this->len = len;
	}

	void setMaxLen(L maxLen) {
		this->maxLen = maxLen;
	}

public:
	// ========== constructors ==========
	List() {
		this->start = nullptr;
		this->end = nullptr;
		this->len = 0;
		this->maxLen = UINT_MAX;
	}

	List(L maxLen) {
		this->start = nullptr;
		this->end = nullptr;
		this->len = 0;
		this->setMaxLen(maxLen);
	}

	// ========== accessors ==========
	L length() {
		return this->len;
	}

	// ========== utility ==========
	void print() {
		DLLNode<T>* element = this->getStart();
		while (element != nullptr) {
			cout << element->getPayload() << endl;
			element = element->getNext();
		}
	}

	bool isEmpty() {
		return this->length() == 0;
	}

	bool isFull() {
		return this->length() == this->getMaxLen();
	}
};

template <typename T, typename L = unsigned int> class Stack : public List<T,L> {
public:
	// ========== constructors ==========
	Stack() {
		this->setStart(nullptr);
		this->setEnd(nullptr); // end will be unused in this implementation
		this->setLength(0);
		this->setMaxLen(UINT_MAX);
	}

	Stack(L maxLen) {
		this->setStart(nullptr);
		this->setEnd(nullptr); // end will be unused in this implementation 
		this->setLength(0);
		this->setMaxLen(maxLen);
	}

	// ========== utility ==========
	void push(T val) {
		if (this->length() < this->getMaxLen()) {
			// only one rule needed to place ontop of the stack
			this->setStart(new DLLNode<T>(val,this->getStart()));
			this->setLength(this->length() + 1);
		}
		else {
			throw length_error("Stack already contains " + to_string(this->getMaxLen()) + " elements.");
		}
	}

	T pop() {
		T payload;
		DLLNode<T>* element = this->getStart();
		if (element != nullptr) {
			this->setStart(element->getNext());
			payload = element->getPayload();
			delete element;
			this->setLength(this->length() - 1);
		}
		else {
			throw length_error("Stack is empty.");
		}
		return payload;
	}

	T peek() {
		T payload;
		DLLNode<T>* element = this->getStart();
		if (element != nullptr) {
			payload = element->getPayload();
		}
		else {
			throw length_error("Stack is empty.");
		}
		return payload;
	}
};

template <typename T, typename L = unsigned int> class Queue : public List<T,L> {
public:
	// ========== constructors ===========
	Queue() {
		this->setStart(nullptr);
		this->setEnd(nullptr);
		this->setLength(0);
		this->setMaxLen(UINT_MAX);
	}

	Queue(L maxLen) {
		this->setStart(nullptr);
		this->setEnd(nullptr);
		this->setLength(0);
		this->setMaxLen(maxLen);
	}

	// ========== utility ==========
	void push(T val) {
		if (this->length() < this->getMaxLen()) {
			// rules for an empty list
			if (this->getStart() == nullptr) {
				this->setStart(new DLLNode<T>(val));
				this->setEnd(this->getStart());
			}
			else {
				this->setStart(new DLLNode <T>(val, this->getStart()));
				this->getStart()->getNext()->setPrevious(this->getStart());
			}
			this->setLength(this->length() + 1);
		}
		else {
			throw length_error("Queue is full.");
		}
	}

	T peek() {
		if (this->getEnd() == nullptr) {
			throw length_error("Queue is empty.");
		}

		return this->getEnd()->getPayload();
	}

	T pop() {
		if (this->getEnd() != nullptr) {
			// set a new end node
			DLLNode<T>* element = this->getEnd();
			this->setEnd(element->getPrevious());

			// remove former end node from the list
			T val = element->getPayload();
			delete element;

			// adjust lists length
			this->setLength(this->length() - 1);

			return val;
		}
		else {
			throw length_error("Queue is empty.");
		}
	}

	// ========== Destructor ==========
	~Queue() {
		DLLNode<T>* deleteMe = nullptr;
		DLLNode<T>* next = this->getEnd();
		while (next != nullptr) {
			deleteMe = next;
			next = next->getPrevious();
			delete deleteMe;
		}
	}
};

template <typename T, typename L = unsigned int> class DoubleLinkedList : public List<T, L> {
public:
	// ========== constructors ==========
	DoubleLinkedList() {
		this->setStart(nullptr);
		this->setEnd(nullptr);
		this->setLength(0);
		this->setMaxLen(UINT_MAX);
	}

	DoubleLinkedList(L maxLen) {
		this->setStart(nullptr);
		this->setEnd(nullptr);
		this->setLength(0);
		this->setMaxLen(maxLen);
	}

	// ========== utillity ==========
	void addEnd(const T& val) {
		// Ensure list is under set maximum size
		if (this->length() < this->getMaxLen()) {
			if (this->getEnd() == nullptr) {
				// rules for empty list
				this->setStart(new DLLNode<T>(val));
				this->setEnd(this->getStart());
			}
			else {
				// rules for list with 1+ element
				this->getEnd()->setNext(new DLLNode<T>(this->getEnd(), val));
				this->setEnd(this->getEnd()->getNext());

			}
			this->setLength(this->length() + 1);
		}
		else {
			throw length_error("Double Linked List already contains " + to_string(this->getMaxLen()) + " elements.");
		}
	}

	void addStart(const T& val) {
		// Ensure list is under set maximum size
		if (this->length() < this->getMaxLen()) {
			if (this->getEnd() == nullptr) {
				// rules for empty list
				this->setStart(new DLLNode<T>(val));
				this->setEnd(this->getStart());
			}
			else {
				// rules for list with 1+ elements
				this->getStart()->setPrevious(new DLLNode<T>(val));
				this->setStart(this->getStart()->getPrevious());
			}
			this->setLength(this->length() - 1);
		}
		else {
			throw length_error("Double Linked List already contains " + to_string(this->getMaxLen()) + " elements.");
		}
	}

	void pop(const T& val) {

	}
};

