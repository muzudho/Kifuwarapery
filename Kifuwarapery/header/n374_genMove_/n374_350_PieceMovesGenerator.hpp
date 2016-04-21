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
#include "n374_040_pieceMoveEvent.hpp"
#include "n374_250_bishopRookMovesGenerator.hpp"


class PieceMovesGenerator {
public:

	// 金, 成り金、馬、竜の指し手生成
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N16_GoldHorseDragon(
		MoveStack* moveStackList,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
	) {
		// 金、成金、馬、竜のbitboardをまとめて扱う。
		Bitboard fromBB = (ptEvent.m_pos.GetGoldsBB() | ptEvent.m_pos.GetBbOf(N13_Horse, N14_Dragon)) & ptEvent.m_pos.GetBbOf(ptEvent.m_us);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			// from にある駒の種類を判別
			const PieceType pt = UtilPiece::ToPieceType(ptEvent.m_pos.GetPiece(from));
			Bitboard toBB = UtilAttack::GetAttacksFrom(pt, ptEvent.m_us, from, ptEvent.m_pos.GetOccupiedBB()) & target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				(*moveStackList++).m_move = g_makePromoteMove.GetSelectedMakeMove(ptEvent.m_mt, PromoteMode::NonPromote, pt, from, to, ptEvent.m_pos);
			}
		}
		return moveStackList;
	}


	// 歩の場合
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N01_Pawn(
		MoveStack* moveStackList,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
	) {
		// Txxx は先手、後手の情報を吸収した変数。数字は先手に合わせている。
		const Rank TRank6 = (ptEvent.m_us == Black ? Rank6 : Rank4);
		const Bitboard TRank789BB = g_inFrontMaskBb.GetInFrontMask(ptEvent.m_us, TRank6);
		const SquareDelta TDeltaS = (ptEvent.m_us == Black ? DeltaS : DeltaN);

		Bitboard toBB = Bitboard::PawnAttack(ptEvent.m_pos.GetBbOf(N01_Pawn, ptEvent.m_us), ptEvent.m_us) & target;

		// 成り
		if (ptEvent.m_mt != N04_NonCaptureMinusPro) {
			Bitboard toOn789BB = toBB & TRank789BB;
			if (toOn789BB.Exists1Bit()) {

				toBB.AndEqualNot(TRank789BB);

				Square to;

				FOREACH_BB(toOn789BB, to, {
					const Square from = to + TDeltaS;
				(*moveStackList++).m_move = g_makePromoteMove.GetSelectedMakeMove(ptEvent.m_mt, PromoteMode::Promote, N01_Pawn, from, to, ptEvent.m_pos);
				if (ptEvent.m_mt == N07_NonEvasion || ptEvent.m_all) {
					const Rank TRank9 = (ptEvent.m_us == Black ? Rank9 : Rank1);
					if (UtilSquare::ToRank(to) != TRank9) {
						(*moveStackList++).m_move = g_makePromoteMove.GetSelectedMakeMove(ptEvent.m_mt, PromoteMode::NonPromote, N01_Pawn, from, to, ptEvent.m_pos);
					}
				}
				});
			}
		}
		else {
			assert(!(ptEvent.m_target & TRank789BB).Exists1Bit());
		}

		// 残り(不成)
		// toBB は 8~4 段目まで。
		Square to;
		FOREACH_BB(toBB, to, {
			const Square from = to + TDeltaS;
		(*moveStackList++).m_move = g_makePromoteMove.GetSelectedMakeMove(ptEvent.m_mt, PromoteMode::NonPromote, N01_Pawn, from, to, ptEvent.m_pos);
		});

		return moveStackList;
	}//演算子のオーバーロードの定義？


	 // 香車の場合
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N02_Lance(
		MoveStack* moveStackList,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
	) {
		Bitboard fromBB = ptEvent.m_pos.GetBbOf(N02_Lance, ptEvent.m_us);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			Bitboard toBB = PieceTypeArray::m_lance.GetAttacks2From(ptEvent.m_pos.GetOccupiedBB(), ptEvent.m_us, from) & target;
			do {
				if (toBB.Exists1Bit()) {
					// 駒取り対象は必ず一つ以下なので、toBB のビットを 0 にする必要がない。
					const Square to = (ptEvent.m_mt == N00_Capture || ptEvent.m_mt == N03_CapturePlusPro ? toBB.GetFirstOneFromI9() : toBB.PopFirstOneFromI9());
					const bool toCanPromote = UtilSquare::CanPromote(ptEvent.m_us, UtilSquare::ToRank(to));
					if (toCanPromote) {
						(*moveStackList++).m_move = g_makePromoteMove.GetSelectedMakeMove(ptEvent.m_mt, PromoteMode::Promote, N02_Lance, from, to, ptEvent.m_pos);
						if (ptEvent.m_mt == N07_NonEvasion || ptEvent.m_all) {
							if (UtilSquare::IsBehind(ptEvent.m_us, Rank9, Rank1, UtilSquare::ToRank(to))) // 1段目の不成は省く
								(*moveStackList++).m_move = g_makePromoteMove.GetSelectedMakeMove(ptEvent.m_mt, PromoteMode::NonPromote, N02_Lance, from, to, ptEvent.m_pos);
						}
						else if (ptEvent.m_mt != N01_NonCapture && ptEvent.m_mt != N04_NonCaptureMinusPro) { // 駒を取らない3段目の不成を省く
							if (UtilSquare::IsBehind(ptEvent.m_us, Rank8, Rank2, UtilSquare::ToRank(to))) // 2段目の不成を省く
								(*moveStackList++).m_move = g_makePromoteMove.GetSelectedMakeMove(ptEvent.m_mt, PromoteMode::NonPromote, N02_Lance, from, to, ptEvent.m_pos);
						}
					}
					else
						(*moveStackList++).m_move = g_makePromoteMove.GetSelectedMakeMove(ptEvent.m_mt, PromoteMode::NonPromote, N02_Lance, from, to, ptEvent.m_pos);
				}
				// 駒取り対象は必ず一つ以下なので、loop は不要。最適化で do while が無くなると良い。
			} while (!(ptEvent.m_mt == N00_Capture || ptEvent.m_mt == N03_CapturePlusPro) && toBB.Exists1Bit());
		}
		return moveStackList;
	}


	// 桂馬の場合
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N03_Knight(
		MoveStack* moveStackList,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
	) {
		Bitboard fromBB = ptEvent.m_pos.GetBbOf(N03_Knight, ptEvent.m_us);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			Bitboard toBB = PieceTypeArray::m_knight.GetAttacks2From(g_nullBitboard, ptEvent.m_us, from) & target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				const bool toCanPromote = UtilSquare::CanPromote(ptEvent.m_us, UtilSquare::ToRank(to));
				if (toCanPromote) {
					(*moveStackList++).m_move = g_makePromoteMove.GetSelectedMakeMove(ptEvent.m_mt, PromoteMode::Promote, N03_Knight, from, to, ptEvent.m_pos);
					if (UtilSquare::IsBehind(ptEvent.m_us, Rank8, Rank2, UtilSquare::ToRank(to))) // 1, 2段目の不成は省く
						(*moveStackList++).m_move = g_makePromoteMove.GetSelectedMakeMove(ptEvent.m_mt, PromoteMode::NonPromote, N03_Knight, from, to, ptEvent.m_pos);
				}
				else
					(*moveStackList++).m_move = g_makePromoteMove.GetSelectedMakeMove(ptEvent.m_mt, PromoteMode::NonPromote, N03_Knight, from, to, ptEvent.m_pos);
			}
		}
		return moveStackList;
	}


	// 銀の場合
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N04_Silver(
		MoveStack* moveStackList,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
	) {
		Bitboard fromBB = ptEvent.m_pos.GetBbOf(N04_Silver, ptEvent.m_us);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			const bool fromCanPromote = UtilSquare::CanPromote(ptEvent.m_us, UtilSquare::ToRank(from));
			Bitboard toBB = PieceTypeArray::m_silver.GetAttacks2From(g_nullBitboard, ptEvent.m_us, from) & target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				const bool toCanPromote = UtilSquare::CanPromote(ptEvent.m_us, UtilSquare::ToRank(to));
				if (fromCanPromote | toCanPromote)
					(*moveStackList++).m_move = g_makePromoteMove.GetSelectedMakeMove(ptEvent.m_mt, PromoteMode::Promote, N04_Silver, from, to, ptEvent.m_pos);
				(*moveStackList++).m_move = g_makePromoteMove.GetSelectedMakeMove(ptEvent.m_mt, PromoteMode::NonPromote, N04_Silver, from, to, ptEvent.m_pos);
			}
		}
		return moveStackList;
	}


	// 角の動き☆？
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N05_Bishop(
		MoveStack* moveStackList,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
	) {
		return BishopRookMovesGenerator::GenerateBishopOrRookMoves(moveStackList, PieceType::N05_Bishop, ptEvent, target);
	}


	// 飛車の動き☆？
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N06_Rook(
		MoveStack* moveStackList,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
	) {
		return BishopRookMovesGenerator::GenerateBishopOrRookMoves(moveStackList, PieceType::N06_Rook, ptEvent, target);
	}


	// 玉の場合
	// 必ず盤上に 1 枚だけあることを前提にすることで、while ループを 1 つ無くして高速化している。
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N08_King(
		MoveStack* moveStackList,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
	) {
		const Square from = ptEvent.m_pos.GetKingSquare(ptEvent.m_us);
		Bitboard toBB = PieceTypeArray::m_king.GetAttacks2From(g_nullBitboard, ptEvent.m_us, from) & target;
		while (toBB.Exists1Bit()) {
			const Square to = toBB.PopFirstOneFromI9();
			(*moveStackList++).m_move = g_makePromoteMove.GetSelectedMakeMove(ptEvent.m_mt, PromoteMode::NonPromote, N08_King, from, to, ptEvent.m_pos);
		}
		return moveStackList;
	}

};

