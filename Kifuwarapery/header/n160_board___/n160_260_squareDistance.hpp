#pragma once


#include "../../header/n110_square__/n110_100_square.hpp"


class SquareDistance {
private:

	// 何かの駒で一手で行ける位置関係についての距離のテーブル。桂馬の位置は距離1とする。
	int m_squareDistance[SquareNum][SquareNum];

public:


	void InitSquareDistance();


	inline int GetSquareDistance(const Square sq1, const Square sq2) const {
		return this->m_squareDistance[sq1][sq2];
	}

};