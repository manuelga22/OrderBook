#pragma once
#include <map>
#include <string>
#include <vector>
#include <limits>
#include <functional>
#include <unordered_map>
#include "orderbook/PriceLevel.h"
#include "orderbook/OrderType.h"
#include "orderbook/price_utils.h"
#include "orderbook/alias.h"
#include "orderbook/Trade.h"

// NodeId, Price and Quantity are defined in alias.h
using PriceLimit = std::numeric_limits<Price>;

// Asks are in ascending order, bids are in descending order. (best Bid/Ask are first in the map)
using AskBook = std::map<Price, PriceLevel, std::less<Price>>;
using BidsBook = std::map<Price, PriceLevel, std::greater<Price>>;
using Trades = std::vector<Trade>;

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

	// Fill an incoming order against the resting orders at its own price level
	// in the given book. Templated so it works for both AskBook and BidsBook,
	// which differ only in their comparator.
	template <typename Book>
	Trades fillAtPrice(Book& book, OrderNode& order) {

		if (book.empty()) {
			return {};
		}

		std::unique_ptr<BestPrice> bp;

		if constexpr (std::is_same_v<Book, AskBook>) {
			// Incoming BUY: ask must be <= buy limit
			bp = std::make_unique(getBestAsk());
			if (bp->price >= order.getPrice()) {
				return {};
			}
		}
		else if constexpr (std::is_same_v<Book, BidsBook>) {
			// Incoming SELL: bid must be >= sell limit
			bp = std::make_unique(getBestBid());
			if (bp->price <= order.getPrice()) {
				return {};
			}
		}

		return bp->priceLevel->attemptFill(order);
	}


public:

	// Returns the best (highest) bid, or nullptr when the book is empty.
	// The caller owns the returned BestPrice and must delete it.
	BestPrice* getBestBid() const {
		if (Bids.empty()) {
			return nullptr;
		}
		auto bestBidIt = Bids.begin();
		return new BestPrice{ bestBidIt->first, &(bestBidIt->second) };
	}

	// Returns the best (lowest) ask, or nullptr when the book is empty.
	// The caller owns the returned BestPrice and must delete it.
	BestPrice* getBestAsk() const {
		if (Asks.empty()) {
			return nullptr;
		}
		auto bestAskIt = Asks.begin();
		return new BestPrice{ bestAskIt->first, &(bestAskIt->second) };
	}

	// The book takes ownership of the referenced node (PriceLevel deletes it),
	// so callers must pass a heap-allocated OrderNode that outlives the book.
	NodeId add(OrderNode& order) {
		// Stamp an exchange-generated id only if the order doesn't already have
		// one, so re-adding during modifyOrder preserves its original id.
		if (order.getOrderId() == 0) {
			order.setOrderId(nextOrderId++);
		}

		Price price = order.getPrice();

		if (order.getOrderType() == OrderType::BUY) {
			Bids[price].addOrder(order);
		}
		else if (order.getOrderType() == OrderType::SELL) {
			Asks[price].addOrder(order);
		}
		else if (order.getOrderType() == OrderType::UNDEFINED) {
			throw std::invalid_argument("Order type is undefined");
		}
		else {
			throw std::invalid_argument("Invalid order type");
		}

		orderBook[order.getOrderId()] = &order;
		return order.getOrderId();
	}

	Trades attemptFill(OrderNode& order) {

		OrderType t = order.getOrderType();

		if (t == OrderType::BUY) {
			return fillAtPrice(Asks, order);
		}
		else if (t == OrderType::SELL) {
			return fillAtPrice(Bids, order);
		}

		return {};
	}
		

	void cancelOrder(NodeId orderId) {

		if (orderBook.find(orderId) == orderBook.end()) {
			throw std::invalid_argument("Order ID not found");
		}

		auto orderNode = orderBook.at(orderId);
		auto orderType = orderNode->getOrderType();
		auto orderPrice = orderNode->getPrice();
		if (orderType == OrderType::BUY) {
			auto& priceLevel = Bids[orderPrice];
			priceLevel.removeNode(*orderNode);
		}
		else if(orderType == OrderType::SELL) {
			auto& priceLevel = Asks[orderPrice];
			priceLevel.removeNode(*orderNode);
		}
		delete orderNode;
		orderBook.erase(orderId);

	}

	void modifyOrder(NodeId orderId, Price newPrice, Quantity newQuantity) {

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
			add(*orderCopy);
		}
		

	}
};

