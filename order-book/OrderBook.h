#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <string>
#include "PriceLevel.h"

using std::unordered_map;
using std::string;

class OrderBook {
	// price maps to a price level, which contains a linked list of orders at that price
	unordered_map<float, PriceLevel> orderBook;

	unordered_map<std::string, PriceLevel> Asks;
	unordered_map<std::string, PriceLevel> Bids;

	PriceLevel* bestBid;
	PriceLevel* bestAsk;

	void addOrder(OrderNode* order) {
		auto priceLevel = orderBook.find(order->getPrice());

		if (priceLevel == orderBook.end()) {
			// create a new price level and add the order to it
			PriceLevel newPriceLevel;
			newPriceLevel.addOrder(order);
			orderBook[order->getPrice()] = newPriceLevel;
		}
		else {
			// add the order to the existing price level
			priceLevel->second.addOrder(order);
		}
	}

	void cancelOrder(string orderId) {

	}
	void modifyOrder(string orderId) {

	}



};

