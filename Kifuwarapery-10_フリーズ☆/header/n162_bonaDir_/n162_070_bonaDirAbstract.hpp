#pragma once


#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n161_sqDistan/n161_500_squareDistance.hpp"


#include "../n220_position/n220_650_position.hpp" //FIXME:
//class Position;


class BonaDirAbstract {
public:

	virtual void InitializeSquareDistance(SquareDistance& squareDistance, Square sq0, Square sq1) const = 0;

	virtual void Do2Move(Position& position, Square from, const Square ksq, const Color us) const = 0;
};


