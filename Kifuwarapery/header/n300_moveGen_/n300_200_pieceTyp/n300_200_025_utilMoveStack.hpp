#pragma once


#include <algorithm> //std::swap std::max_element
#include "../n300_200_pieceTyp/n300_200_020_moveStack.hpp"


class UtilMoveStack {
public:

	// 最も score の高い moveStack のポインタを返す。
	// MoveStack の数が多いとかなり時間がかかるので、
	// 駒打ちを含むときに使用してはならない。
	static inline MoveStack* PickBest(MoveStack* currMove, MoveStack* lastMove) {
		std::swap(*currMove, *std::max_element(currMove, lastMove));
		return currMove;
	}

};