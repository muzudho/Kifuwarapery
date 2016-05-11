#pragma once


#include "../../n080_common__/n080_100_common.hpp"
#include "../../n113_piece___/n113_150_piece.hpp"
#include "../../n220_position/n220_650_position.hpp"
#include "../../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../../n300_moveGen_/n300_200_pieceTyp/n300_200_500_ptPrograms.hpp"
#include "../../n300_moveGen_/n300_400_genMove_/n300_400_250_bishopRookMovesGenerator.hpp"
#include "../../n300_moveGen_/n300_400_genMove_/n300_400_350_PieceMovesGenerator.hpp"
#include "../../n300_moveGen_/n300_400_genMove_/n300_400_750_dropMoveGenerator.hpp"
//#include "../n374_genMove_/n374_780_MoveGenerator100.hpp""
#include "../../n300_moveGen_/n300_500_piece___/n300_500_070_pieceAbstract.hpp"
#include "../../n300_moveGen_/n300_500_piece___/n300_500_500_pieceArray.hpp"
#include "../../n300_moveGen_/n300_600_moveType/n300_600_100_mtCapture.hpp"
#include "../../n300_moveGen_/n300_600_moveType/n300_600_110_mtNonCapture.hpp"
#include "../../n300_moveGen_/n300_600_moveType/n300_600_120_mtDrop.hpp"
#include "../../n300_moveGen_/n300_600_moveType/n300_600_130_mtCapturePlusPro.hpp"
#include "../../n300_moveGen_/n300_600_moveType/n300_600_140_mtNonCaptureMinusPro.hpp"
#include "../../n300_moveGen_/n300_600_moveType/n300_600_150_mtRecapture.hpp"
#include "../../n300_moveGen_/n300_600_moveType/n300_600_160_mtEvasion.hpp"
#include "../../n300_moveGen_/n300_600_moveType/n300_600_170_mtNonEvasion.hpp"
#include "../../n300_moveGen_/n300_600_moveType/n300_600_180_mtLegal.hpp"
#include "../../n300_moveGen_/n300_600_moveType/n300_600_190_mtLegalAll.hpp"
#include "../../n300_moveGen_/n300_600_moveType/n300_600_200_mtMoveTypeNone.hpp"
#include "../../n300_moveGen_/n300_600_moveType/n300_600_500_mtArray.hpp"


// MoveType の全ての指し手を生成
class MoveGenerator200 {
public:

	inline MoveStack* GenerateMoves_2(
		MoveType mt, MoveStack* pMoveStackList, const Position& pos
	) const {
		return g_moveTypeArray.m_moveTypeArray[mt]->GenerateMove(pMoveStackList, pos);
	}


	// pin は省かない。リキャプチャー専用か。
	template<Color US, Color THEM>
	static inline MoveStack* GenerateMoves_recapture(
		MoveStack* pMoveStackList, const Position& pos, const Square to
	) {
		Bitboard fromBB = pos.GetAttackersTo_a<US, THEM>(to, pos.GetOccupiedBB());
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();

			// 駒の種類によって違う☆（＾ｑ＾）
			const PieceType pieceType = ConvPiece::TO_PIECE_TYPE10(pos.GetPiece(from));

			// TODO: 配列のリミットチェックをしてないぜ☆（＾ｑ＾）
			US == Color::Black
				?
				PiecetypePrograms::m_PIECETYPE_PROGRAMS[pieceType]->Generate2RecaptureMoves_usBlack(pMoveStackList, pos, from, to)
				:
				PiecetypePrograms::m_PIECETYPE_PROGRAMS[pieceType]->Generate2RecaptureMoves_usWhite(pMoveStackList, pos, from, to)
				;
		}
		return pMoveStackList;
	}

};


extern MoveGenerator200 g_moveGenerator200;
