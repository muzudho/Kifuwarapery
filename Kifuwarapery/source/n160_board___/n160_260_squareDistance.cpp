#include "../../header/n105_color___/n105_100_color.hpp"
#include "../../header/n160_board___/n160_180_knightAttackBb.hpp"
#include "../../header/n160_board___/n160_230_setMaskBb.hpp"
#include "../../header/n160_board___/n160_250_squareRelation.hpp"
#include "../../header/n160_board___/n160_260_squareDistance.hpp"


extern SquareRelation g_squareRelation;
extern KnightAttackBb g_knightAttackBb;
extern SetMaskBb g_setMaskBb;


// �����̋�ň��ōs����ʒu�֌W�ɂ��Ă̋����̃e�[�u���B�j�n�̈ʒu�͋���1�Ƃ���B
SquareDistance g_squareDistance;


void SquareDistance::InitSquareDistance() {
	for (Square sq0 = I9; sq0 < SquareNum; ++sq0) {
		for (Square sq1 = I9; sq1 < SquareNum; ++sq1) {
			switch (g_squareRelation.GetSquareRelation(sq0, sq1)) {
			case DirecMisc:
				// DirecMisc �Ȋ֌W�͑S�ċ��� 1 �ɂ��Ă�KPE�w�K�ɂ͖�薳���񂾂���ǁB
				this->m_squareDistance_[sq0][sq1] = 0;
				if (g_setMaskBb.IsSet(&g_knightAttackBb.GetControllBb(Black, sq0), sq1) || g_setMaskBb.IsSet(&g_knightAttackBb.GetControllBb(White, sq0), sq1))
				    this->m_squareDistance_[sq0][sq1] = 1;
				break;
			case DirecFile:
				this->m_squareDistance_[sq0][sq1] = abs(static_cast<int>(sq0 - sq1) / static_cast<int>(DeltaN));
				break;
			case DirecRank:
				this->m_squareDistance_[sq0][sq1] = abs(static_cast<int>(sq0 - sq1) / static_cast<int>(DeltaE));
				break;
			case DirecDiagNESW:
				this->m_squareDistance_[sq0][sq1] = abs(static_cast<int>(sq0 - sq1) / static_cast<int>(DeltaNE));
				break;
			case DirecDiagNWSE:
				this->m_squareDistance_[sq0][sq1] = abs(static_cast<int>(sq0 - sq1) / static_cast<int>(DeltaNW));
				break;
			default: UNREACHABLE;
			}
		}
	}
}
