#pragma once

#include "../n110_square__/n110_100_square.hpp"
#include "../n110_square__/n110_500_convSquare.hpp"
#include "../n113_piece___/n113_150_piece.hpp"
#include "../n119_score___/n119_090_scoreIndex.hpp"


// History として使うときと、 Gain として使うときの２種類があるぜ☆
template <bool Gain>
class Stats {
public:

	// 点数の最大。
	static const ScoreIndex m_MAX_SCORE = static_cast<ScoreIndex>(2000);

	// 配列をクリアー☆
	void Clear() { memset(this->m_table_, 0, sizeof(this->m_table_)); }

	// 単に配列から、値を引っ張り出しているだけ☆
	ScoreIndex GetValue(const bool isDrop, const Piece pc, const Square to) const {
		assert(0 < pc && pc < N31_PieceNone);
		assert(ConvSquare::ContainsOf(to));
		return this->m_table_[isDrop][pc][to];
	}

	// 値を更新☆
	void UpdateStats(const bool isDrop, const Piece pc, const Square to, const ScoreIndex score) {
		if (Gain) {
			// 差し替え☆
			// 大きくなっていれば上書き☆
			this->m_table_[isDrop][pc][to] = std::max(score, this->GetValue(isDrop, pc, to) - 1);
		}
		else if (abs( this->GetValue(isDrop, pc, to) + score) < Stats::m_MAX_SCORE) {
			// 累計☆
			// 加算後の絶対値がマックス以下ならＯＫ☆
			this->m_table_[isDrop][pc][to] += score;
		}
	}

private:

	// [isDrop][piece][square] とする。
	// 打つ手か、駒の種類・先後は、升は。そのときの点数は。
	ScoreIndex m_table_[2][N31_PieceNone][SquareNum];
};

