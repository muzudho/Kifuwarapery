#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n110_square__/n110_200_rank.hpp"
#include "../n110_square__/n110_250_squareDelta.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n113_piece___/n113_150_piece.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_600_bitboardAll.hpp"
#include "../n165_movStack/n165_300_moveType.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_640_utilAttack.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n350_pieceTyp/n350_500_ptArray.hpp"
#include "../n372_piece___/n372_070_pieceAbstract.hpp"
#include "../n372_piece___/n372_500_pieceArray.hpp"
#include "n374_040_mtEvent.hpp"
#include "n374_250_bishopRookMovesGenerator.hpp"


// ��, ������A�n�A���̎w���萶��
FORCE_INLINE MoveStack* GeneratePieceMoves_pt(
	PieceType pt1,
	MoveType mt,
	MoveTypeEvent& mtEvent,//Color us,MoveStack* moveStackList,	const Position& pos,
	bool all,
	const Bitboard& target,
	const Square /*ksq*/
) {
	assert(pt1 == N16_GoldHorseDragon, "");
	// ���A�����A�n�A����bitboard���܂Ƃ߂Ĉ����B
	Bitboard fromBB = (mtEvent.m_pos.GetGoldsBB() | mtEvent.m_pos.GetBbOf(N13_Horse, N14_Dragon)) & mtEvent.m_pos.GetBbOf(mtEvent.m_us);
	while (fromBB.Exists1Bit()) {
		const Square from = fromBB.PopFirstOneFromI9();
		// from �ɂ����̎�ނ𔻕�
		const PieceType pt = UtilPiece::ToPieceType(mtEvent.m_pos.GetPiece(from));
		Bitboard toBB = UtilAttack::GetAttacksFrom(pt, mtEvent.m_us, from, mtEvent.m_pos.GetOccupiedBB()) & target;
		while (toBB.Exists1Bit()) {
			const Square to = toBB.PopFirstOneFromI9();
			(*mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, pt, from, to, mtEvent.m_pos);
		}
	}
	return mtEvent.m_moveStackList;
}


// ���̏ꍇ
FORCE_INLINE MoveStack* GeneratePieceMoves_N01_Pawn(
	MoveType mt,
	MoveTypeEvent& mtEvent,
	bool all,
	const Bitboard& target, const Square /*ksq*/
) {
	// Txxx �͐��A���̏����z�������ϐ��B�����͐��ɍ��킹�Ă���B
	const Bitboard TRank789BB = g_inFrontMaskBb.GetInFrontMask(mtEvent.m_us, (mtEvent.m_us == Black ? Rank6 : Rank4));
	const SquareDelta TDeltaS = (mtEvent.m_us == Black ? DeltaS : DeltaN);

	Bitboard toBB = Bitboard:: PawnAttack(mtEvent.m_pos.GetBbOf(N01_Pawn, mtEvent.m_us), mtEvent.m_us) & target;

	// ����
	if (mt != N04_NonCaptureMinusPro) {
		Bitboard toOn789BB = toBB & TRank789BB;
		if (toOn789BB.Exists1Bit()) {

			toBB.AndEqualNot(TRank789BB);

			Square to;

			FOREACH_BB(toOn789BB, to, {
				const Square from = to + TDeltaS;
				(*mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2(mt, N01_Pawn, from, to, mtEvent.m_pos);
			if (mt == N07_NonEvasion || all) {
				const Rank TRank9 = (mtEvent.m_us == Black ? Rank9 : Rank1);
				if (UtilSquare::ToRank(to) != TRank9) {
					(*mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, N01_Pawn, from, to, mtEvent.m_pos);
				}
			}
			});
		}
	}
	else {
		assert(!(target & TRank789BB).Exists1Bit());
	}

	// �c��(�s��)
	// toBB �� 8~4 �i�ڂ܂ŁB
	Square to;
	FOREACH_BB(toBB, to, {
		const Square from = to + TDeltaS;
		(*mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, N01_Pawn, from, to, mtEvent.m_pos);
	});

	return mtEvent.m_moveStackList;
}//���Z�q�̃I�[�o�[���[�h�̒�`�H


 // ���Ԃ̏ꍇ
FORCE_INLINE MoveStack* GeneratePieceMoves_N02_Lance(
	MoveType mt,
	MoveTypeEvent& mtEvent,
	bool all,
	const Bitboard& target, const Square /*ksq*/
) {
	Bitboard fromBB = mtEvent.m_pos.GetBbOf(N02_Lance, mtEvent.m_us);
	while (fromBB.Exists1Bit()) {
		const Square from = fromBB.PopFirstOneFromI9();
		Bitboard toBB = PieceTypeArray::m_lance.GetAttacks2From(mtEvent.m_pos.GetOccupiedBB(), mtEvent.m_us, from) & target;
		do {
			if (toBB.Exists1Bit()) {
				// ����Ώۂ͕K����ȉ��Ȃ̂ŁAtoBB �̃r�b�g�� 0 �ɂ���K�v���Ȃ��B
				const Square to = (mt == N00_Capture || mt == N03_CapturePlusPro ? toBB.GetFirstOneFromI9() : toBB.PopFirstOneFromI9());
				const bool toCanPromote = UtilSquare::CanPromote(mtEvent.m_us, UtilSquare::ToRank(to));
				if (toCanPromote) {
					(*mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2(mt, N02_Lance, from, to, mtEvent.m_pos);
					if (mt == N07_NonEvasion || all) {
						if (UtilSquare::IsBehind(mtEvent.m_us, Rank9, Rank1, UtilSquare::ToRank(to))) // 1�i�ڂ̕s���͏Ȃ�
							(*mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, N02_Lance, from, to, mtEvent.m_pos);
					}
					else if (mt != N01_NonCapture && mt != N04_NonCaptureMinusPro) { // ������Ȃ�3�i�ڂ̕s�����Ȃ�
						if (UtilSquare::IsBehind(mtEvent.m_us, Rank8, Rank2, UtilSquare::ToRank(to))) // 2�i�ڂ̕s�����Ȃ�
							(*mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, N02_Lance, from, to, mtEvent.m_pos);
					}
				}
				else
					(*mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, N02_Lance, from, to, mtEvent.m_pos);
			}
			// ����Ώۂ͕K����ȉ��Ȃ̂ŁAloop �͕s�v�B�œK���� do while �������Ȃ�Ɨǂ��B
		} while (!(mt == N00_Capture || mt == N03_CapturePlusPro) && toBB.Exists1Bit());
	}
	return mtEvent.m_moveStackList;
}


// �j�n�̏ꍇ
FORCE_INLINE MoveStack* GeneratePieceMoves_N03_Knight(
	MoveType mt,
	MoveTypeEvent& mtEvent,
	bool all,
	const Bitboard& target, const Square /*ksq*/
) {
	Bitboard fromBB = mtEvent.m_pos.GetBbOf(N03_Knight, mtEvent.m_us);
	while (fromBB.Exists1Bit()) {
		const Square from = fromBB.PopFirstOneFromI9();
		Bitboard toBB = PieceTypeArray::m_knight.GetAttacks2From(g_nullBitboard, mtEvent.m_us, from) & target;
		while (toBB.Exists1Bit()) {
			const Square to = toBB.PopFirstOneFromI9();
			const bool toCanPromote = UtilSquare::CanPromote(mtEvent.m_us, UtilSquare::ToRank(to));
			if (toCanPromote) {
				(*mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2(mt, N03_Knight, from, to, mtEvent.m_pos);
				if (UtilSquare::IsBehind(mtEvent.m_us, Rank8, Rank2, UtilSquare::ToRank(to))) // 1, 2�i�ڂ̕s���͏Ȃ�
					(*mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, N03_Knight, from, to, mtEvent.m_pos);
			}
			else
				(*mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, N03_Knight, from, to, mtEvent.m_pos);
		}
	}
	return mtEvent.m_moveStackList;
}


// ��̏ꍇ
FORCE_INLINE MoveStack* GeneratePieceMoves_N04_Silver(
	MoveType mt,
	MoveTypeEvent& mtEvent,
	bool all,
	const Bitboard& target, const Square /*ksq*/
) {
	Bitboard fromBB = mtEvent.m_pos.GetBbOf(N04_Silver, mtEvent.m_us);
	while (fromBB.Exists1Bit()) {
		const Square from = fromBB.PopFirstOneFromI9();
		const bool fromCanPromote = UtilSquare::CanPromote(mtEvent.m_us, UtilSquare::ToRank(from));
		Bitboard toBB = PieceTypeArray::m_silver.GetAttacks2From(g_nullBitboard, mtEvent.m_us, from) & target;
		while (toBB.Exists1Bit()) {
			const Square to = toBB.PopFirstOneFromI9();
			const bool toCanPromote = UtilSquare::CanPromote(mtEvent.m_us, UtilSquare::ToRank(to));
			if (fromCanPromote | toCanPromote)
				(*mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2(mt, N04_Silver, from, to, mtEvent.m_pos);
			(*mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, N04_Silver, from, to, mtEvent.m_pos);
		}
	}
	return mtEvent.m_moveStackList;
}


// �p�̓������H
FORCE_INLINE MoveStack* GeneratePieceMoves_N05_Bishop(
	MoveType mt,
	MoveTypeEvent& mtEvent,
	bool all,
	const Bitboard& target, const Square ksq
) {
	return BishopRookMovesGenerator::GenerateBishopOrRookMoves(
		mt,
		PieceType::N05_Bishop,
		mtEvent,
		all,
		target,
		ksq
	);
}


// ��Ԃ̓������H
FORCE_INLINE MoveStack* GeneratePieceMoves_N06_Rook(
	MoveType mt,
	MoveTypeEvent& mtEvent,
	bool all,
	const Bitboard& target, const Square ksq
) {
	return BishopRookMovesGenerator::GenerateBishopOrRookMoves(
		mt,
		PieceType::N06_Rook,
		mtEvent,
		all,		
		target,
		ksq
		);
}


// �ʂ̏ꍇ
// �K���Տ�� 1 ���������邱�Ƃ�O��ɂ��邱�ƂŁAwhile ���[�v�� 1 �������č��������Ă���B
FORCE_INLINE MoveStack* GeneratePieceMoves_N08_King(
	MoveType mt,
	MoveTypeEvent& mtEvent,
	bool all,
	const Bitboard& target, const Square /*ksq*/
) {
	const Square from = mtEvent.m_pos.GetKingSquare(mtEvent.m_us);
	Bitboard toBB = PieceTypeArray::m_king.GetAttacks2From(g_nullBitboard, mtEvent.m_us, from) & target;
	while (toBB.Exists1Bit()) {
		const Square to = toBB.PopFirstOneFromI9();
		(*mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, N08_King, from, to, mtEvent.m_pos);
	}
	return mtEvent.m_moveStackList;
}

