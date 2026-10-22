#pragma once
#include <cstdint>

// Fundamental type aliases shared across the order book.
using NodeId = uint64_t;    // exchange-generated order id (0 = unassigned)
using Price = int64_t;      // fixed-point price in ticks (see price_utils.h)
using Quantity = uint32_t;  // order / fill size
