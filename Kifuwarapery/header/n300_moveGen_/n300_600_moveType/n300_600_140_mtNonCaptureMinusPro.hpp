#pragma once


#include "../../n165_movStack/n165_300_moveType.hpp"
#include "../../n220_position/n220_650_position.hpp"
#include "../../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../../n300_moveGen_/n300_400_genMove_/n300_400_350_PieceMovesGenerator.hpp"
#include "n300_600_070_mtAbstract.hpp"


class MoveTypeNonCaptureMinusPro : public MoveTypeAbstract {
public:
	// 指し手生成 functor
	// テンプレート引数が複数あり、部分特殊化したかったので、関数ではなく、struct にした。
	// ALL == true のとき、歩、飛、角の不成、香の2段目の不成、香の3段目の駒を取らない不成も生成する。
	MoveStack* GenerateMove(MoveStack* moveStackList, const Position& pos, bool all = false
		) const override {

		if (pos.GetTurn() == Color::Black)
		{
			moveStackList = MoveTypeNonCaptureMinusPro::GENERATE_MOVE_<Color::Black, Color::White>(moveStackList, pos, all);
		}
		else
		{
			moveStackList = MoveTypeNonCaptureMinusPro::GENERATE_MOVE_<Color::White, Color::Black>(moveStackList, pos, all);
		}

		return moveStackList;
	}

private:
	template<Color US, Color THEM>
	static inline MoveStack* GENERATE_MOVE_(MoveStack* moveStackList, const Position& pos, bool all = false
		) {

		// Txxx は先手、後手の情報を吸収した変数。数字は先手に合わせている。
		const Rank TRank6 = (US == Black ? Rank6 : Rank4);
		const Rank TRank7 = (US == Black ? Rank7 : Rank3);
		const Rank TRank8 = (US == Black ? Rank8 : Rank2);
		const Bitboard TRank789BB = g_inFrontMaskBb.GetInFrontMask(US, TRank6);
		const Bitboard TRank1_6BB = g_inFrontMaskBb.GetInFrontMask(THEM, TRank7);
		const Bitboard TRank1_7BB = g_inFrontMaskBb.GetInFrontMask(THEM, TRank8);

		const Bitboard targetPawn = pos.GetOccupiedBB().NotThisAnd(TRank1_6BB);
		const Bitboard targetOther = pos.GetEmptyBB();
		const Square ksq = pos.GetKingSquare(THEM);

		const PieceMoveEvent pmEvent(MoveType::N04_NonCaptureMinusPro, all, pos, ksq);

		moveStackList = PieceMovesGenerator::BuildCards_N01_Pawn<US>(moveStackList, pmEvent, targetPawn);
		moveStackList = PieceMovesGenerator::BuildCards_N02_Lance<US>(moveStackList, pmEvent, targetOther);
		moveStackList = PieceMovesGenerator::BuildCards_N03_Knight<US>(moveStackList, pmEvent, targetOther);
		moveStackList = PieceMovesGenerator::BuildCards_N04_Silver<US>(moveStackList, pmEvent, targetOther);
		moveStackList = PieceMovesGenerator::BuildCards_N05_Bishop<US>(moveStackList, pmEvent, targetOther);
		moveStackList = PieceMovesGenerator::BuildCards_N06_Rook<US>(moveStackList, pmEvent, targetOther);
		moveStackList = PieceMovesGenerator::BuildCards_N16_GoldHorseDragon<US>(moveStackList, pmEvent, targetOther);
		moveStackList = PieceMovesGenerator::BuildCards_N08_King<US>(moveStackList, pmEvent, targetOther);

		return moveStackList;
	}

};


