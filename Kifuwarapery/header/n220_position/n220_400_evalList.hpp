#pragma once

#include "../n110_square__/n110_500_convSquare.hpp"
#include "../n113_piece___/n113_205_convHandPiece.hpp"


class Position;

class EvalList {
public:
	static const int m_ListSize = 38;

	int m_list0[m_ListSize];
	int m_list1[m_ListSize];
	Square m_listToSquareHand[m_ListSize];
	int m_squareHandToList[SquareHandNum];

	void Set(const Position& pos);
};

extern const Square g_HandPieceToSquareHand[ColorNum][HandPieceNum];

