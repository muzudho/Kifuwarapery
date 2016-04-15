#include "../../header/n105_color___/n105_100_color.hpp"
#include "../../header/n110_square__/n110_250_squareDelta.hpp"
#include "../../header/n110_square__/n110_400_squareRelation.hpp"
#include "../../header/n160_board___/n160_180_knightAttackBb.hpp"
#include "../../header/n160_board___/n160_230_setMaskBb.hpp"
#include "../../header/n161_sqDistan/n161_500_squareDistance.hpp"
#include "../../header/n163_sqDistan/n163_600_utilSquareDistance.hpp"
#include "../../header/n351_bonaDir_/n351_500_bonaDirArray.hpp"



void UtilSquareDistance::InitSquareDistance(SquareDistance& squareDistance) {
	for (Square sq0 = I9; sq0 < SquareNum; ++sq0) {
		for (Square sq1 = I9; sq1 < SquareNum; ++sq1) {

			g_bonaDirArray[g_squareRelation.GetSquareRelation(sq0, sq1)]->InitializeSquareDistance(squareDistance, sq0, sq1);

			/*
			switch (g_squareRelation.GetSquareRelation(sq0, sq1)) {
			case N00_DirecMisc:
				// DirecMisc な関係は全て距離 1 にしてもKPE学習には問題無いんだけれど。
				this->m_squareDistance_[sq0][sq1] = 0;
				if (g_setMaskBb.IsSet(&g_knightAttackBb.GetControllBb(Black, sq0), sq1) ||
					g_setMaskBb.IsSet(&g_knightAttackBb.GetControllBb(White, sq0), sq1))
				    this->m_squareDistance_[sq0][sq1] = 1;
				break;
			case N02_DirecFile:
				this->m_squareDistance_[sq0][sq1] = abs(static_cast<int>(sq0 - sq1) / static_cast<int>(SquareDelta::DeltaN));
				break;
			case N03_DirecRank:
				this->m_squareDistance_[sq0][sq1] = abs(static_cast<int>(sq0 - sq1) / static_cast<int>(SquareDelta::DeltaE));
				break;
			case N04_DirecDiagNESW:
				this->m_squareDistance_[sq0][sq1] = abs(static_cast<int>(sq0 - sq1) / static_cast<int>(SquareDelta::DeltaNE));
				break;
			case N05_DirecDiagNWSE:
				this->m_squareDistance_[sq0][sq1] = abs(static_cast<int>(sq0 - sq1) / static_cast<int>(SquareDelta::DeltaNW));
				break;
			default: UNREACHABLE;
			}
			*/

		}
	}
}
