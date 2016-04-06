#pragma once

#include "n160_100_bitboard.hpp"


//────────────────────────────────────────────────────────────────────────────────
// 香
//────────────────────────────────────────────────────────────────────────────────
class LanceAttackBb {
public:
	// メモリ節約をせず、無駄なメモリを持っている。
	Bitboard m_controllBb[ColorNum][SquareNum][128];
	Bitboard m_controllBbToEdge[ColorNum][SquareNum];
	Bitboard m_lanceCheckTable[ColorNum][SquareNum];

public:
	// todo: 香車の筋がどこにあるか先に分かっていれば、Bitboard の片方の変数だけを調べれば良くなる。
	inline Bitboard GetControllBb(const Bitboard* thisBitboard, const Color c, const Square sq) const {
		const int part = Bitboard::Part(sq);
		const int index = ((*thisBitboard).GetP(part) >> ConfigBits::m_slide[sq]) & 127;
		return this->m_controllBb[c][sq][index];
	}

	inline Bitboard GetControllBbToEdge(const Color c, const Square sq) const {
		return this->m_controllBbToEdge[c][sq];
	}

	inline Bitboard LanceCheckTable(const Color c, const Square sq) const {
		return this->m_lanceCheckTable[c][sq];
	}
};
