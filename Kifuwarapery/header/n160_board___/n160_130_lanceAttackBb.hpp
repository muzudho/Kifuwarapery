#pragma once


#include "n160_050_configBits.hpp"
#include "n160_100_bitboard.hpp"


extern ConfigBits g_configBits;


//────────────────────────────────────────────────────────────────────────────────
// 香
//────────────────────────────────────────────────────────────────────────────────
class LanceAttackBb {
public:
	// メモリ節約をせず、無駄なメモリを持っている。
	Bitboard m_controllBb[ColorNum][SquareNum][128];

private:
	// メモリ節約をせず、無駄なメモリを持っている。
	Bitboard m_controllBbToEdge_[ColorNum][SquareNum];
	Bitboard m_lanceCheckTable_[ColorNum][SquareNum];

public:

	void Initialize();
	void InitCheckTableLance();

	// 障害物が無いときの利きの Bitboard
	// g_rookAttack, g_bishopAttack, g_lanceAttack を設定してから、この関数を呼ぶこと。
	void InitializeToEdge();

	// todo: 香車の筋がどこにあるか先に分かっていれば、Bitboard の片方の変数だけを調べれば良くなる。
	inline Bitboard GetControllBb(const Bitboard* thisBitboard, const Color c, const Square sq) const {
		const int part = Bitboard::Part(sq);
		const int index = ((*thisBitboard).GetP(part) >> g_configBits.m_slide[sq]) & 127;
		return this->m_controllBb[c][sq][index];
	}

	inline Bitboard GetControllBbToEdge(const Color c, const Square sq) const {
		return this->m_controllBbToEdge_[c][sq];
	}

	inline Bitboard LanceCheckTable(const Color c, const Square sq) const {
		return this->m_lanceCheckTable_[c][sq];
	}

private:

	Bitboard LanceBlockMask(const Square square);

	Bitboard LanceAttackCalc(const Color c, const Square square, const Bitboard& occupied);

};


extern LanceAttackBb g_lanceAttackBb;
