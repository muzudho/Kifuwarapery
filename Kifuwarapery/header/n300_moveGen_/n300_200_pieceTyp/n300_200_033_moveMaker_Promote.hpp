#pragma once


#include "../../n165_movStack/n165_400_move.hpp"


class MoveMaker_Promote {
public:

	static inline void APPEND_PROMOTE_FLAG(Move& move)
	{
		//, MoveType mt_forAssert, const PieceType pt_forAssert
		//assert(!(
		//	// éùÇøãÓÇÃë≈ÇøçûÇ›Ç…ê¨ÇËÇÕñ≥Ç¢Åô
		//	mt_forAssert == N02_Drop ||
		//	// ã‡Ç∆ã Ç…ê¨ÇËÇÕñ≥Ç¢Åô
		//	pt_forAssert == N07_Gold || pt_forAssert == N08_King
		//));
		move |= g_MOVE_PROMOTE_FLAG;
	}

};

