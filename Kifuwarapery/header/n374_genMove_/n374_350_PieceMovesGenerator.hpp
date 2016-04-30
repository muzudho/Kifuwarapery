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


	// 歩の場合
	template <const Color US>
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N01_Pawn(
		MoveStack* moveStackList,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
	) {
		static_assert(US==Color::Black | US==Color::White,"");

		// Txxx は先手、後手の情報を吸収した変数。数字は先手に合わせている。
		const Rank TRank6 = (US == Black ? Rank6 : Rank4);
		const Rank TRank9 = (US == Black ? Rank9 : Rank1);
		const Bitboard TRank789BB = g_inFrontMaskBb.GetInFrontMask<US>(TRank6);
		const SquareDelta TDeltaS = (US == Black ? DeltaS : DeltaN);

		Bitboard toBB = Bitboard::PawnAttack<US>(ptEvent.m_pos.GetBbOf20<US>(N01_Pawn)) & target;

		// 成り
		if (ptEvent.m_mt != N04_NonCaptureMinusPro) {
			Bitboard toOn789BB = toBB & TRank789BB;
			if (toOn789BB.Exists1Bit()) {

				toBB.AndEqualNot(TRank789BB);

				Square to;

				FOREACH_BB(toOn789BB, to, {
					const Square from = to + TDeltaS;

					moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt2(ptEvent.m_mt,g_PTPAWN_ONBOARD_AS_MOVE,from, to, ptEvent.m_pos);
					MakePromoteMove::APPEND_PROMOTE_FLAG(moveStackList->m_move);
					moveStackList++;

					if (ptEvent.m_mt == N07_NonEvasion || ptEvent.m_all) {
						if (ConvSquare::TO_RANK10(to) != TRank9) {
							moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt2(ptEvent.m_mt,g_PTPAWN_ONBOARD_AS_MOVE,from, to, ptEvent.m_pos);
							moveStackList++;
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
			moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt2(ptEvent.m_mt, g_PTPAWN_ONBOARD_AS_MOVE,	from, to, ptEvent.m_pos);
			moveStackList++;
		});

		return moveStackList;
	}//演算子のオーバーロードの定義？


	 // 香車の場合
	template <const Color US>
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N02_Lance(
		MoveStack* moveStackList,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
	) {
		Bitboard fromBB = ptEvent.m_pos.GetBbOf20<US>(N02_Lance);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			const PieceTypeEvent ptEvent1(ptEvent.m_pos.GetOccupiedBB(), US, from);
			Bitboard toBB = PiecetypePrograms::m_LANCE.GetAttacks2From(ptEvent1) & target;
			do {
				if (toBB.Exists1Bit()) {
					// 駒取り対象は必ず一つ以下なので、toBB のビットを 0 にする必要がない。
					const Square to = (ptEvent.m_mt == N00_Capture || ptEvent.m_mt == N03_CapturePlusPro ? toBB.GetFirstOneFromI9() : toBB.PopFirstOneFromI9());
					const bool toCanPromote = ConvSquare::CAN_PROMOTE10<US>(ConvSquare::TO_RANK10(to));
					if (toCanPromote) {

						moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt2(ptEvent.m_mt,g_PTLANCE_ONBOARD_AS_MOVE,	from, to, ptEvent.m_pos);
						MakePromoteMove::APPEND_PROMOTE_FLAG(moveStackList->m_move);
						moveStackList++;

						if (ptEvent.m_mt == N07_NonEvasion || ptEvent.m_all) {
							if (ConvSquare::IS_BEHIND10<US>(Rank9, Rank1, ConvSquare::TO_RANK10(to))) // 1段目の不成は省く
							{
								moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt2(ptEvent.m_mt,g_PTLANCE_ONBOARD_AS_MOVE,	from, to, ptEvent.m_pos);
								moveStackList++;
							}
						}
						else if (ptEvent.m_mt != N01_NonCapture && ptEvent.m_mt != N04_NonCaptureMinusPro) { // 駒を取らない3段目の不成を省く
							if (ConvSquare::IS_BEHIND10<US>(Rank8, Rank2, ConvSquare::TO_RANK10(to))) // 2段目の不成を省く
							{
								moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt2(ptEvent.m_mt,g_PTLANCE_ONBOARD_AS_MOVE,	from, to, ptEvent.m_pos);
								moveStackList++;
							}
						}
					}
					else
					{
						moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt2(ptEvent.m_mt,g_PTLANCE_ONBOARD_AS_MOVE,	from, to, ptEvent.m_pos);
						moveStackList++;
					}
				}
				// 駒取り対象は必ず一つ以下なので、loop は不要。最適化で do while が無くなると良い。
			} while (!(ptEvent.m_mt == N00_Capture || ptEvent.m_mt == N03_CapturePlusPro) && toBB.Exists1Bit());
		}
		return moveStackList;
	}


	// 桂馬の場合
	template <const Color US>
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N03_Knight(
		MoveStack* moveStackList,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
	) {
		Bitboard fromBB = ptEvent.m_pos.GetBbOf20<US>(N03_Knight);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			const PieceTypeEvent ptEvent1(g_nullBitboard, US, from);
			Bitboard toBB = PiecetypePrograms::m_KNIGHT.GetAttacks2From(ptEvent1) & target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				const bool toCanPromote = ConvSquare::CAN_PROMOTE10<US>(ConvSquare::TO_RANK10(to));
				if (toCanPromote) {

					moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt2(ptEvent.m_mt, g_PTKNIGHT_ONBOARD_AS_MOVE, from, to, ptEvent.m_pos);
					MakePromoteMove::APPEND_PROMOTE_FLAG(moveStackList->m_move);
					moveStackList++;

					if (ConvSquare::IS_BEHIND10<US>(Rank8, Rank2, ConvSquare::TO_RANK10(to))) // 1, 2段目の不成は省く
					{
						moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt2(ptEvent.m_mt, g_PTKNIGHT_ONBOARD_AS_MOVE, from, to, ptEvent.m_pos);
						moveStackList++;
					}
				}
				else
				{
					moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt2(ptEvent.m_mt, g_PTKNIGHT_ONBOARD_AS_MOVE, from, to, ptEvent.m_pos);
					moveStackList++;
				}
			}
		}
		return moveStackList;
	}


	// 銀の場合
	template <const Color US>
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N04_Silver(
		MoveStack* moveStackList,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
	) {
		Bitboard fromBB = ptEvent.m_pos.GetBbOf20<US>(N04_Silver);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			const bool fromCanPromote = ConvSquare::CAN_PROMOTE10<US>(ConvSquare::TO_RANK10(from));
			const PieceTypeEvent ptEvent1(g_nullBitboard, US, from);
			Bitboard toBB = PiecetypePrograms::m_SILVER.GetAttacks2From(ptEvent1) & target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				const bool toCanPromote = ConvSquare::CAN_PROMOTE10<US>(ConvSquare::TO_RANK10(to));
				if (fromCanPromote | toCanPromote) {
					moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt2(ptEvent.m_mt,g_PTSILVER_ONBOARD_AS_MOVE,	from, to, ptEvent.m_pos);
					MakePromoteMove::APPEND_PROMOTE_FLAG(moveStackList->m_move);
					moveStackList++;
				}

				moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt2(ptEvent.m_mt,g_PTSILVER_ONBOARD_AS_MOVE,from, to, ptEvent.m_pos);
				moveStackList++;
			}
		}
		return moveStackList;
	}


	// 角の動き☆？
	template <const Color US>
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N05_Bishop(
		MoveStack* moveStackList,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
	) {
		return BishopRookMovesGenerator::GenerateBishopOrRookMoves<true,US>(moveStackList, g_PTBISHOP_ONBOARD_AS_MOVE, ptEvent, target);
	}


	// 飛車の動き☆？
	template <const Color US>
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N06_Rook(
		MoveStack* moveStackList,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
	) {
		return BishopRookMovesGenerator::GenerateBishopOrRookMoves<false,US>(moveStackList, g_PTROOK_ONBOARD_AS_MOVE, ptEvent, target);
	}


	// 金, 成り金、馬、竜の指し手生成
	template <const Color US>
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N16_GoldHorseDragon(
		MoveStack* moveStackList,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
	) {
		// 金、成金、馬、竜のbitboardをまとめて扱う。
		Bitboard fromBB = (ptEvent.m_pos.GetGoldsBB() | ptEvent.m_pos.GetBbOf20(N13_Horse, N14_Dragon)) &
			ptEvent.m_pos.GetBbOf10<US>();
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			// from にある駒の種類を判別
			const PieceType pt = ConvPiece::TO_PIECE_TYPE10(ptEvent.m_pos.GetPiece(from));
			Bitboard toBB = UtilAttack::GetAttacksFrom<US>(pt, from, ptEvent.m_pos.GetOccupiedBB()) & target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_goldHorseDragon(ptEvent.m_mt,
					pt,// 金、成り金、馬、竜のいずれかだぜ☆（＾ｑ＾）
					from, to, ptEvent.m_pos);
				moveStackList++;
			}
		}
		return moveStackList;
	}


	// 玉の場合
	// 必ず盤上に 1 枚だけあることを前提にすることで、while ループを 1 つ無くして高速化している。
	template <const Color US>
	static FORCE_INLINE MoveStack* GeneratePieceMoves_N08_King(
		MoveStack* moveStackList,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
	) {
		const Square from = ptEvent.m_pos.GetKingSquare<US>();
		const PieceTypeEvent ptEvent1(g_nullBitboard, US, from);
		Bitboard toBB = PiecetypePrograms::m_KING.GetAttacks2From(ptEvent1) & target;
		while (toBB.Exists1Bit()) {
			const Square to = toBB.PopFirstOneFromI9();
			moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt2(ptEvent.m_mt,g_PTKING_ONBOARD_AS_MOVE,from, to, ptEvent.m_pos);
			moveStackList++;
		}
		return moveStackList;
	}

};

