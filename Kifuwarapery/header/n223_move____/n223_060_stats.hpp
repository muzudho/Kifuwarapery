#pragma once

#include "../n110_square__/n110_100_square.hpp"
#include "../n110_square__/n110_500_utilSquare.hpp"
#include "../n113_piece___/n113_150_piece.hpp"
#include "../n119_score___/n119_090_score.hpp"

template <bool Gain>
class Stats {
public:
	static const Score m_MaxScore = static_cast<Score>(2000);

	void Clear() { memset(this->m_table_, 0, sizeof(this->m_table_)); }

	Score GetValue(const bool isDrop, const Piece pc, const Square to) const {
		assert(0 < pc && pc < N31_PieceNone);
		assert(UtilSquare::ContainsOf(to));
		return this->m_table_[isDrop][pc][to];
	}

	void Update(const bool isDrop, const Piece pc, const Square to, const Score s) {
		if (Gain) {
			this->m_table_[isDrop][pc][to] = std::max(s, GetValue(isDrop, pc, to) - 1);
		}
		else if (abs(GetValue(isDrop, pc, to) + s) < m_MaxScore) {
			this->m_table_[isDrop][pc][to] += s;
		}
	}

private:
	// [isDrop][piece][square] とする。
	Score m_table_[2][N31_PieceNone][SquareNum];
};

