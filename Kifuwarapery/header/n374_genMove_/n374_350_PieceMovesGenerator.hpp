#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n113_piece___/n113_150_piece.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n350_pieceTyp/n350_500_ptArray.hpp"
#include "../n372_genMoveP/n372_070_pieceAbstract.hpp"
#include "n374_050_generateMoves.hpp"
#include "n374_250_bishopRookMovesGenerator.hpp"
#include "../n376_genMoveP/n376_500_pieceArray.hpp"


// ��, ������A�n�A���̎w���萶��
template <PieceType PT>
struct GeneratePieceMoves {
	FORCE_INLINE MoveStack* operator () (
		MoveType mt,
		Color us,
		bool all,
		MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square /*ksq*/
	) {
		static_assert(PT == N16_GoldHorseDragon, "");
		// ���A�����A�n�A����bitboard���܂Ƃ߂Ĉ����B
		Bitboard fromBB = (pos.GetGoldsBB() | pos.GetBbOf(N13_Horse, N14_Dragon)) & pos.GetBbOf(us);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			// from �ɂ����̎�ނ𔻕�
			const PieceType pt = UtilPiece::ToPieceType(pos.GetPiece(from));
			Bitboard toBB = UtilAttack::GetAttacksFrom(pt, us, from, pos.GetOccupiedBB()) & target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, pt, from, to, pos);
			}
		}
		return moveStackList;
	}
};

// ���̏ꍇ
template <>
struct GeneratePieceMoves<N01_Pawn> {
	FORCE_INLINE MoveStack* operator () (
		MoveType mt,
		Color us,
		bool all,
		MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square /*ksq*/
	) {
		// Txxx �͐��A���̏����z�������ϐ��B�����͐��ɍ��킹�Ă���B
		const Rank TRank6 = (us == Black ? Rank6 : Rank4);
		const Bitboard TRank789BB = g_inFrontMaskBb.GetInFrontMask(us, TRank6);
		const SquareDelta TDeltaS = (us == Black ? DeltaS : DeltaN);

		Bitboard toBB = Bitboard:: PawnAttack(pos.GetBbOf(N01_Pawn, us), us) & target;

		// ����
		if (mt != NonCaptureMinusPro) {
			Bitboard toOn789BB = toBB & TRank789BB;
			if (toOn789BB.Exists1Bit()) {

				toBB.AndEqualNot(TRank789BB);

				Square to;

				FOREACH_BB(toOn789BB, to, {
					const Square from = to + TDeltaS;
				(*moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2(mt, N01_Pawn, from, to, pos);
				if (mt == NonEvasion || all) {
					const Rank TRank9 = (us == Black ? Rank9 : Rank1);
					if (UtilSquare::ToRank(to) != TRank9) {
						(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, N01_Pawn, from, to, pos);
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
		(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, N01_Pawn, from, to, pos);
		});

		return moveStackList;
	}//���Z�q�̃I�[�o�[���[�h�̒�`�H
};//struct

 // ���Ԃ̏ꍇ
template <>
struct GeneratePieceMoves<N02_Lance> {
	FORCE_INLINE MoveStack* operator () (
		MoveType mt,
		Color us,
		bool all,
		MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square /*ksq*/
	) {
		Bitboard fromBB = pos.GetBbOf(N02_Lance, us);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			Bitboard toBB = PieceTypeArray::m_lance.GetAttacks2From(pos.GetOccupiedBB(), us, from) & target;
			do {
				if (toBB.Exists1Bit()) {
					// ����Ώۂ͕K����ȉ��Ȃ̂ŁAtoBB �̃r�b�g�� 0 �ɂ���K�v���Ȃ��B
					const Square to = (mt == Capture || mt == CapturePlusPro ? toBB.GetFirstOneFromI9() : toBB.PopFirstOneFromI9());
					const bool toCanPromote = UtilSquare::CanPromote(us, UtilSquare::ToRank(to));
					if (toCanPromote) {
						(*moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2(mt, N02_Lance, from, to, pos);
						if (mt == NonEvasion || all) {
							if (UtilSquare::IsBehind(us, Rank9, Rank1, UtilSquare::ToRank(to))) // 1�i�ڂ̕s���͏Ȃ�
								(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, N02_Lance, from, to, pos);
						}
						else if (mt != NonCapture && mt != NonCaptureMinusPro) { // ������Ȃ�3�i�ڂ̕s�����Ȃ�
							if (UtilSquare::IsBehind(us, Rank8, Rank2, UtilSquare::ToRank(to))) // 2�i�ڂ̕s�����Ȃ�
								(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, N02_Lance, from, to, pos);
						}
					}
					else
						(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, N02_Lance, from, to, pos);
				}
				// ����Ώۂ͕K����ȉ��Ȃ̂ŁAloop �͕s�v�B�œK���� do while �������Ȃ�Ɨǂ��B
			} while (!(mt == Capture || mt == CapturePlusPro) && toBB.Exists1Bit());
		}
		return moveStackList;
	}
};

// �j�n�̏ꍇ
template <>
struct GeneratePieceMoves<N03_Knight> {
	FORCE_INLINE MoveStack* operator () (
		MoveType mt,
		Color us,
		bool all,
		MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square /*ksq*/
	) {
		Bitboard fromBB = pos.GetBbOf(N03_Knight, us);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			Bitboard toBB = PieceTypeArray::m_knight.GetAttacks2From(g_nullBitboard, us, from) & target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				const bool toCanPromote = UtilSquare::CanPromote(us, UtilSquare::ToRank(to));
				if (toCanPromote) {
					(*moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2(mt, N03_Knight, from, to, pos);
					if (UtilSquare::IsBehind(us, Rank8, Rank2, UtilSquare::ToRank(to))) // 1, 2�i�ڂ̕s���͏Ȃ�
						(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, N03_Knight, from, to, pos);
				}
				else
					(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, N03_Knight, from, to, pos);
			}
		}
		return moveStackList;
	}
};

// ��̏ꍇ
template <>
struct GeneratePieceMoves<N04_Silver> {
	FORCE_INLINE MoveStack* operator () (
		MoveType mt,
		Color us,
		bool all,
		MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square /*ksq*/
	) {
		Bitboard fromBB = pos.GetBbOf(N04_Silver, us);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			const bool fromCanPromote = UtilSquare::CanPromote(us, UtilSquare::ToRank(from));
			Bitboard toBB = PieceTypeArray::m_silver.GetAttacks2From(g_nullBitboard, us, from) & target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				const bool toCanPromote = UtilSquare::CanPromote(us, UtilSquare::ToRank(to));
				if (fromCanPromote | toCanPromote)
					(*moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2(mt, N04_Silver, from, to, pos);
				(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, N04_Silver, from, to, pos);
			}
		}
		return moveStackList;
	}
};

// �p�̓������H
template <>
struct GeneratePieceMoves<N05_Bishop> {
	FORCE_INLINE MoveStack* operator () (
		MoveType mt,
		Color us,
		bool all,
		MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square ksq
	) {
		return BishopRookMovesGenerator::GenerateBishopOrRookMoves(mt, PieceType::N05_Bishop, us, all, moveStackList, pos, target, ksq);
	}
};

// ��Ԃ̓������H
template <>
struct GeneratePieceMoves<N06_Rook> {
	FORCE_INLINE MoveStack* operator () (
		MoveType mt,
		Color us,
		bool all,
		MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square ksq
	) {
		return BishopRookMovesGenerator::GenerateBishopOrRookMoves(mt, PieceType::N06_Rook, us,  all, moveStackList, pos, target, ksq);
	}
};

// �ʂ̏ꍇ
// �K���Տ�� 1 ���������邱�Ƃ�O��ɂ��邱�ƂŁAwhile ���[�v�� 1 �������č��������Ă���B
template <>
struct GeneratePieceMoves<N08_King> {
	FORCE_INLINE MoveStack* operator () (
		MoveType mt,
		Color us,
		bool all,
		MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square /*ksq*/
	) {
		const Square from = pos.GetKingSquare(us);
		Bitboard toBB = PieceTypeArray::m_king.GetAttacks2From(g_nullBitboard, us, from) & target;
		while (toBB.Exists1Bit()) {
			const Square to = toBB.PopFirstOneFromI9();
			(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, N08_King, from, to, pos);
		}
		return moveStackList;
	}
};

