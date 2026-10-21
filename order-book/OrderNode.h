#pragma once
#include "OrderType.h"

class OrderNode {

private:

    OrderNode* next = nullptr;
    OrderNode* prev = nullptr;

	// price is scaled using PRICE_SCALE_FACTOR to avoid floating point precision issues
	int64_t price = 0;
	OrderType orderType = OrderType::UNDEFINED;
	uint32_t quantity = 0;

public:
   
	OrderNode() = default;

	int64_t getPrice() const {
		return price;
	}

	OrderType getOrderType() const {
		return orderType;
	}

	uint32_t getQuantity() const {
		return quantity;
	}

	OrderNode* getNext() const {
		return next;
	}

	OrderNode* getPrev() const {
		return prev;
	}

	void setPrice(float price) {
		this->price = price;
	}

	void setOrderType(OrderType orderType) {
		this->orderType = orderType;
	}

	void setQuantity(int quantity) {
		this->quantity = quantity;
	}

	void setNext(OrderNode* next) {
		this->next = next;
	}

	void setPrev(OrderNode* prev) {
		this->prev = prev;
	}

	void printOrderNode() {
		std::cout << "Printing Order Node: " << std::endl;
		std::cout <<" Price: " << price << ", Quantity: " << quantity << std::endl;
	}
};