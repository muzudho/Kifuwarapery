#pragma once

template <bool Gain>
class Stats {
public:
	static const Score MaxScore = static_cast<Score>(2000);

	void clear() { memset(table_, 0, sizeof(table_)); }
	Score value(const bool isDrop, const Piece pc, const Square to) const {
		assert(0 < pc && pc < PieceNone);
		assert(UtilSquare::IsInSquare(to));
		return table_[isDrop][pc][to];
	}
	void update(const bool isDrop, const Piece pc, const Square to, const Score s) {
		if (Gain) {
			table_[isDrop][pc][to] = std::max(s, value(isDrop, pc, to) - 1);
		}
		else if (abs(value(isDrop, pc, to) + s) < MaxScore) {
			table_[isDrop][pc][to] += s;
		}
	}

private:
	// [isDrop][piece][square] ‚Æ‚·‚éB
	Score table_[2][PieceNone][SquareNum];
};

