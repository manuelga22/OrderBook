#include "pch.h"
#include "CppUnitTest.h"

#include "price_utils.h"
#include "OrderBook.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace orderbooktest
{
	// --- Start simple: price_utils is pure, no pointers, no ownership. ---
	TEST_CLASS(PriceUtilsTests)
	{
	public:
		TEST_METHOD(ParsesFourDecimals)
		{
			Assert::AreEqual<Price>(1234567, priceFromString("123.4567"));
		}

		TEST_METHOD(PadsShortFraction)
		{
			// "10.5" -> frac 5000 -> 105000 ticks
			Assert::AreEqual<Price>(105000, priceFromString("10.5"));
		}

		TEST_METHOD(RoundTrips)
		{
			Assert::AreEqual(std::string("123.4567"),
				priceToString(priceFromString("123.4567")));
		}
	};

	// --- Then the OrderBook itself. ---
	namespace
	{
		// Build a heap OrderNode; the OrderBook takes ownership once add()'ed
		// (its PriceLevel destructors free the nodes).
		OrderNode* makeOrder(OrderType type, Price price, uint32_t qty)
		{
			OrderNode* n = new OrderNode();
			n->setOrderType(type);
			n->setPrice(static_cast<float>(price));
			n->setQuantity(qty);
			return n;
		}
	}

	TEST_CLASS(OrderBookTests)
	{
	public:
		TEST_METHOD(EmptyBookHasNoBestBidOrAsk)
		{
			OrderBook book;
			Assert::IsNull(book.getBestBid().priceLevel);
			Assert::IsNull(book.getBestAsk().priceLevel);
		}

		TEST_METHOD(AddedBuyBecomesBestBid)
		{
			OrderBook book;
			book.add(makeOrder(OrderType::BUY, 1000000, 5));

			auto bid = book.getBestBid();
			Assert::IsNotNull(bid.priceLevel);
			Assert::AreEqual<Price>(1000000, bid.price);
		}

		TEST_METHOD(HighestBidWins)
		{
			OrderBook book;
			book.add(makeOrder(OrderType::BUY, 1000000, 5));
			book.add(makeOrder(OrderType::BUY, 1010000, 3));

			Assert::AreEqual<Price>(1010000, book.getBestBid().price);
		}

		TEST_METHOD(LowestAskWins)
		{
			OrderBook book;
			book.add(makeOrder(OrderType::SELL, 1020000, 5));
			book.add(makeOrder(OrderType::SELL, 1010000, 3));

			Assert::AreEqual<Price>(1010000, book.getBestAsk().price);
		}
	};
}
