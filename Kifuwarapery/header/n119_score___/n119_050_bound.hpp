#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n080_common__/n080_150_overloadEnumOperators.hpp"


enum Bound {
	BoundNone = 0,
	BoundUpper = Binary< 1>::value, // fail low  で正しい score が分からない。alpha 以下が確定という意味。
	BoundLower = Binary<10>::value, // fail high で正しい score が分からない。beta 以上が確定という意味。
	BoundExact = Binary<11>::value  // alpha と beta の間に score がある。
};
