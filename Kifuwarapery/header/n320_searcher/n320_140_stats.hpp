#pragma once

template <bool Gain>
class Stats {
public:
	static const Score m_MaxScore = static_cast<Score>(2000);

	void Clear() { memset(m_table_, 0, sizeof(m_table_)); }

	Score GetValue(const bool isDrop, const Piece pc, const Square to) const {
		assert(0 < pc && pc < PieceNone);
		assert(UtilSquare::ContainsOf(to));
		return m_table_[isDrop][pc][to];
	}

	void Update(const bool isDrop, const Piece pc, const Square to, const Score s) {
		if (Gain) {
			m_table_[isDrop][pc][to] = std::max(s, GetValue(isDrop, pc, to) - 1);
		}
		else if (abs(GetValue(isDrop, pc, to) + s) < m_MaxScore) {
			m_table_[isDrop][pc][to] += s;
		}
	}

private:
	// [isDrop][piece][square] ‚Æ‚·‚éB
	Score m_table_[2][PieceNone][SquareNum];
};

