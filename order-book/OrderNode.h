#pragma once
#include <cstdint>
#include "OrderType.h"

using NodeId = uint64_t;

class OrderNode {

private:

    OrderNode* next = nullptr;
    OrderNode* prev = nullptr;

	// 0 is reserved for "unassigned"; the OrderBook stamps a real id on add()
	NodeId orderId = 0;
	// price is scaled using PRICE_SCALE_FACTOR to avoid floating point precision issues
	int64_t price = 0;
	OrderType orderType = OrderType::UNDEFINED;
	uint32_t quantity = 0;

public:

	OrderNode() = default;

	// copy constructor
	OrderNode(const OrderNode& other)
	  : next(nullptr),
		prev(nullptr),
		orderId(other.orderId),
		price(other.price),
		orderType(other.orderType),
		quantity(other.quantity) {}

	NodeId getOrderId() const {
		return orderId;
	}

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

	void setOrderId(NodeId orderId) {
		this->orderId = orderId;
	}

	void setPrice(int64_t price) {
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