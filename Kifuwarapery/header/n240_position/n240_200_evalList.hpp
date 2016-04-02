#pragma once

#include "../../header/n110_square__/n110_500_utilSquare.hpp"
#include "../../header/n120_brdEntry/n120_200_piece.hpp"

class Position;

class EvalList {
public:
	static const int ListSize = 38;

	int list0[ListSize];
	int list1[ListSize];
	Square listToSquareHand[ListSize];
	int squareHandToList[SquareHandNum];

	void set(const Position& pos);
};

extern const Square HandPieceToSquareHand[ColorNum][HandPieceNum];

