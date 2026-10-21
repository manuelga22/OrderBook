#pragma once
#include <iostream>
#include "OrderNode.h"
#include "OrderId.h"

using std::cout;

class PriceLevel
{
private:
	OrderNode* head = nullptr;
	OrderNode* tail = nullptr;
public:

	PriceLevel() = default;

	OrderNode* popLastOrder() {
		// when there are no orders in the price level.
		if (head == nullptr) {
			return nullptr;
		}
		OrderNode* oldHead = head;

		// when there is only one order in the price level.
		if (head == tail) {
			head = nullptr;
			tail = nullptr;
			return oldHead;
		}
		
		OrderNode* newHead = head->getNext();
		newHead->setPrev(nullptr);
		head = newHead;
		
		// deallocate old head
		oldHead->setPrev(nullptr);
		oldHead->setNext(nullptr);

		return oldHead;
	}

	void addOrder(OrderNode* order) {
		if (head == nullptr) {
			head = order;
			tail = head;
		} else if (head == tail) {
			head->setNext(order);
			order->setPrev(head);
			order->setNext(nullptr);
			tail = order;
		} else {
			order->setPrev(tail);
			tail->setNext(order);
			tail = order;
		}
	}

	void removeNode(OrderNode* node) {
		auto prevNode = node->getPrev();
		auto nextNode = node->getNext();


		if (prevNode && nextNode) {
			prevNode->setNext(nextNode);
			nextNode->setPrev(prevNode);
		}
		else if (tail == node && head == node) {
			head = nullptr;
			tail = nullptr;
		}
		else if (tail == node) {
			prevNode->setNext(nullptr);
			tail = prevNode;
		}
		else if (head == node) {
			nextNode->setPrev(nullptr);
			head = nextNode;
		}

		node->setNext(nullptr);
		node->setPrev(nullptr);
	}

	void printPriceLevel() {
		OrderNode* current = head;
		cout << "Price Level: " << std::endl;
		while (current != nullptr) {
			cout <<  " Price: " << current->getPrice() << ", Quantity: " << current->getQuantity() << std::endl;
			current = current->getNext();
		}
	}

	~PriceLevel() {
		OrderNode* current = head;
		while (current != nullptr) {
			OrderNode* next = current->getNext();
			delete current;
			current = next;
		}
	}

};

