#pragma once


#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n160_board___/n160_180_knightAttackBb.hpp"
#include "../n160_board___/n160_230_setMaskBB.hpp"
#include "../n161_sqDistan/n161_500_squareDistance.hpp"
#include "../n162_bonaDir_/n162_070_bonaDirAbstract.hpp"
#include "../n220_position/n220_650_position.hpp"


class DirecMisc : public BonaDirAbstract {
public:

	void InitializeSquareDistance(SquareDistance& squareDistance, Square sq0, Square sq1) const {
		// DirecMisc な関係は全て距離 1 にしてもKPE学習には問題無いんだけれど。
		squareDistance.SetValue(sq0,sq1, 0);
		if (
			g_setMaskBb.IsSet(&g_knightAttackBb.GetControllBb(Black, sq0), sq1) ||
			g_setMaskBb.IsSet(&g_knightAttackBb.GetControllBb(White, sq0), sq1)
			)
		{
			squareDistance.SetValue( sq0, sq1, 1);
		}
	}

	void Do2Move(Position& position, Square from, const Square ksq, const Color us) const {
		assert(false); // 最適化の為のダミー
	}

};


extern DirecMisc g_direcMisc;

