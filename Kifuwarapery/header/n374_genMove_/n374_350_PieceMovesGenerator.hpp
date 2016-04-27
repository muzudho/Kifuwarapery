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
#include "../n350_pieceTyp/n350_040_ptEvent.hpp"
#include "../n350_pieceTyp/n350_500_ptPrograms.hpp"
#include "../n372_piece___/n372_070_pieceAbstract.hpp"
#include "../n372_piece___/n372_500_pieceArray.hpp"
#include "n374_040_pieceMoveEvent.hpp"
#include "n374_250_bishopRookMovesGenerator.hpp"


class PieceMovesGenerator {
public:


	// ���̏ꍇ
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N01_Pawn(
		MoveStack* moveStackList,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
	) {
		// Txxx �͐��A���̏����z�������ϐ��B�����͐��ɍ��킹�Ă���B
		const Rank TRank6 = (ptEvent.m_us == Black ? Rank6 : Rank4);
		const Bitboard TRank789BB = g_inFrontMaskBb.GetInFrontMask(ptEvent.m_us, TRank6);
		const SquareDelta TDeltaS = (ptEvent.m_us == Black ? DeltaS : DeltaN);

		Bitboard toBB = Bitboard::PawnAttack(ptEvent.m_pos.GetBbOf20(N01_Pawn, ptEvent.m_us), ptEvent.m_us) & target;

		// ����
		if (ptEvent.m_mt != N04_NonCaptureMinusPro) {
			Bitboard toOn789BB = toBB & TRank789BB;
			if (toOn789BB.Exists1Bit()) {

				toBB.AndEqualNot(TRank789BB);

				Square to;

				FOREACH_BB(toOn789BB, to, {
					const Square from = to + TDeltaS;

					moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt(ptEvent.m_mt, N01_Pawn, from, to, ptEvent.m_pos);
					MakePromoteMove::APPEND_PROMOTE_FLAG(moveStackList->m_move);//, ptEvent.m_mt, N01_Pawn
					moveStackList++;

					if (ptEvent.m_mt == N07_NonEvasion || ptEvent.m_all) {
						const Rank TRank9 = (ptEvent.m_us == Black ? Rank9 : Rank1);
						if (ConvSquare::TO_RANK10(to) != TRank9) {
							moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt(ptEvent.m_mt, N01_Pawn, from, to, ptEvent.m_pos);
							moveStackList++;
						}
					}
				});
			}
		}
		else {
			assert(!(ptEvent.m_target & TRank789BB).Exists1Bit());
		}

		// �c��(�s��)
		// toBB �� 8~4 �i�ڂ܂ŁB
		Square to;
		FOREACH_BB(toBB, to, {
			const Square from = to + TDeltaS;
			moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt(ptEvent.m_mt, N01_Pawn, from, to, ptEvent.m_pos);
			moveStackList++;
		});

		return moveStackList;
	}//���Z�q�̃I�[�o�[���[�h�̒�`�H


	 // ���Ԃ̏ꍇ
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N02_Lance(
		MoveStack* moveStackList,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
	) {
		Bitboard fromBB = ptEvent.m_pos.GetBbOf20(N02_Lance, ptEvent.m_us);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			const PieceTypeEvent ptEvent1(ptEvent.m_pos.GetOccupiedBB(), ptEvent.m_us, from);
			Bitboard toBB = PiecetypePrograms::m_LANCE.GetAttacks2From(ptEvent1) & target;
			do {
				if (toBB.Exists1Bit()) {
					// ����Ώۂ͕K����ȉ��Ȃ̂ŁAtoBB �̃r�b�g�� 0 �ɂ���K�v���Ȃ��B
					const Square to = (ptEvent.m_mt == N00_Capture || ptEvent.m_mt == N03_CapturePlusPro ? toBB.GetFirstOneFromI9() : toBB.PopFirstOneFromI9());
					const bool toCanPromote = ConvSquare::CAN_PROMOTE10(ptEvent.m_us, ConvSquare::TO_RANK10(to));
					if (toCanPromote) {

						moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt(ptEvent.m_mt, N02_Lance, from, to, ptEvent.m_pos);
						MakePromoteMove::APPEND_PROMOTE_FLAG(moveStackList->m_move);//, ptEvent.m_mt, N02_Lance
						moveStackList++;

						if (ptEvent.m_mt == N07_NonEvasion || ptEvent.m_all) {
							if (ConvSquare::IS_BEHIND10(ptEvent.m_us, Rank9, Rank1, ConvSquare::TO_RANK10(to))) // 1�i�ڂ̕s���͏Ȃ�
							{
								moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt(ptEvent.m_mt, N02_Lance, from, to, ptEvent.m_pos);
								moveStackList++;
							}
						}
						else if (ptEvent.m_mt != N01_NonCapture && ptEvent.m_mt != N04_NonCaptureMinusPro) { // ������Ȃ�3�i�ڂ̕s�����Ȃ�
							if (ConvSquare::IS_BEHIND10(ptEvent.m_us, Rank8, Rank2, ConvSquare::TO_RANK10(to))) // 2�i�ڂ̕s�����Ȃ�
							{
								moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt(ptEvent.m_mt, N02_Lance, from, to, ptEvent.m_pos);
								moveStackList++;
							}
						}
					}
					else
					{
						moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt(ptEvent.m_mt, N02_Lance, from, to, ptEvent.m_pos);
						moveStackList++;
					}
				}
				// ����Ώۂ͕K����ȉ��Ȃ̂ŁAloop �͕s�v�B�œK���� do while �������Ȃ�Ɨǂ��B
			} while (!(ptEvent.m_mt == N00_Capture || ptEvent.m_mt == N03_CapturePlusPro) && toBB.Exists1Bit());
		}
		return moveStackList;
	}


	// �j�n�̏ꍇ
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N03_Knight(
		MoveStack* moveStackList,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
	) {
		Bitboard fromBB = ptEvent.m_pos.GetBbOf20(N03_Knight, ptEvent.m_us);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			const PieceTypeEvent ptEvent1(g_nullBitboard, ptEvent.m_us, from);
			Bitboard toBB = PiecetypePrograms::m_KNIGHT.GetAttacks2From(ptEvent1) & target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				const bool toCanPromote = ConvSquare::CAN_PROMOTE10(ptEvent.m_us, ConvSquare::TO_RANK10(to));
				if (toCanPromote) {

					moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt(ptEvent.m_mt, N03_Knight, from, to, ptEvent.m_pos);
					MakePromoteMove::APPEND_PROMOTE_FLAG(moveStackList->m_move);//, ptEvent.m_mt, N03_Knight
					moveStackList++;

					if (ConvSquare::IS_BEHIND10(ptEvent.m_us, Rank8, Rank2, ConvSquare::TO_RANK10(to))) // 1, 2�i�ڂ̕s���͏Ȃ�
					{
						moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt(ptEvent.m_mt, N03_Knight, from, to, ptEvent.m_pos);
						moveStackList++;
					}
				}
				else
				{
					moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt(ptEvent.m_mt, N03_Knight, from, to, ptEvent.m_pos);
					moveStackList++;
				}
			}
		}
		return moveStackList;
	}


	// ��̏ꍇ
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N04_Silver(
		MoveStack* moveStackList,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
	) {
		Bitboard fromBB = ptEvent.m_pos.GetBbOf20(N04_Silver, ptEvent.m_us);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			const bool fromCanPromote = ConvSquare::CAN_PROMOTE10(ptEvent.m_us, ConvSquare::TO_RANK10(from));
			const PieceTypeEvent ptEvent1(g_nullBitboard, ptEvent.m_us, from);
			Bitboard toBB = PiecetypePrograms::m_SILVER.GetAttacks2From(ptEvent1) & target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				const bool toCanPromote = ConvSquare::CAN_PROMOTE10(ptEvent.m_us, ConvSquare::TO_RANK10(to));
				if (fromCanPromote | toCanPromote) {
					moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt(ptEvent.m_mt, N04_Silver, from, to, ptEvent.m_pos);
					MakePromoteMove::APPEND_PROMOTE_FLAG(moveStackList->m_move);//, ptEvent.m_mt, N04_Silver
					moveStackList++;
				}

				moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt(ptEvent.m_mt, N04_Silver, from, to, ptEvent.m_pos);
				moveStackList++;
			}
		}
		return moveStackList;
	}


	// �p�̓������H
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N05_Bishop(
		MoveStack* moveStackList,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
	) {
		return BishopRookMovesGenerator::GenerateBishopOrRookMoves(moveStackList, PieceType::N05_Bishop, ptEvent, target);
	}


	// ��Ԃ̓������H
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N06_Rook(
		MoveStack* moveStackList,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
	) {
		return BishopRookMovesGenerator::GenerateBishopOrRookMoves(moveStackList, PieceType::N06_Rook, ptEvent, target);
	}


	// ��, ������A�n�A���̎w���萶��
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N16_GoldHorseDragon(
		MoveStack* moveStackList,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
	) {
		// ���A�����A�n�A����bitboard���܂Ƃ߂Ĉ����B
		Bitboard fromBB = (ptEvent.m_pos.GetGoldsBB() | ptEvent.m_pos.GetBbOf20(N13_Horse, N14_Dragon)) &
			ptEvent.m_pos.GetBbOf10(ptEvent.m_us);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			// from �ɂ����̎�ނ𔻕�
			const PieceType pt = ConvPiece::TO_PIECE_TYPE10(ptEvent.m_pos.GetPiece(from));
			Bitboard toBB = UtilAttack::GetAttacksFrom(pt, ptEvent.m_us, from, ptEvent.m_pos.GetOccupiedBB()) & target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt(ptEvent.m_mt, pt, from, to, ptEvent.m_pos);
				moveStackList++;
			}
		}
		return moveStackList;
	}


	// �ʂ̏ꍇ
	// �K���Տ�� 1 ���������邱�Ƃ�O��ɂ��邱�ƂŁAwhile ���[�v�� 1 �������č��������Ă���B
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N08_King(
		MoveStack* moveStackList,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
	) {
		const Square from = ptEvent.m_pos.GetKingSquare(ptEvent.m_us);
		const PieceTypeEvent ptEvent1(g_nullBitboard, ptEvent.m_us, from);
		Bitboard toBB = PiecetypePrograms::m_KING.GetAttacks2From(ptEvent1) & target;
		while (toBB.Exists1Bit()) {
			const Square to = toBB.PopFirstOneFromI9();
			moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt(ptEvent.m_mt, N08_King, from, to, ptEvent.m_pos);
			moveStackList++;
		}
		return moveStackList;
	}

};

