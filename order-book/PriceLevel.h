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

	OrderNode* popLastOrder() {
		// when there are no orders in the price level.
		if (head == nullptr) {
			return nullptr;
		}
		auto oldHead = head;

		// when there is only one order in the price level.
		if (head == tail) {
			head = nullptr;
			tail = head;
			return oldHead;
		}
		
		auto newHead = head->getPrev();
		newHead->setNext(nullptr);
		head = newHead;
		
		// deallocate old head
		oldHead->setPrev(nullptr);

		return oldHead;
	}

	void addOrder(OrderNode* order) {
		if (head == nullptr) {
			head = order;
			tail = order;
		}
		else {
			order->setNext(tail);
			tail->setPrev(order);
			tail = order;
		}
	}

	void removeNode(OrderNode* node) {
		auto prevNode = node->getPrev();
		auto nextNode = node->getNext();

		node->setNext(nullptr);
		node->setPrev(nullptr);

		if (prevNode != nullptr && nextNode != nullptr) {
			prevNode->setNext(nextNode);
			nextNode->setPrev(prevNode);
		}
		else if (prevNode != nullptr && nextNode == nullptr) {
			prevNode->setNext(nullptr);
			tail = prevNode;
		}
		else if (prevNode == nullptr && nextNode != nullptr) {
			nextNode->setPrev(nullptr);
			head = nextNode;
		}
		else {
			head = nullptr;
			tail = nullptr;
		}
	}

	void printPriceLevel() {
		OrderNode* current = tail;
		cout << "Price Level: " << std::endl;
		while (current != nullptr) {
			cout << "Order ID: " << current->getOrderId() << ", Price: " << current->getPrice() << ", Quantity: " << current->getQuantity() << std::endl;
			current = current->getNext();
		}
	}

};

