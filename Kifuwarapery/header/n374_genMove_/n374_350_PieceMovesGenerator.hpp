#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n113_piece___/n113_150_piece.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n350_pieceTyp/n350_500_ptArray.hpp"
#include "../n372_genMoveP/n372_070_pieceAbstract.hpp"
#include "n374_050_generateMoves.hpp"
#include "../n376_genMoveP/n376_500_pieceArray.hpp"


// 金, 成り金、馬、竜の指し手生成
template <MoveType MT, PieceType PT, Color us, bool ALL>
struct GeneratePieceMoves {
	FORCE_INLINE MoveStack* operator () (
		MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square /*ksq*/
		) {
		static_assert(PT == N16_GoldHorseDragon, "");
		// 金、成金、馬、竜のbitboardをまとめて扱う。
		Bitboard fromBB = (pos.GetGoldsBB() | pos.GetBbOf(N13_Horse, N14_Dragon)) & pos.GetBbOf(us);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			// from にある駒の種類を判別
			const PieceType pt = UtilPiece::ToPieceType(pos.GetPiece(from));
			Bitboard toBB = UtilAttack::GetAttacksFrom(pt, us, from, pos.GetOccupiedBB()) & target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(pt, from, to, pos);
			}
		}
		return moveStackList;
	}
};
// 歩の場合
template <MoveType MT, Color US, bool ALL>
struct GeneratePieceMoves<MT, N01_Pawn, US, ALL> {
	FORCE_INLINE MoveStack* operator () (
		MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square /*ksq*/
		) {
		// Txxx は先手、後手の情報を吸収した変数。数字は先手に合わせている。
		const Rank TRank6 = (US == Black ? Rank6 : Rank4);
		const Bitboard TRank789BB = g_inFrontMaskBb.GetInFrontMask<US, TRank6>();
		const SquareDelta TDeltaS = (US == Black ? DeltaS : DeltaN);

		Bitboard toBB = Bitboard:: PawnAttack(pos.GetBbOf(N01_Pawn, US), US) & target;

		// 成り
		if (MT != NonCaptureMinusPro) {
			Bitboard toOn789BB = toBB & TRank789BB;
			if (toOn789BB.Exists1Bit()) {

				toBB.AndEqualNot(TRank789BB);

				Square to;

				FOREACH_BB(toOn789BB, to, {
					const Square from = to + TDeltaS;
				(*moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2<MT>(N01_Pawn, from, to, pos);
				if (MT == NonEvasion || ALL) {
					const Rank TRank9 = (US == Black ? Rank9 : Rank1);
					if (UtilSquare::ToRank(to) != TRank9) {
						(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(N01_Pawn, from, to, pos);
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
		(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(N01_Pawn, from, to, pos);
		});

		return moveStackList;
	}//演算子のオーバーロードの定義？
};//struct

  // 香車の場合
template <MoveType MT, Color US, bool ALL>
struct GeneratePieceMoves<MT, N02_Lance, US, ALL> {
	FORCE_INLINE MoveStack* operator () (
		MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square /*ksq*/
		) {
		Bitboard fromBB = pos.GetBbOf(N02_Lance, US);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			Bitboard toBB = PieceTypeArray::m_lance.GetAttacks2From(pos.GetOccupiedBB(), US, from) & target;
			do {
				if (toBB.Exists1Bit()) {
					// 駒取り対象は必ず一つ以下なので、toBB のビットを 0 にする必要がない。
					const Square to = (MT == Capture || MT == CapturePlusPro ? toBB.GetFirstOneFromI9() : toBB.PopFirstOneFromI9());
					const bool toCanPromote = UtilSquare::CanPromote(US, UtilSquare::ToRank(to));
					if (toCanPromote) {
						(*moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2<MT>(N02_Lance, from, to, pos);
						if (MT == NonEvasion || ALL) {
							if (UtilSquare::IsBehind<US, Rank9, Rank1>(UtilSquare::ToRank(to))) // 1段目の不成は省く
								(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(N02_Lance, from, to, pos);
						}
						else if (MT != NonCapture && MT != NonCaptureMinusPro) { // 駒を取らない3段目の不成を省く
							if (UtilSquare::IsBehind<US, Rank8, Rank2>(UtilSquare::ToRank(to))) // 2段目の不成を省く
								(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(N02_Lance, from, to, pos);
						}
					}
					else
						(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(N02_Lance, from, to, pos);
				}
				// 駒取り対象は必ず一つ以下なので、loop は不要。最適化で do while が無くなると良い。
			} while (!(MT == Capture || MT == CapturePlusPro) && toBB.Exists1Bit());
		}
		return moveStackList;
	}
};
// 桂馬の場合
template <MoveType MT, Color US, bool ALL>
struct GeneratePieceMoves<MT, N03_Knight, US, ALL> {
	FORCE_INLINE MoveStack* operator () (
		MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square /*ksq*/
		) {
		Bitboard fromBB = pos.GetBbOf(N03_Knight, US);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			Bitboard toBB = PieceTypeArray::m_knight.GetAttacks2From(g_nullBitboard, US, from) & target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				const bool toCanPromote = UtilSquare::CanPromote(US, UtilSquare::ToRank(to));
				if (toCanPromote) {
					(*moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2<MT>(N03_Knight, from, to, pos);
					if (UtilSquare::IsBehind<US, Rank8, Rank2>(UtilSquare::ToRank(to))) // 1, 2段目の不成は省く
						(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(N03_Knight, from, to, pos);
				}
				else
					(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(N03_Knight, from, to, pos);
			}
		}
		return moveStackList;
	}
};
// 銀の場合
template <MoveType MT, Color US, bool ALL>
struct GeneratePieceMoves<MT, N04_Silver, US, ALL> {
	FORCE_INLINE MoveStack* operator () (
		MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square /*ksq*/
		) {
		Bitboard fromBB = pos.GetBbOf(N04_Silver, US);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			const bool fromCanPromote = UtilSquare::CanPromote(US, UtilSquare::ToRank(from));
			Bitboard toBB = PieceTypeArray::m_silver.GetAttacks2From(g_nullBitboard, US, from) & target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				const bool toCanPromote = UtilSquare::CanPromote(US, UtilSquare::ToRank(to));
				if (fromCanPromote | toCanPromote)
					(*moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2<MT>(N04_Silver, from, to, pos);
				(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(N04_Silver, from, to, pos);
			}
		}
		return moveStackList;
	}
};
// 角の動き☆？
template <MoveType MT, Color US, bool ALL>
struct GeneratePieceMoves<MT, N05_Bishop, US, ALL> {
	FORCE_INLINE MoveStack* operator () (
		MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square ksq
		) {
		return BishopRookMovesGenerator::GenerateBishopOrRookMoves<MT, N05_Bishop, US, ALL>(moveStackList, pos, target, ksq);
	}
};
// 飛車の動き☆？
template <MoveType MT, Color US, bool ALL>
struct GeneratePieceMoves<MT, N06_Rook, US, ALL> {
	FORCE_INLINE MoveStack* operator () (
		MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square ksq
		) {
		return BishopRookMovesGenerator::GenerateBishopOrRookMoves<MT, N06_Rook, US, ALL>(moveStackList, pos, target, ksq);
	}
};
// 玉の場合
// 必ず盤上に 1 枚だけあることを前提にすることで、while ループを 1 つ無くして高速化している。
template <MoveType MT, Color US, bool ALL>
struct GeneratePieceMoves<MT, N08_King, US, ALL> {
	FORCE_INLINE MoveStack* operator () (
		MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square /*ksq*/
		) {
		const Square from = pos.GetKingSquare(US);
		Bitboard toBB = PieceTypeArray::m_king.GetAttacks2From(g_nullBitboard, US, from) & target;
		while (toBB.Exists1Bit()) {
			const Square to = toBB.PopFirstOneFromI9();
			(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(N08_King, from, to, pos);
		}
		return moveStackList;
	}
};

