#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n110_square__/n110_200_rank.hpp"
#include "../n110_square__/n110_250_squareDelta.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n113_piece___/n113_150_piece.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_300_moveType.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_640_utilAttack.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n350_pieceTyp/n350_500_ptArray.hpp"
#include "../n372_piece___/n372_070_pieceAbstract.hpp"
#include "../n372_piece___/n372_500_pieceArray.hpp"
#include "n374_050_generateMoves.hpp"
#include "n374_250_bishopRookMovesGenerator.hpp"


// 金, 成り金、馬、竜の指し手生成
struct GeneratePieceMoves_pt {
	FORCE_INLINE MoveStack* operator () (
		PieceType pt1,
		MoveType mt,
		Color us,
		bool all,
		MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square /*ksq*/
	) {
		assert(pt1 == N16_GoldHorseDragon, "");
		// 金、成金、馬、竜のbitboardをまとめて扱う。
		Bitboard fromBB = (pos.GetGoldsBB() | pos.GetBbOf(N13_Horse, N14_Dragon)) & pos.GetBbOf(us);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			// from にある駒の種類を判別
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

// 歩の場合
struct GeneratePieceMoves_N01_Pawn {
	FORCE_INLINE MoveStack* operator () (
		MoveType mt,
		Color us,
		bool all,
		MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square /*ksq*/
	) {
		// Txxx は先手、後手の情報を吸収した変数。数字は先手に合わせている。
		const Rank TRank6 = (us == Black ? Rank6 : Rank4);
		const Bitboard TRank789BB = g_inFrontMaskBb.GetInFrontMask(us, TRank6);
		const SquareDelta TDeltaS = (us == Black ? DeltaS : DeltaN);

		Bitboard toBB = Bitboard:: PawnAttack(pos.GetBbOf(N01_Pawn, us), us) & target;

		// 成り
		if (mt != N04_NonCaptureMinusPro) {
			Bitboard toOn789BB = toBB & TRank789BB;
			if (toOn789BB.Exists1Bit()) {

				toBB.AndEqualNot(TRank789BB);

				Square to;

				FOREACH_BB(toOn789BB, to, {
					const Square from = to + TDeltaS;
				(*moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2(mt, N01_Pawn, from, to, pos);
				if (mt == N07_NonEvasion || all) {
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

		// 残り(不成)
		// toBB は 8~4 段目まで。
		Square to;
		FOREACH_BB(toBB, to, {
			const Square from = to + TDeltaS;
		(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, N01_Pawn, from, to, pos);
		});

		return moveStackList;
	}//演算子のオーバーロードの定義？
};//struct

 // 香車の場合
struct GeneratePieceMoves_N02_Lance {
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
					// 駒取り対象は必ず一つ以下なので、toBB のビットを 0 にする必要がない。
					const Square to = (mt == N00_Capture || mt == N03_CapturePlusPro ? toBB.GetFirstOneFromI9() : toBB.PopFirstOneFromI9());
					const bool toCanPromote = UtilSquare::CanPromote(us, UtilSquare::ToRank(to));
					if (toCanPromote) {
						(*moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2(mt, N02_Lance, from, to, pos);
						if (mt == N07_NonEvasion || all) {
							if (UtilSquare::IsBehind(us, Rank9, Rank1, UtilSquare::ToRank(to))) // 1段目の不成は省く
								(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, N02_Lance, from, to, pos);
						}
						else if (mt != N01_NonCapture && mt != N04_NonCaptureMinusPro) { // 駒を取らない3段目の不成を省く
							if (UtilSquare::IsBehind(us, Rank8, Rank2, UtilSquare::ToRank(to))) // 2段目の不成を省く
								(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, N02_Lance, from, to, pos);
						}
					}
					else
						(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, N02_Lance, from, to, pos);
				}
				// 駒取り対象は必ず一つ以下なので、loop は不要。最適化で do while が無くなると良い。
			} while (!(mt == N00_Capture || mt == N03_CapturePlusPro) && toBB.Exists1Bit());
		}
		return moveStackList;
	}
};

// 桂馬の場合
struct GeneratePieceMoves_N03_Knight {
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
					if (UtilSquare::IsBehind(us, Rank8, Rank2, UtilSquare::ToRank(to))) // 1, 2段目の不成は省く
						(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, N03_Knight, from, to, pos);
				}
				else
					(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, N03_Knight, from, to, pos);
			}
		}
		return moveStackList;
	}
};

// 銀の場合
struct GeneratePieceMoves_N04_Silver {
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

// 角の動き☆？
struct GeneratePieceMoves_N05_Bishop {
	FORCE_INLINE MoveStack* operator () (
		MoveType mt,
		Color us,
		bool all,
		MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square ksq
	) {
		return BishopRookMovesGenerator::GenerateBishopOrRookMoves(mt, PieceType::N05_Bishop, us, all, moveStackList, pos, target, ksq);
	}
};

// 飛車の動き☆？
struct GeneratePieceMoves_N06_Rook {
	FORCE_INLINE MoveStack* operator () (
		MoveType mt,
		Color us,
		bool all,
		MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square ksq
	) {
		return BishopRookMovesGenerator::GenerateBishopOrRookMoves(mt, PieceType::N06_Rook, us,  all, moveStackList, pos, target, ksq);
	}
};

// 玉の場合
// 必ず盤上に 1 枚だけあることを前提にすることで、while ループを 1 つ無くして高速化している。
struct GeneratePieceMoves_N08_King {
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

