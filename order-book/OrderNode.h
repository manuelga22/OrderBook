#pragma once
#include "OrderType.h"

class OrderNode {

private:
	uint64_t orderId;
    OrderType orderType;
	float price;
    int quantity;
    OrderNode* next = nullptr;
    OrderNode* prev = nullptr;
public:
   
    OrderNode(): price(-1.0), quantity(-1), next(nullptr), prev(nullptr) {
        // Code inside the body runs after variables are initialized
    }

	uint64_t getOrderId() const {
		return orderId;
	}

	int getQuantity() const {
		return quantity;
	}

	float getPrice() const {
		return price;
	}

	OrderNode* getNext() const {
		return next;
	}

	OrderNode* getPrev() const {
		return prev;
	}

	void setQuantity(int quantity) {
		this->quantity = quantity;
	}

	void setOrderType(OrderType orderType) {
		this->orderType = orderType;
	}

	void setNext(OrderNode* next) {
		this->next = next;
	}

	float getPrice() {
		return price;
	}

	void setPrice(float price) {
		this->price = price;
	}

	void setPrev(OrderNode* prev) {
		this->prev = prev;
	}

	void printOrderNode() {
		std::cout << "Printing Order Node: " << std::endl;
		std::cout << "Order ID: " << orderId << ", Price: " << price << ", Quantity: " << quantity << std::endl;
	}
};