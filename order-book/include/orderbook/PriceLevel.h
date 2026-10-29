#pragma once
#include <iostream>
#include "orderbook/OrderNode.h"

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

	void addOrder(OrderNode& order) {
		if (head == nullptr) {
			head = &order;
			tail = head;
		} else if (head == tail) {
			head->setNext(&order);
			order.setPrev(head);
			order.setNext(nullptr);
			tail = &order;
		} else {
			order.setPrev(tail);
			tail->setNext(&order);
			tail = &order;
		}
	}

	void removeNode(OrderNode& node) {
		auto prevNode = node.getPrev();
		auto nextNode = node.getNext();


		if (prevNode && nextNode) {
			prevNode->setNext(nextNode);
			nextNode->setPrev(prevNode);
		}
		else if (tail == &node && head == &node) {
			head = nullptr;
			tail = nullptr;
		}
		else if (tail == &node) {
			prevNode->setNext(nullptr);
			tail = prevNode;
		}
		else if (head == &node) {
			nextNode->setPrev(nullptr);
			head = nextNode;
		}

		node.setNext(nullptr);
		node.setPrev(nullptr);
	}

	Trades attemptFill(OrderNode& order) {
		OrderNode* current = head;
		OrderNode* prev = nullptr;
		Trades trades;

		OrderType orderType = order.getOrderType();

		TradeInfo bidTrade;
		TradeInfo askTrade;

		// Attempt to fill the order with the orders in the price level
		while (current != nullptr) {

			current->fillQuantity(order.getQuantityRemaining());
			order.fillQuantity(current->getQuantity());

			if (orderType == OrderType::BUY) {
				TradeInfo bidTrade{0, order.getPrice(), order.getQuantityFilled()};
				TradeInfo askTrade{0, current->getPrice(), current->getQuantityFilled()};
				trades.push_back(Trade(bidTrade, askTrade));
			}
			else {
				bidTrade = TradeInfo {0, current->getPrice(), current->getQuantityFilled()};
				askTrade = TradeInfo {0, order.getPrice(), order.getQuantityFilled()};
				trades.push_back(Trade(bidTrade, askTrade));
			}

			prev = current;
			current = current->getNext();

			// Remove the node from the price level if it has been completely filled
			if (prev->getQuantityRemaining() == 0) {
				removeNode(*prev);
			}

			if (order.getQuantityRemaining() == 0) {
				return;
			}

		}

		// If order wasn't filled completely, we add it to the order book.
		if (order.getQuantityRemaining() > 0) {
			addOrder(order);
		}

		return trades;
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

