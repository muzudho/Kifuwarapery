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
#include "n374_045_pmEvent.hpp"
#include "n374_250_bishopRookMovesGenerator.hpp"


class PieceMovesGenerator {
public:


	// 金, 成り金、馬、竜の指し手生成
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N16_GoldHorseDragon(
		PieceMovesEvent pmEvent
	) {
		// 金、成金、馬、竜のbitboardをまとめて扱う。
		Bitboard fromBB = (pmEvent.m_mtEvent.m_pos.GetGoldsBB() |
			pmEvent.m_mtEvent.m_pos.GetBbOf(N13_Horse, N14_Dragon))
			&
			pmEvent.m_mtEvent.m_pos.GetBbOf(pmEvent.m_mtEvent.m_us);

		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			// from にある駒の種類を判別
			const PieceType pt = UtilPiece::ToPieceType(pmEvent.m_mtEvent.m_pos.GetPiece(from));
			Bitboard toBB = UtilAttack::GetAttacksFrom(
				pt, pmEvent.m_mtEvent.m_us, from, pmEvent.m_mtEvent.m_pos.GetOccupiedBB()) & pmEvent.m_target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				(*pmEvent.m_mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(
					pmEvent.m_mt, pt, from, to, pmEvent.m_mtEvent.m_pos);
			}
		}
		return pmEvent.m_mtEvent.m_moveStackList;
	}


	// 歩の場合
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N01_Pawn(
		PieceMovesEvent pmEvent
	) {
		// Txxx は先手、後手の情報を吸収した変数。数字は先手に合わせている。
		const SquareDelta TDeltaS = (pmEvent.m_mtEvent.m_us == Black ? DeltaS : DeltaN);

		Bitboard toBB = Bitboard::PawnAttack(pmEvent.m_mtEvent.m_pos.GetBbOf(N01_Pawn, pmEvent.m_mtEvent.m_us), pmEvent.m_mtEvent.m_us) &
			pmEvent.m_target;

		// 成り
		if (pmEvent.m_mt != N04_NonCaptureMinusPro) {
			Bitboard toOn789BB = toBB & pmEvent.m_mtEvent.m_tRank789BB;
			if (toOn789BB.Exists1Bit()) {

				toBB.AndEqualNot(pmEvent.m_mtEvent.m_tRank789BB);

				Square to;

				FOREACH_BB(toOn789BB, to, {
					const Square from = to + TDeltaS;
				(*pmEvent.m_mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2(
					pmEvent.m_mt, N01_Pawn, from, to, pmEvent.m_mtEvent.m_pos);

				if (pmEvent.m_mt == N07_NonEvasion || pmEvent.m_all) {
					const Rank TRank9 = (pmEvent.m_mtEvent.m_us == Black ? Rank9 : Rank1);
					if (UtilSquare::ToRank(to) != TRank9) {
						(*pmEvent.m_mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(
							pmEvent.m_mt, N01_Pawn, from, to, pmEvent.m_mtEvent.m_pos);
					}
				}
				});
			}
		}
		else {
			assert(!(pmEvent.m_target & pmEvent.m_mtEvent.m_tRank789BB).Exists1Bit());
		}

		// 残り(不成)
		// toBB は 8~4 段目まで。
		Square to;
		FOREACH_BB(toBB, to, {
			const Square from = to + TDeltaS;
		(*pmEvent.m_mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(
			pmEvent.m_mt, N01_Pawn, from, to, pmEvent.m_mtEvent.m_pos);
		});

		return pmEvent.m_mtEvent.m_moveStackList;
	}//演算子のオーバーロードの定義？


	 // 香車の場合
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N02_Lance(
		PieceMovesEvent pmEvent
	) {
		Bitboard fromBB = pmEvent.m_mtEvent.m_pos.GetBbOf(N02_Lance, pmEvent.m_mtEvent.m_us);

		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			Bitboard toBB = PieceTypeArray::m_lance.GetAttacks2From(pmEvent.m_mtEvent.m_pos.GetOccupiedBB(), pmEvent.m_mtEvent.m_us, from) &
				pmEvent.m_target;
			do {
				if (toBB.Exists1Bit()) {
					// 駒取り対象は必ず一つ以下なので、toBB のビットを 0 にする必要がない。
					const Square to = (pmEvent.m_mt == N00_Capture ||
						pmEvent.m_mt == N03_CapturePlusPro ? toBB.GetFirstOneFromI9() : toBB.PopFirstOneFromI9());
					const bool toCanPromote = UtilSquare::CanPromote(pmEvent.m_mtEvent.m_us, UtilSquare::ToRank(to));
					if (toCanPromote) {
						(*pmEvent.m_mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2(
							pmEvent.m_mt, N02_Lance, from, to, pmEvent.m_mtEvent.m_pos);
						if (pmEvent.m_mt == N07_NonEvasion || pmEvent.m_all) {
							if (UtilSquare::IsBehind(pmEvent.m_mtEvent.m_us, Rank9, Rank1, UtilSquare::ToRank(to))) // 1段目の不成は省く
								(*pmEvent.m_mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(
									pmEvent.m_mt, N02_Lance, from, to, pmEvent.m_mtEvent.m_pos);
						}
						else if (pmEvent.m_mt != N01_NonCapture && pmEvent.m_mt != N04_NonCaptureMinusPro) { // 駒を取らない3段目の不成を省く
							if (UtilSquare::IsBehind(pmEvent.m_mtEvent.m_us, Rank8, Rank2, UtilSquare::ToRank(to))) // 2段目の不成を省く
								(*pmEvent.m_mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(
									pmEvent.m_mt, N02_Lance, from, to, pmEvent.m_mtEvent.m_pos);
						}
					}
					else
						(*pmEvent.m_mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(
							pmEvent.m_mt, N02_Lance, from, to, pmEvent.m_mtEvent.m_pos);
				}
				// 駒取り対象は必ず一つ以下なので、loop は不要。最適化で do while が無くなると良い。
			} while (!(pmEvent.m_mt == N00_Capture || pmEvent.m_mt == N03_CapturePlusPro) && toBB.Exists1Bit());
		}
		return pmEvent.m_mtEvent.m_moveStackList;
	}


	// 桂馬の場合
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N03_Knight(
		PieceMovesEvent pmEvent
	) {
		Bitboard fromBB = pmEvent.m_mtEvent.m_pos.GetBbOf(N03_Knight, pmEvent.m_mtEvent.m_us);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			Bitboard toBB = PieceTypeArray::m_knight.GetAttacks2From(g_nullBitboard, pmEvent.m_mtEvent.m_us, from) &
				pmEvent.m_target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				const bool toCanPromote = UtilSquare::CanPromote(pmEvent.m_mtEvent.m_us, UtilSquare::ToRank(to));
				if (toCanPromote) {
					(*pmEvent.m_mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2(
						pmEvent.m_mt, N03_Knight, from, to, pmEvent.m_mtEvent.m_pos);
					if (UtilSquare::IsBehind(pmEvent.m_mtEvent.m_us, Rank8, Rank2, UtilSquare::ToRank(to))) // 1, 2段目の不成は省く
						(*pmEvent.m_mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(
							pmEvent.m_mt, N03_Knight, from, to, pmEvent.m_mtEvent.m_pos);
				}
				else
					(*pmEvent.m_mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(
						pmEvent.m_mt, N03_Knight, from, to, pmEvent.m_mtEvent.m_pos);
			}
		}
		return pmEvent.m_mtEvent.m_moveStackList;
	}


	// 銀の場合
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N04_Silver(
		PieceMovesEvent pmEvent
	) {
		Bitboard fromBB = pmEvent.m_mtEvent.m_pos.GetBbOf(N04_Silver, pmEvent.m_mtEvent.m_us);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			const bool fromCanPromote = UtilSquare::CanPromote(pmEvent.m_mtEvent.m_us, UtilSquare::ToRank(from));
			Bitboard toBB = PieceTypeArray::m_silver.GetAttacks2From(g_nullBitboard, pmEvent.m_mtEvent.m_us, from) &
				pmEvent.m_target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				const bool toCanPromote = UtilSquare::CanPromote(pmEvent.m_mtEvent.m_us, UtilSquare::ToRank(to));
				if (fromCanPromote | toCanPromote)
					(*pmEvent.m_mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2(
						pmEvent.m_mt, N04_Silver, from, to, pmEvent.m_mtEvent.m_pos);
				(*pmEvent.m_mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(
					pmEvent.m_mt, N04_Silver, from, to, pmEvent.m_mtEvent.m_pos);
			}
		}
		return pmEvent.m_mtEvent.m_moveStackList;
	}


	// 角の動き☆？
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N05_Bishop(
		PieceMovesEvent pmEvent
	) {
		return BishopRookMovesGenerator::GenerateBishopOrRookMoves(
			PieceType::N05_Bishop,
			pmEvent
		);
	}


	// 飛車の動き☆？
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N06_Rook(
		PieceMovesEvent pmEvent
	) {
		return BishopRookMovesGenerator::GenerateBishopOrRookMoves(
			PieceType::N06_Rook,
			pmEvent
		);
	}


	// 玉の場合
	// 必ず盤上に 1 枚だけあることを前提にすることで、while ループを 1 つ無くして高速化している。
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N08_King(
		PieceMovesEvent pmEvent
	) {
		const Square from = pmEvent.m_mtEvent.m_pos.GetKingSquare(pmEvent.m_mtEvent.m_us);
		Bitboard toBB = PieceTypeArray::m_king.GetAttacks2From(g_nullBitboard, pmEvent.m_mtEvent.m_us, from) &
			pmEvent.m_target;
		while (toBB.Exists1Bit()) {
			const Square to = toBB.PopFirstOneFromI9();
			(*pmEvent.m_mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(
				pmEvent.m_mt, N08_King, from, to, pmEvent.m_mtEvent.m_pos);
		}
		return pmEvent.m_mtEvent.m_moveStackList;
	}

};
