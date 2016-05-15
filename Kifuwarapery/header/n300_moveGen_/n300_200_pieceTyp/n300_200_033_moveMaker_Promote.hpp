#pragma once


#include "../../n165_movStack/n165_400_move.hpp"


class MoveMaker_Promote {
public:

	static inline void APPEND_PROMOTE_FLAG(Move& move)
	{
		//, MoveType mt_forAssert, const PieceType pt_forAssert
		//assert(!(
		//	// 持ち駒の打ち込みに成りは無い☆
		//	mt_forAssert == N02_Drop ||
		//	// 金と玉に成りは無い☆
		//	pt_forAssert == N07_Gold || pt_forAssert == N08_King
		//));
		move |= g_MOVE_PROMOTE_FLAG;
	}

};

