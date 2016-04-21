#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n113_piece___/n113_205_utilHandPiece.hpp"
#include "../n165_movStack/n165_300_moveType.hpp"
#include "../n165_movStack/n165_310_promoteMode.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_600_utilMove.hpp"
#include "../n220_position/n220_660_utilMovePos.hpp"


class Position;


class MakePromoteMove {
public:

	//(^q^)新型
	// MoveType によって指し手生成関数を使い分ける。
	// Drop, Check, Evasion, の場合は別で指し手生成を行う。
	static inline Move GetSelectedMakeMove_ExceptPromote_mt(MoveType mt, const PieceType pt, const Square from, const Square to, const Position& pos) {
		// キャプチャー系か、非キャプチャー系かで処理を分けるぜ☆（＾ｑ＾）
		Move move = ((mt == N01_NonCapture || mt == N04_NonCaptureMinusPro) ?
			UtilMove::MakeMove(pt, from, to) :
			UtilMovePos::MakeCaptureMove(pt, from, to, pos)
		);
		return move;
	}
	// 非キャプチャー系と分かっているならこちら☆（＾ｑ＾）
	static inline Move GetSelectedMakeMove_ExceptPromote_CaptureCategory( const PieceType pt, const Square from, const Square to, const Position& pos) {
		//MoveType mt_forAssert, assert(!(mt_forAssert == N01_NonCapture || mt_forAssert == N04_NonCaptureMinusPro), "");
		return UtilMovePos::MakeCaptureMove(pt, from, to, pos);
	}

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


extern MakePromoteMove g_makePromoteMove;
