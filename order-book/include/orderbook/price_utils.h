#pragma once
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <string_view>
#include "orderbook/alias.h"

// Prices are stored as fixed-point integers in units of 1 / PRICE_SCALE.
// With a scale of 10^4, the smallest representable tick is 0.0001 and
// $123.4567 is stored as the integer 1234567. (the Price alias lives in alias.h)

constexpr int64_t PRICE_SCALE = 10000;      // 4 decimal places
constexpr int PRICE_DECIMALS = 4;

// Parse a decimal string such as "123.4567" into fixed-point ticks WITHOUT
// routing through double, so no floating-point rounding error is introduced.
inline Price priceFromString(std::string_view s) {
	bool neg = false;
	size_t i = 0;
	if (i < s.size() && (s[i] == '+' || s[i] == '-')) {
		neg = (s[i] == '-');
		++i;
	}

	int64_t whole = 0;
	for (; i < s.size() && s[i] != '.'; ++i) {
		whole = whole * 10 + (s[i] - '0');
	}

	int64_t frac = 0;
	int fracDigits = 0;
	if (i < s.size() && s[i] == '.') {
		++i;
		for (; i < s.size() && fracDigits < PRICE_DECIMALS; ++i, ++fracDigits) {
			frac = frac * 10 + (s[i] - '0');
		}
	}
	// pad remaining decimal places (e.g. "10.5" -> frac 5000)
	for (; fracDigits < PRICE_DECIMALS; ++fracDigits) {
		frac *= 10;
	}

	int64_t value = whole * PRICE_SCALE + frac;
	return neg ? -value : value;
}

// Format fixed-point ticks back to a human-readable decimal string.
inline std::string priceToString(Price p) {
	bool neg = p < 0;
	int64_t a = neg ? -p : p;
	int64_t whole = a / PRICE_SCALE;
	int64_t frac = a % PRICE_SCALE;

	char buf[32];
	std::snprintf(buf, sizeof(buf), "%s%lld.%0*lld",
		neg ? "-" : "",
		static_cast<long long>(whole),
		PRICE_DECIMALS,
		static_cast<long long>(frac));
	return buf;
}
