#pragma once

#include "../../n080_common__/n080_100_common.hpp"
#include "../../n113_piece___/n113_205_convHandPiece.hpp"
#include "../../n165_movStack/n165_300_moveType.hpp"
#include "../../n165_movStack/n165_310_promoteMode.hpp"
#include "../../n165_movStack/n165_400_move.hpp"
#include "../../n165_movStack/n165_420_convMove.hpp"
#include "../../n220_position/n220_660_utilMovePos.hpp"
#include "../../n220_position/n220_650_position.hpp"


//class Position;


// 成る手☆？
class MoveMaker_ExceptPromote {
public:

	//(^q^)新型2
	// MoveType によって指し手生成関数を使い分ける。
	// Drop, Check, Evasion, の場合は別で指し手生成を行う。
	static inline Move BuildCard_mt2(// 成らない☆？
		MoveType mt,
		const Position& pos,		// （５）
		const Move pieceTypeAsMove,	// （４）
		const Square from,			// （２）
		const Square to				// （１）
		) {

		Move move = (mt == N01_NonCapture || mt == N04_NonCaptureMinusPro)
			?
			// 駒を取らない手☆
			ConvMove::FROM_PT_SRC_DST20(
				pieceTypeAsMove,
				from,
				to
			)
			:
			// 駒を取る手☆
			UtilMovePos::BUILD_CARD_CAPTURE(
				pos,				// 駒を取るために☆
				pieceTypeAsMove,
				from,
				to
			)
		;
		return move;
	}
	//(^q^)新型1
	// MoveType によって指し手生成関数を使い分ける。
	// Drop, Check, Evasion, の場合は別で指し手生成を行う。
	static inline Move BuildCard_goldHorseDragon(// 成らない☆？
		MoveType mt,
		const Position& pos,		// （５）
		const PieceType pt,			// （４） // 新型２ならここを Move にできるぜ☆（＾ｑ＾）
		const Square from,			// （２）
		const Square to				// （１）
		) {

		// キャプチャー系か、非キャプチャー系かで処理を分けるぜ☆（＾ｑ＾）
		Move move = ((mt == N01_NonCapture || mt == N04_NonCaptureMinusPro) ?
			ConvMove::FROM_PT_SRC_DST20(
				ConvMove::FROM_PIECETYPE_ONBOARD10(pt),
			from, to) :
			UtilMovePos::BUILD_CARD_CAPTURE(
				pos,
				ConvMove::FROM_PIECETYPE_ONBOARD10( pt),
				from,
				to
				)
		);
		return move;
	}

	// 非キャプチャー系と分かっているならこちら☆（＾ｑ＾）
	static inline Move BuildCard_CaptureCategory(// 成らない☆？
		const Position& pos,			// （５）
		const Move pieceTypeAsMove,		// （４）
		const Square from,				// （２）
		const Square to					// （１）
		) {
		//MoveType mt_forAssert, assert(!(mt_forAssert == N01_NonCapture || mt_forAssert == N04_NonCaptureMinusPro), "");
		return UtilMovePos::BUILD_CARD_CAPTURE(pos, pieceTypeAsMove, from, to);
	}

};


extern MoveMaker_ExceptPromote g_makePromoteMove;
