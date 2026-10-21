#pragma once
#include <map>
#include <string>
#include <vector>
#include <limits>
#include <functional>
#include <unordered_map>
#include "PriceLevel.h"
#include "OrderType.h"
#include "price_utils.h"

// NodeId is defined in OrderNode.h (included via PriceLevel.h)
using PriceLimit = std::numeric_limits<Price>;

// Asks are in ascending order, bids are in descending order. (best Bid/Ask are first in the map)
using AskBook = std::map<Price, PriceLevel>;
using BidsBook = std::map<Price, PriceLevel, std::greater<Price>>;

struct BestPrice {
	Price price;
	const PriceLevel* priceLevel;
};

class OrderBook {

private:
	// book to find order nodes by Id
	std::unordered_map<NodeId, OrderNode*> orderBook;

	// monotonic exchange-generated id counter; 0 is reserved for "unassigned"
	NodeId nextOrderId = 1;

	// price maps to a price level, which contains a linked list of orders at that price
	AskBook Asks;
	BidsBook Bids;

public:

	BestPrice getBestBid() const noexcept {
		if (Bids.empty()) {
			return { -PriceLimit::min(), nullptr };
		}
		auto bestBidIt = Bids.begin();
		return { bestBidIt->first, &(bestBidIt->second) };
	}

	BestPrice getBestAsk() const noexcept {
		if (Asks.empty()) {
			return { PriceLimit::max(), nullptr};
		}
		auto bestAskIt = Asks.begin();
		return { bestAskIt->first, &(bestAskIt->second) };
	}

	NodeId add(OrderNode* order) {
		// Stamp an exchange-generated id only if the order doesn't already have
		// one, so re-adding during modifyOrder preserves its original id.
		if (order->getOrderId() == 0) {
			order->setOrderId(nextOrderId++);
		}

		int64_t price = order->getPrice();

		if (order->getOrderType() == OrderType::BUY) {
			Bids[price].addOrder(order);
		}
		else if (order->getOrderType() == OrderType::SELL) {
			Asks[price].addOrder(order);
		}
		else if (order->getOrderType() == OrderType::UNDEFINED) {
			throw std::invalid_argument("Order type is undefined");
		}
		else {
			throw std::invalid_argument("Invalid order type");
		}

		orderBook[order->getOrderId()] = order;
	}

	void cancelOrder(uint64_t orderId) {

		if (orderBook.find(orderId) == orderBook.end()) {
			throw std::invalid_argument("Order ID not found");
		}

		auto orderNode = orderBook.at(orderId);
		auto orderType = orderNode->getOrderType();
		auto orderPrice = orderNode->getPrice();
		if (orderType == OrderType::BUY) {
			auto& priceLevel = Bids[orderPrice];
			priceLevel.removeNode(orderNode);
		}
		else if(orderType == OrderType::SELL) {
			auto& priceLevel = Asks[orderPrice];
			priceLevel.removeNode(orderNode);
		}
		delete orderNode;
		orderBook.erase(orderId);

	}

	void modifyOrder(uint64_t orderId, float newPrice, int newQuantity) {

		if (orderBook.find(orderId) == orderBook.end()) {
			throw std::invalid_argument("Order ID not found");
		}

		auto orderNode = orderBook.at(orderId);

		orderNode->setQuantity(newQuantity);
		// If the price is different, we need to remove the order from 
		// the old price level and add it to the new price level
		if (orderNode->getPrice() != newPrice) {
			OrderNode* orderCopy = new OrderNode(*orderNode);
			cancelOrder(orderId);
			add(orderCopy);
		}
		

	}
};

