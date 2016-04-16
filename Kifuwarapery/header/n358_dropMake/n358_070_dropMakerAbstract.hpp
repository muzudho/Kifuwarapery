#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_600_bitboardAll.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n165_movStack/n165_600_utilMove.hpp"


class DropMakerAbstract {
public:

	// テンプレートを使っている関数で使うには、static にするしかないぜ☆（＾ｑ＾）
	static void MakeDropMovesToRank9ExceptNL(
		const Bitboard& target,
		const Bitboard TRank9BB,
		MoveStack* pMovestack,
		const PieceType haveHand[6],
		int noKnightLanceIdx
	) {
		// このクラスのメソッドを実行しているようではエラーだぜ☆（＾ｑ＾）
		UNREACHABLE;
	};

	// テンプレートを使っている関数で使うには、static にするしかないぜ☆（＾ｑ＾）
	static void MakeDropMovesToRank8ExceptN(
		const Bitboard& target,
		const Bitboard TRank8BB,
		MoveStack* pMovestack,
		const PieceType haveHand[6],
		int noKnightIdx
	) {
		// このクラスのメソッドを実行しているようではエラーだぜ☆（＾ｑ＾）
		UNREACHABLE;
	};

	// テンプレートを使っている関数で使うには、static にするしかないぜ☆（＾ｑ＾）
	static void MakeDropMovesToRank1234567(
		Bitboard& toBB,	// const
		MoveStack* pMovestack,
		const PieceType haveHand[6]
	) {
		// このクラスのメソッドを実行しているようではエラーだぜ☆（＾ｑ＾）
		UNREACHABLE;
	};

};


