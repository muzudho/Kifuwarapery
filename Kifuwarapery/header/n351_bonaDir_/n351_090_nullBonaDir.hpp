#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n161_sqDistan/n161_500_squareDistance.hpp"
#include "../n162_bonaDir_/n162_070_bonaDirAbstract.hpp"
#include "../n220_position/n220_650_position.hpp" // FIXME:


class NullBonaDir : public BonaDirAbstract {
public:

	void InitializeSquareDistance(SquareDistance& squareDistance, Square sq0, Square sq1) const {
		UNREACHABLE;
	}

	void Do2Move(Position& position, Square from, const Square ksq, const Color us) const {
		UNREACHABLE;
	}

};


extern NullBonaDir g_nullBonaDir;

