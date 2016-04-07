#pragma once


#include "../../header/n110_square__/n110_100_square.hpp"


class SquareDistance {
private:

	// �����̋�ň��ōs����ʒu�֌W�ɂ��Ă̋����̃e�[�u���B�j�n�̈ʒu�͋���1�Ƃ���B
	int m_squareDistance[SquareNum][SquareNum];

public:


	void InitSquareDistance();


	inline int GetSquareDistance(const Square sq1, const Square sq2) const {
		return this->m_squareDistance[sq1][sq2];
	}

};