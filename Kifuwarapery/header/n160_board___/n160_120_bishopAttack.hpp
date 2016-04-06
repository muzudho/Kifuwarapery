#pragma once

#include "../n160_board___/n160_100_bitboard.hpp"

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// �p
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
class BishopAttackBb {
public:
	Bitboard	m_bishopAttack[20224];
	int			m_bishopAttackIndex[SquareNum];
	Bitboard	m_bishopBlockMask[SquareNum];
	Bitboard	m_bishopAttackToEdge[SquareNum];

public:
	inline Bitboard GetControllBbToEdge(const Square sq) const {
		return this->m_bishopAttackToEdge[sq];
	}

};
