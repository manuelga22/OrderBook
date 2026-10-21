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

using NodeId = uint64_t;
using PriceLimit = std::numeric_limits<Price>;

// Asks are in ascending order, bids are in descending order. (best Bid/Ask are first in the map)
using AskBook = std::map<Price, PriceLevel>;
using BidsBook = std::map<Price, PriceLevel, std::greater<Price>>;

struct BestPrice {
	Price price;
	PriceLevel* priceLevel;
};

class OrderBook {

private:
	// book to find order nodes by Id
	std::unordered_map<NodeId, OrderNode*> orderBook;
	
	// price maps to a price level, which contains a linked list of orders at that price
	AskBook Asks;
	BidsBook Bids;

public:

	BestPrice getBestBid() {
		if (Bids.empty()) {
			return { -FloatLimit::infinity(), nullptr };
		}
		auto bestBidIt = Bids.begin();
		return { bestBidIt->first, &(bestBidIt->second) };
	}

	BestPrice getBestAsk() {
		if (Asks.empty()) {
			return { FloatLimit::infinity(), nullptr };
		}
		auto bestAskIt = Asks.begin();
		return { bestAskIt->first, &(bestAskIt->second) };
	}

	void add(const OrderNode* order) {
		float price = order->getPrice();

		if (order->getOrderType() == OrderType::BUY) {
			Bids[price].addOrder(order);
		}
		else {
			Asks[price].addOrder(order);
		}

		orderBook[order->getOrderId()] = order;
	}

	void cancelOrder(uint64_t orderId) {

		auto found = orderBook.find(orderId);
		if (found != orderBook.end()) {
			auto& orderNode = found->second;
			auto orderType = orderNode->getOrderType();
			auto orderPrice = orderNode->getPrice();

			if (orderType == OrderType::BUY) {
				auto& priceLevel = Bids[orderPrice];
				priceLevel.removeNode(orderNode);
			}
			else {
				auto& priceLevel = Asks[orderPrice];
				priceLevel.removeNode(orderNode);
			}
		}

	}

	void modifyOrder(uint64_t orderId, float newPrice, int newQuantity) {
		auto found = orderBook.find(orderId);

		if (found != orderBook.end()) {
			found->second->setQuantity(newQuantity);
			// If the price is different, we need to remove the order from 
			// the old price level and add it to the new price level
			if (found->second->getPrice() != newPrice) {
				auto orderCopy = found->second;
				cancelOrder(orderId);
				add(orderCopy);
			}
		}

	}
};

