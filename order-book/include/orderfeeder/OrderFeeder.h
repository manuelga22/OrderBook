#pragma once
#include <string>
#include "orderbook/OrderBook.h"

using AssetName = std::string;

class OrderFeeder
{
public:

	OrderFeeder(AssetName assetName)
		: assetName_{ assetName }	
	{ }

	bool match(OrderNode& order) {
		if (order.getOrderType() == OrderType::BUY) {

			

		}
		else if (order.getOrderType() == OrderType::SELL) {

		}
		else if (order.getOrderType() == OrderType::UNDEFINED) {
			throw std::invalid_argument("Order type is undefined");
		}
		else {
			throw std::invalid_argument("Invalid order type");
		})
	}

private:
	AssetName assetName_;
	OrderBook book;
};

