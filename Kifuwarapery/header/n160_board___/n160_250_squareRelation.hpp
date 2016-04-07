#pragma once


#include "../n110_square__/n110_300_direction.hpp"


class SquareRelation {
private:
	// 2�̈ʒu�֌W�̃e�[�u��
	Direction m_squareRelation_[SquareNum][SquareNum];

public:

	void Initialize();

	inline Direction GetSquareRelation(const Square sq1, const Square sq2) const {
		return this->m_squareRelation_[sq1][sq2];
	}


	// from, to, ksq �� �c���΂߂̓��ꃉ�C����ɂ���� true ��Ԃ��B
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