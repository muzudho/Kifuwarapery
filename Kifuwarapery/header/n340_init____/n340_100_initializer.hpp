#pragma once

#include "../n080_common__/n080_050_ifdef.hpp"
#include "../n080_common__/n080_100_common.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"

class Initializer {
public:
	void InitTable();



private:


	void InitSquareRelation();

	void InitAttackToEdge();

	void InitBetweenBB();

	void InitCheckTable();

	void InitSquareDistance();

};

