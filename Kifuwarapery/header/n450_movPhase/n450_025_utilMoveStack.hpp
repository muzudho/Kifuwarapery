#pragma once


#include <algorithm> //std::swap std::max_element
#include "../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"


class UtilMoveStack {
public:

	// 最も score の高い moveStack のポインタを返す。
	// MoveStack の数が多いとかなり時間がかかるので、
	// 駒打ちを含むときに使用してはならない。
	static inline MoveStack* PickBest_AndSort(MoveStack* currMove, MoveStack* lastMove) {
		// 現在のカードと、一番点数の高いカードの位置をスワップしていないか☆？
		std::swap(*currMove, *std::max_element(currMove, lastMove));
		return currMove;
	}

};