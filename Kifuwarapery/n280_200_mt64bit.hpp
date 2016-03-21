#pragma once

#include "header/n080_common__/n080_100_common.hpp"

// 64bit のランダムな値を返す為のクラス
class MT64bit : public std::mt19937_64 {
public:
	MT64bit() : std::mt19937_64() {}
	explicit MT64bit(const unsigned int seed) : std::mt19937_64(seed) {}
	u64 random() {
		return (*this)();
	}
	u64 randomFewBits() {
		return random() & random() & random();
	}
};

extern MT64bit g_mt64bit;

