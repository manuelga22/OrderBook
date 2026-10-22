#pragma once
#include "orderbook/OrderType.h"
#include "orderbook/alias.h"

class OrderNode {

private:

    OrderNode* next = nullptr;
    OrderNode* prev = nullptr;

	// 0 is reserved for "unassigned"; the OrderBook stamps a real id on add()
	NodeId orderId = 0;
	// price is scaled using PRICE_SCALE_FACTOR to avoid floating point precision issues
	Price price = 0;
	OrderType orderType = OrderType::UNDEFINED;
	Quantity quantity = 0;
	Quantity quantityFilled = 0;


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

	NodeId getOrderId() const { return orderId; }
	Price getPrice() const { return price; }
	OrderType getOrderType() const { return orderType; }
	Quantity getQuantity() const { return quantity; }
	Quantity getQuantityFilled() const { return quantityFilled; }
	Quantity getQuantityRemaining() const { return getQuantity() - getQuantityFilled(); }
	OrderNode* getNext() const { return next; }
	OrderNode* getPrev() const { return prev; }

	void setOrderId(NodeId orderId) {
		this->orderId = orderId;
	}

	void setPrice(Price price) {
		this->price = price;
	}

	void setOrderType(OrderType orderType) {
		this->orderType = orderType;
	}

	void setQuantity(Quantity quantity) {
		this->quantity = quantity;
	}

	void setNext(OrderNode* next) {
		this->next = next;
	}

	void setPrev(OrderNode* prev) {
		this->prev = prev;
	}

	void fillQuantity(Quantity quantityFilled) {
		if (quantityFilled > getQuantityRemaining()) {
			throw std::invalid_argument("Cannot fill more than remaining quantity");
		}
		this->quantityFilled += quantityFilled;
	}

	void printOrderNode() {
		std::cout << "Printing Order Node: " << std::endl;
		std::cout <<" Price: " << price << ", Quantity: " << quantity << std::endl;
	}
};