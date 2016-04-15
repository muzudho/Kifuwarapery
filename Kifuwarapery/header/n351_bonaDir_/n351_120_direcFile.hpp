#pragma once


#include <cstdlib>	// abs()
#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n110_square__/n110_250_squareDelta.hpp"
#include "../n160_board___/n160_150_rookAttackBb.hpp"
#include "../n161_sqDistan/n161_500_squareDistance.hpp"
#include "../n162_bonaDir_/n162_070_bonaDirAbstract.hpp"
#include "../n220_position/n220_650_position.hpp"


class DirecFile : public BonaDirAbstract {
public:

	void InitializeSquareDistance(SquareDistance& squareDistance, Square sq0, Square sq1) const {
		squareDistance.SetValue( sq0, sq1,
			abs(static_cast<int>(sq0 - sq1) / static_cast<int>(SquareDelta::DeltaN))
		);
	}

	void Do2Move(Position& position, Square from, const Square ksq, const Color us) const {
		// from の位置から縦に利きを調べると相手玉と、空き王手している駒に当たっているはず。味方の駒が空き王手している駒。
		position.GetStateInfo()->m_checkersBB |=
			g_rookAttackBb.GetControllBbFile(&position.GetOccupiedBB(), from) & position.GetBbOf(us);
	}

};


extern DirecFile g_direcFile;

