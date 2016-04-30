#pragma once


#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "n160_100_bitboard.hpp"
#include "n160_108_slideBits.hpp"


//────────────────────────────────────────────────────────────────────────────────
// 香
//────────────────────────────────────────────────────────────────────────────────
class LanceAttackBb {
public:
	// メモリ節約をせず、無駄なメモリを持っている。
	Bitboard m_controllBb[g_COLOR_NUM][SquareNum][128];

private:
	// メモリ節約をせず、無駄なメモリを持っている。
	Bitboard m_controllBbToEdge_[g_COLOR_NUM][SquareNum];
	Bitboard m_lanceCheckTable_[g_COLOR_NUM][SquareNum];

public:

	void Initialize();
	void InitCheckTableLance();

	// 障害物が無いときの利きの Bitboard
	// g_rookAttack, g_bishopAttack, g_lanceAttack を設定してから、この関数を呼ぶこと。
	void InitializeToEdge();

	// todo: 香車の筋がどこにあるか先に分かっていれば、Bitboard の片方の変数だけを調べれば良くなる。
	Bitboard GetControllBb(const Bitboard& thisBitboard, const Color c, const Square sq) const {
		const int part = Bitboard::Part(sq);
		const int index = (thisBitboard.GetP(part) >> g_slideBits.m_slide[sq]) & 127;
		return this->m_controllBb[c][sq][index];
	}

	Bitboard GetControllBbToEdge(const Color c, const Square sq) const {
		return this->m_controllBbToEdge_[c][sq];
	}

	Bitboard LanceCheckTable(const Color c, const Square sq) const {
		return this->m_lanceCheckTable_[c][sq];
	}

private:

	Bitboard LanceBlockMask(const Square square);

	Bitboard LanceAttackCalc(const Color c, const Square square, const Bitboard& occupied);

};


// クラス定義のあとに書くとビルドできるぜ☆（＾ｑ＾）
extern LanceAttackBb g_lanceAttackBb;
