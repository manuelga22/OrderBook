#include "orderbook/PriceLevel.h"
#include "orderbook/OrderNode.h"


int main() {
	PriceLevel pl{};

	OrderNode order{};
	order.setOrderType(OrderType::BUY);
	order.setQuantity(100);
	order.setPrice(10.5);

	OrderNode order2{};
	order2.setOrderType(OrderType::BUY);
	order2.setQuantity(200);
	order2.setPrice(10.5);

	OrderNode copyOrder = order2;

	pl.addOrder(&order);
	pl.addOrder(&order2);
	pl.addOrder(&copyOrder);
	pl.printPriceLevel();

	pl.removeNode(&order2);
	pl.printPriceLevel();

	pl.removeNode(&order);
	pl.printPriceLevel();

	// Add an order and pop it
	pl.addOrder(&order2);
	OrderNode* lastOrder = pl.popLastOrder();
	lastOrder->printOrderNode();
	pl.printPriceLevel();


	return 0;

}