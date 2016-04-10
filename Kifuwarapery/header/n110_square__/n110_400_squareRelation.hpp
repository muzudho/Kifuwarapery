#pragma once


#include "../n110_square__/n110_100_square.hpp"
#include "../n110_square__/n110_300_direction.hpp"


class SquareRelation {
private:
	// 2つの位置関係のテーブル
	Direction m_squareRelation_[SquareNum][SquareNum];

public:

	void Initialize();

	inline Direction GetSquareRelation(const Square sq1, const Square sq2) const {
		return this->m_squareRelation_[sq1][sq2];
	}


	// from, to, ksq が 縦横斜めの同一ライン上にあれば true を返す。
	template <bool FROM_KSQ_NEVER_BE_DIRECMISC>
	inline bool IsAligned(const Square from, const Square to, const Square ksq) const {

		const Direction direc = SquareRelation::GetSquareRelation(from, ksq);

		if (FROM_KSQ_NEVER_BE_DIRECMISC) {
			assert(direc != DirecMisc);
			return (direc == SquareRelation::GetSquareRelation(from, to));
		}
		else {
			return (direc != DirecMisc && direc == SquareRelation::GetSquareRelation(from, to));
		}
	}

};


// クラス定義のあとに書くとビルドできるぜ☆（＾ｑ＾）
extern SquareRelation g_squareRelation;
