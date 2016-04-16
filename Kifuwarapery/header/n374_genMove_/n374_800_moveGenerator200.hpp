#pragma once


#include "../n113_piece___/n113_150_piece.hpp"
#include "../n350_pieceTyp/n350_500_ptArray.hpp"
#include "../n372_genMoveP/n372_070_pieceAbstract.hpp"
#include "../n376_genMoveP/n376_500_pieceArray.hpp"
#include "n374_500_generateMoves.hpp"


class MoveGenerator200 {
private:
	// 角, 飛車の場合
	template <MoveType MT, PieceType PT, Color US, bool ALL>
	static FORCE_INLINE MoveStack* generateBishopOrRookMoves(
		MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square /*ksq*/
		)
	{
		Bitboard fromBB = pos.GetBbOf(PT, US);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			const bool fromCanPromote = UtilSquare::CanPromote(US, UtilSquare::ToRank(from));
			Bitboard toBB = g_ptArray[PT]->GetAttacks2From(pos.GetOccupiedBB(), US, from) & target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				const bool toCanPromote = UtilSquare::CanPromote(US, UtilSquare::ToRank(to));
				if (fromCanPromote | toCanPromote) {
					(*moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2<MT>(PT, from, to, pos);
					if (MT == NonEvasion || ALL)
						(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(PT, from, to, pos);
				}
				else // 角、飛車は成れるなら成り、不成は生成しない。
					(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(PT, from, to, pos);
			}
		}
		return moveStackList;
	}



	// 駒打ちの場合
	// 歩以外の持ち駒は、loop の前に持ち駒の種類の数によって switch で展開している。
	// ループの展開はコードが膨れ上がる事によるキャッシュヒット率の低下と、演算回数のバランスを取って決める必要がある。
	// NPSに影響が出ないならシンプルにした方が良さそう。
	//template <Color US>
	static MoveStack* generateDropMoves(
		Color us,
		MoveStack* pMovestack,
		const Position& pos,
		const Bitboard& target
	) {
		const Hand hand = pos.GetHand(us);

		// まず、歩に対して指し手を生成
		if (hand.Exists<HPawn>()) {

			Bitboard toBB = target;
			// 一段目には打てない

			const Rank tRank9 = (us == Black ? Rank9 : Rank1);
			toBB.AndEqualNot(g_rankMaskBb.GetRankMask(tRank9));

			// 二歩の回避
			Bitboard pawnsBB = pos.GetBbOf(N01_Pawn, us);
			Square pawnsSquare;
			foreachBB(pawnsBB, pawnsSquare, [&](const int part) {
				toBB.SetP(part, toBB.GetP(part) & ~g_fileMaskBb.GetSquareFileMask(pawnsSquare).GetP(part));
			});

			// 打ち歩詰めの回避
			const Rank tRank1 = (us == Black ? Rank1 : Rank9);
			const SquareDelta tDeltaS = (us == Black ? DeltaS : DeltaN);

			const Square ksq = pos.GetKingSquare(UtilColor::OppositeColor(us));
			// 相手玉が九段目なら、歩で王手出来ないので、打ち歩詰めを調べる必要はない。
			if (UtilSquare::ToRank(ksq) != tRank1) {
				const Square pawnDropCheckSquare = ksq + tDeltaS;
				assert(UtilSquare::ContainsOf(pawnDropCheckSquare));
				if (g_setMaskBb.IsSet(&toBB, pawnDropCheckSquare) && pos.GetPiece(pawnDropCheckSquare) == N00_Empty) {
					if (!pos.IsPawnDropCheckMate(us, pawnDropCheckSquare)) {
						// ここで clearBit だけして MakeMove しないことも出来る。
						// 指し手が生成される順番が変わり、王手が先に生成されるが、後で問題にならないか?
						(*pMovestack++).m_move = UtilMove::MakeDropMove(N01_Pawn, pawnDropCheckSquare);
					}
					g_setMaskBb.XorBit(&toBB, pawnDropCheckSquare);
				}
			}

			Square to;
			FOREACH_BB(toBB, to, {
				(*pMovestack++).m_move = UtilMove::MakeDropMove(N01_Pawn, to);
			});
		}

		// 歩 以外の駒を持っているか
		if (hand.ExceptPawnExists()) {
			PieceType haveHandArr[6]; // 歩以外の持ち駒。vector 使いたいけど、速度を求めるので使わない。
			int haveHandNum = 0; // 持ち駒の駒の種類の数

								 // 桂馬、香車、それ以外の順番で格納する。(駒を打てる位置が限定的な順)
			if (hand.Exists<HKnight>()) { haveHandArr[haveHandNum++] = PieceType::N03_Knight; }
			const int noKnightIdx = haveHandNum; // 桂馬を除く駒でループするときのループの初期値
			if (hand.Exists<HLance >()) { haveHandArr[haveHandNum++] = PieceType::N02_Lance; }
			const int noKnightLanceIdx = haveHandNum; // 桂馬, 香車を除く駒でループするときのループの初期値
			if (hand.Exists<HSilver>()) { haveHandArr[haveHandNum++] = PieceType::N04_Silver; }
			if (hand.Exists<HGold  >()) { haveHandArr[haveHandNum++] = PieceType::N07_Gold; }
			if (hand.Exists<HBishop>()) { haveHandArr[haveHandNum++] = PieceType::N05_Bishop; }
			if (hand.Exists<HRook  >()) { haveHandArr[haveHandNum++] = PieceType::N06_Rook; }

			const Rank tRank8 = (us == Black ? Rank8 : Rank2);
			const Rank tRank9 = (us == Black ? Rank9 : Rank1);
			const Bitboard TRank8BB = g_rankMaskBb.GetRankMask(tRank8);
			const Bitboard TRank9BB = g_rankMaskBb.GetRankMask(tRank9);


			/* FIXME: 駒の打ち込みが見えないぜ☆！（＾ｑ＾）
			//
			// 桂馬、香車 以外の持ち駒があれば、
			// 一段目に対して、桂馬、香車以外の指し手を生成。
			// FIXME: 配列の範囲チェックしてないぜ☆（＾ｑ＾）
			DropMakerArray::m_dropMakerArray[haveHandNum - noKnightLanceIdx].MakeDropMovesToRank9ExceptNL(target, TRank9BB, pMovestack, haveHandArr, noKnightLanceIdx);

			// 桂馬以外の持ち駒があれば、
			// 二段目に対して、桂馬以外の指し手を生成。
			DropMakerArray::m_dropMakerArray[haveHandNum - noKnightIdx].MakeDropMovesToRank8ExceptN(target, TRank8BB, pMovestack, haveHandArr, noKnightIdx);

			// 一、二段目以外に対して、全ての持ち駒の指し手を生成。
			Bitboard toBB = target & ~(TRank8BB | TRank9BB);
			DropMakerArray::m_dropMakerArray[haveHandNum].MakeDropMovesToRank1234567( toBB, pMovestack, haveHandArr);
			//*/

			//* OK
			//Square iTo;// ループカウンタを兼ねる。
			// 桂馬、香車 以外の持ち駒があれば、
			// 一段目に対して、桂馬、香車以外の指し手を生成。
			switch (haveHandNum - noKnightLanceIdx) {//templateの中なので改造しにくいぜ☆（＾ｑ＾）
			case 0:
			{
				break; // 桂馬、香車 以外の持ち駒がない。
			}
			case 1:
			{
				Bitboard toBB = target & TRank9BB;
				do {
					while (toBB.GetP(0)) {
						Square iTo = toBB.PopFirstOneRightFromI9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
						};
					}
					while (toBB.GetP(1)) {
						Square iTo = toBB.PopFirstOneLeftFromB9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
						};
					}
				} while (false);
				break;
			}
			case 2:
			{
				Bitboard toBB = target & TRank9BB;
				do {
					while (toBB.GetP(0)) {
						Square iTo = toBB.PopFirstOneRightFromI9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
						};
					}
					while (toBB.GetP(1)) {
						Square iTo = toBB.PopFirstOneLeftFromB9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
						};
					}
				} while (false);
				break;
			}
			case 3:
			{
				Bitboard toBB = target & TRank9BB;
				do {
					while (toBB.GetP(0)) {
						Square iTo = toBB.PopFirstOneRightFromI9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 2], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
						};
					}
					while (toBB.GetP(1)) {
						Square iTo = toBB.PopFirstOneLeftFromB9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 2], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
						};
					}
				} while (false);
				break;
			}
			case 4:
			{
				Bitboard toBB = target & TRank9BB;
				do {
					while (toBB.GetP(0)) {
						Square iTo = toBB.PopFirstOneRightFromI9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 3], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 2], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
						};
					}
					while (toBB.GetP(1)) {
						Square iTo = toBB.PopFirstOneLeftFromB9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 3], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 2], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
						};
					}
				} while (false);
				break;
			}
			default:
			{
				UNREACHABLE;
			}
			}

			// 桂馬以外の持ち駒があれば、
			// 二段目に対して、桂馬以外の指し手を生成。
			switch (haveHandNum - noKnightIdx) {//templateの中なので改造しにくいぜ☆（＾ｑ＾）
			case 0:
			{
				break; // 桂馬 以外の持ち駒がない。
			}
			case 1:
			{
				Bitboard toBB = target & TRank8BB;
				do {
					while (toBB.GetP(0)) {
						Square iTo = toBB.PopFirstOneRightFromI9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
						};
					}
					while (toBB.GetP(1)) {
						Square iTo = toBB.PopFirstOneLeftFromB9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
						};
					}
				} while (false);
				break;
			}
			case 2:
			{
				Bitboard toBB = target & TRank8BB;
				do {
					while (toBB.GetP(0)) {
						Square iTo = toBB.PopFirstOneRightFromI9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
						};
					}
					while (toBB.GetP(1)) {
						Square iTo = toBB.PopFirstOneLeftFromB9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
						};
					}
				} while (false);
				break;
			}
			case 3:
			{
				Bitboard toBB = target & TRank8BB;
				do {
					while (toBB.GetP(0)) {
						Square iTo = toBB.PopFirstOneRightFromI9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
						};
					}
					while (toBB.GetP(1)) {
						Square iTo = toBB.PopFirstOneLeftFromB9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
						};
					}
				} while (false);
				break;
			}
			case 4:
			{
				Bitboard toBB = target & TRank8BB;
				do {
					while (toBB.GetP(0)) {
						Square iTo = toBB.PopFirstOneRightFromI9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 3], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
						};
					}
					while (toBB.GetP(1)) {
						Square iTo = toBB.PopFirstOneLeftFromB9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 3], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
						};
					}
				} while (false);
				break;
			}
			case 5:
			{
				Bitboard toBB = target & TRank8BB;
				do {
					while (toBB.GetP(0)) {
						Square iTo = toBB.PopFirstOneRightFromI9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 4], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 3], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
						};
					}
					while (toBB.GetP(1)) {
						Square iTo = toBB.PopFirstOneLeftFromB9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 4], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 3], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
						};
					}
				} while (false);
				break;
			}
			default:
			{
				UNREACHABLE;
			}
			}

			// 一、二段目以外に対して、全ての持ち駒の指し手を生成。
			//Bitboard toBB = target & ~(TRank8BB | TRank9BB);
			switch (haveHandNum) {//templateの中なので改造しにくいぜ☆（＾ｑ＾）
			case 0:
			{
				assert(false); break; // 最適化の為のダミー
			}
			case 1:
			{
				Bitboard toBB = target & ~(TRank8BB | TRank9BB);
				do {
					while (toBB.GetP(0)) {
						Square iTo = toBB.PopFirstOneRightFromI9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
						};
					}
					while (toBB.GetP(1)) {
						Square iTo = toBB.PopFirstOneLeftFromB9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
						};
					}
				} while (false);
				break;
			}
			case 2:
			{
				Bitboard toBB = target & ~(TRank8BB | TRank9BB);
				do {
					while (toBB.GetP(0)) {
						Square iTo = toBB.PopFirstOneRightFromI9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
						};
					}
					while (toBB.GetP(1)) {
						Square iTo = toBB.PopFirstOneLeftFromB9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
						};
					}
				} while (false);
				break;
			}
			case 3:
			{
				Bitboard toBB = target & ~(TRank8BB | TRank9BB);
				do {
					while (toBB.GetP(0)) {
						Square iTo = toBB.PopFirstOneRightFromI9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
						};
					}
					while (toBB.GetP(1)) {
						Square iTo = toBB.PopFirstOneLeftFromB9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
						};
					}
				} while (false);
				break;
			}
			case 4:
			{
				Bitboard toBB = target & ~(TRank8BB | TRank9BB);
				do {
					while (toBB.GetP(0)) {
						Square iTo = toBB.PopFirstOneRightFromI9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
						};
					}
					while (toBB.GetP(1)) {
						Square iTo = toBB.PopFirstOneLeftFromB9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
						};
					}
				} while (false);
				break;
			}
			case 5:
			{
				Bitboard toBB = target & ~(TRank8BB | TRank9BB);
				do {
					while (toBB.GetP(0)) {
						Square iTo = toBB.PopFirstOneRightFromI9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[4], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
						};
					}
					while (toBB.GetP(1)) {
						Square iTo = toBB.PopFirstOneLeftFromB9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[4], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
						};
					}
				} while (false);
				break;
			}
			case 6:
			{
				Bitboard toBB = target & ~(TRank8BB | TRank9BB);
				do {
					while (toBB.GetP(0)) {
						Square iTo = toBB.PopFirstOneRightFromI9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[5], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[4], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
						};
					}
					while (toBB.GetP(1)) {
						Square iTo = toBB.PopFirstOneLeftFromB9();
						{
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[5], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[4], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
							pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
						};
					}
				} while (false);
				break;
			}
			default:
			{
				UNREACHABLE;
			}
			}
			//*/
		}

		return pMovestack;
	}


	// 金, 成り金、馬、竜の指し手生成
	template <MoveType MT, PieceType PT, Color US, bool ALL>
	struct GeneratePieceMoves {
		FORCE_INLINE MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square /*ksq*/
			) {
			static_assert(PT == N16_GoldHorseDragon, "");
			// 金、成金、馬、竜のbitboardをまとめて扱う。
			Bitboard fromBB = (pos.GetGoldsBB() | pos.GetBbOf(N13_Horse, N14_Dragon)) & pos.GetBbOf(US);
			while (fromBB.Exists1Bit()) {
				const Square from = fromBB.PopFirstOneFromI9();
				// from にある駒の種類を判別
				const PieceType pt = UtilPiece::ToPieceType(pos.GetPiece(from));
				Bitboard toBB = UtilAttack::GetAttacksFrom(pt, US, from, pos.GetOccupiedBB()) & target;
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

			Bitboard toBB = pos.GetBbOf(N01_Pawn, US).PawnAttack<US>() & target;

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
				Bitboard toBB = g_ptLance.GetAttacks2From(pos.GetOccupiedBB(), US, from) & target;
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
				Bitboard toBB = g_ptKnight.GetAttacks2From(g_nullBitboard, US, from) & target;
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
				Bitboard toBB = g_ptSilver.GetAttacks2From(g_nullBitboard, US, from) & target;
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
	template <MoveType MT, Color US, bool ALL>
	struct GeneratePieceMoves<MT, N05_Bishop, US, ALL> {
		FORCE_INLINE MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square ksq
			) {
			return generateBishopOrRookMoves<MT, N05_Bishop, US, ALL>(moveStackList, pos, target, ksq);
		}
	};
	template <MoveType MT, Color US, bool ALL>
	struct GeneratePieceMoves<MT, N06_Rook, US, ALL> {
		FORCE_INLINE MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square ksq
			) {
			return generateBishopOrRookMoves<MT, N06_Rook, US, ALL>(moveStackList, pos, target, ksq);
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
			Bitboard toBB = g_ptKing.GetAttacks2From(g_nullBitboard, US, from) & target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(N08_King, from, to, pos);
			}
			return moveStackList;
		}
	};

	// 指し手生成 functor
	// テンプレート引数が複数あり、部分特殊化したかったので、関数ではなく、struct にした。
	// ALL == true のとき、歩、飛、角の不成、香の2段目の不成、香の3段目の駒を取らない不成も生成する。
	template <MoveType MT, Color US, bool ALL = false>
	struct GenerateMoves {
		MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos
			) {
			static_assert(MT == Capture || MT == NonCapture || MT == CapturePlusPro || MT == NonCaptureMinusPro, "");
			// Txxx は先手、後手の情報を吸収した変数。数字は先手に合わせている。
			const Rank TRank6 = (US == Black ? Rank6 : Rank4);
			const Rank TRank7 = (US == Black ? Rank7 : Rank3);
			const Rank TRank8 = (US == Black ? Rank8 : Rank2);
			const Bitboard TRank789BB = g_inFrontMaskBb.GetInFrontMask<US, TRank6>();
			const Bitboard TRank1_6BB = g_inFrontMaskBb.GetInFrontMask<UtilColor::OppositeColor(US), TRank7>();
			const Bitboard TRank1_7BB = g_inFrontMaskBb.GetInFrontMask<UtilColor::OppositeColor(US), TRank8>();

			const Bitboard targetPawn =
				(MT == Capture) ? pos.GetBbOf(UtilColor::OppositeColor(US)) :
				(MT == NonCapture) ? pos.GetEmptyBB() :
				(MT == CapturePlusPro) ? pos.GetBbOf(UtilColor::OppositeColor(US)) | (pos.GetOccupiedBB().NotThisAnd(TRank789BB)) :
				(MT == NonCaptureMinusPro) ? pos.GetOccupiedBB().NotThisAnd(TRank1_6BB) :
				Bitboard::CreateAllOneBB(); // error
			const Bitboard targetOther =
				(MT == Capture) ? pos.GetBbOf(UtilColor::OppositeColor(US)) :
				(MT == NonCapture) ? pos.GetEmptyBB() :
				(MT == CapturePlusPro) ? pos.GetBbOf(UtilColor::OppositeColor(US)) :
				(MT == NonCaptureMinusPro) ? pos.GetEmptyBB() :
				Bitboard::CreateAllOneBB(); // error
			const Square ksq = pos.GetKingSquare(UtilColor::OppositeColor(US));

			moveStackList = GeneratePieceMoves<MT, N01_Pawn, US, ALL>()(moveStackList, pos, targetPawn, ksq);
			moveStackList = GeneratePieceMoves<MT, N02_Lance, US, ALL>()(moveStackList, pos, targetOther, ksq);
			moveStackList = GeneratePieceMoves<MT, N03_Knight, US, ALL>()(moveStackList, pos, targetOther, ksq);
			moveStackList = GeneratePieceMoves<MT, N04_Silver, US, ALL>()(moveStackList, pos, targetOther, ksq);
			moveStackList = GeneratePieceMoves<MT, N05_Bishop, US, ALL>()(moveStackList, pos, targetOther, ksq);
			moveStackList = GeneratePieceMoves<MT, N06_Rook, US, ALL>()(moveStackList, pos, targetOther, ksq);
			moveStackList = GeneratePieceMoves<MT, N16_GoldHorseDragon, US, ALL>()(moveStackList, pos, targetOther, ksq);
			moveStackList = GeneratePieceMoves<MT, N08_King, US, ALL>()(moveStackList, pos, targetOther, ksq);

			return moveStackList;
		}
	};

	// 部分特殊化
	// 駒打ち生成
	template <Color US>
	struct GenerateMoves<Drop, US> {
		FORCE_INLINE MoveStack* operator () (
			MoveStack* pMovestack, const Position& pos
			) {
			const Bitboard target = pos.GetEmptyBB();
			pMovestack = generateDropMoves(US, pMovestack, pos, target);//<US>
			return pMovestack;
		}
	};

	//*
	// checkSq にある駒で王手されたとき、玉はその駒の利きの位置には移動できないので、移動できない位置を bannnedKingToBB に格納する。
	// 両王手のときには二度連続で呼ばれるため、= ではなく |= を使用している。
	// 最初に呼ばれたときは、bannedKingToBB == allZeroBB() である。
	// todo: FOECE_INLINE と template 省いてNPS比較
	//
	// 黒か白で関数を２つ用意しちゃダメなのかだぜ☆？（＾ｑ＾）？
	//
	//(THEM == Black ? BPawn : WPawn)
	//template <Color THEM>
	static FORCE_INLINE void DoMakeBannedKingTo_black(
		Bitboard& bannedKingToBB,
		const Position& pos,
		const Square checkSq,
		const Square ksq
		) {

		//
		// (^q^)黒か白かで最初に分けちゃダメなのかだぜ☆？
		//
		switch (pos.GetPiece(checkSq))//templateの中なので改造しにくいぜ☆（＾ｑ＾）
		{

			// (^q^)ビットが 0 のとき☆？
			//		case Empty: assert(false); break; // 最適化の為のダミー

		case Piece::N01_BPawn:
		case Piece::N03_BKnight:
			// 歩、桂馬で王手したときは、どこへ逃げても、その駒で取られることはない。
			// よって、ここでは何もしない。
			assert(	pos.GetPiece(checkSq) == N01_BPawn ||pos.GetPiece(checkSq) == N03_BKnight	);
			break;

		case Piece::N02_BLance:
			bannedKingToBB |= g_lanceAttackBb.GetControllBbToEdge(Color::Black, checkSq);
			break;

		case Piece::N04_BSilver:
			bannedKingToBB |= g_silverAttackBb.GetControllBb(Color::Black, checkSq);
			break;

		case Piece::N07_BGold:
		case Piece::N09_BProPawn:
		case Piece::N10_BProLance:
		case Piece::N11_BProKnight:
		case Piece::N12_BProSilver:
			bannedKingToBB |= g_goldAttackBb.GetControllBb(Color::Black, checkSq);
			break;

		case Piece::N05_BBishop:
			bannedKingToBB |= g_bishopAttackBb.GetControllBbToEdge(checkSq);
			break;

		case Piece::N13_BHorse:
			bannedKingToBB |= g_horseAttackBb.GetControllBbToEdge(checkSq);
			break;

		case Piece::N06_BRook:
			bannedKingToBB |= g_rookAttackBb.GetControllBbToEdge(checkSq);
			break;

		case Piece::N14_BDragon:
			if (g_squareRelation.GetSquareRelation(checkSq, ksq) & N04_DirecDiag) {
				// 斜めから王手したときは、玉の移動先と王手した駒の間に駒があることがあるので、
				// dragonAttackToEdge(checkSq) は使えない。
				bannedKingToBB |= g_ptDragon.GetAttacks2From(pos.GetOccupiedBB(), Color::ColorNum, checkSq);
			}
			else {
				bannedKingToBB |= g_dragonAttackBb.GetControllBbToEdge(checkSq);
			}
			break;
		default:
			UNREACHABLE;
		}
	}
	//template <Color THEM>
	static FORCE_INLINE void DoMakeBannedKingTo_white(
		Bitboard& bannedKingToBB,
		const Position& pos,
		const Square checkSq,
		const Square ksq
		) {

		//
		// (^q^)黒か白かで最初に分けちゃダメなのかだぜ☆？
		//
		switch (pos.GetPiece(checkSq))//templateの中なので改造しにくいぜ☆（＾ｑ＾）
		{

			// (^q^)ビットが 0 のとき☆？
			//		case Empty: assert(false); break; // 最適化の為のダミー

			// (^q^)色が黒なら BPawn、そうでなければ WPawn のケースだぜ☆
		case Piece::N17_WPawn:
			// (^q^)色が黒なら BKnight、そうでなければ WKnight のケースだぜ☆
		case Piece::N19_WKnight:
			// 歩、桂馬で王手したときは、どこへ逃げても、その駒で取られることはない。
			// よって、ここでは何もしない。
			assert(
				pos.GetPiece(checkSq) == N17_WPawn ||
				pos.GetPiece(checkSq) == N19_WKnight
				);
			break;

		case Piece::N18_WLance:
			bannedKingToBB |= g_lanceAttackBb.GetControllBbToEdge(Color::White, checkSq);
			break;

		case Piece::N20_WSilver:
			bannedKingToBB |= g_silverAttackBb.GetControllBb(Color::White, checkSq);
			break;

		case Piece::N23_WGold:
		case Piece::N25_WProPawn:
		case Piece::N26_WProLance:
		case Piece::N27_WProKnight:
		case Piece::N28_WProSilver:
			bannedKingToBB |= g_goldAttackBb.GetControllBb(Color::White, checkSq);
			break;

		case Piece::N21_WBishop:
			bannedKingToBB |= g_bishopAttackBb.GetControllBbToEdge(checkSq);
			break;

		case Piece::N29_WHorse:
			bannedKingToBB |= g_horseAttackBb.GetControllBbToEdge(checkSq);
			break;

		case Piece::N22_WRook:
			bannedKingToBB |= g_rookAttackBb.GetControllBbToEdge(checkSq);
			break;

		case Piece::N30_WDragon:
			if (g_squareRelation.GetSquareRelation(checkSq, ksq) & N04_DirecDiag) {
				// 斜めから王手したときは、玉の移動先と王手した駒の間に駒があることがあるので、
				// dragonAttackToEdge(checkSq) は使えない。
				bannedKingToBB |= g_ptDragon.GetAttacks2From(pos.GetOccupiedBB(), Color::ColorNum, checkSq);
			}
			else {
				bannedKingToBB |= g_dragonAttackBb.GetControllBbToEdge(checkSq);
			}
			break;
		default:
			UNREACHABLE;
		}
	}
	//*/

	// 部分特殊化
	// 王手回避生成
	// 王手をしている駒による王手は避けるが、
	// 玉の移動先に敵の利きがある場合と、pinされている味方の駒を動かした場合、非合法手を生成する。
	// そのため、pseudo legal である。
	template <Color US, bool ALL>
	struct GenerateMoves<Evasion, US, ALL> {
		/*FORCE_INLINE*/ MoveStack* operator () (
			MoveStack* pMovestack,
			const Position& pos
			) {
			assert(pos.IsOK());
			assert(pos.InCheck());

			const Square ksq = pos.GetKingSquare(US);

			// 相手の色☆
			const Color Them = UtilColor::OppositeColor(US);

			const Bitboard checkers = pos.GetCheckersBB();
			Bitboard bb = checkers;
			Bitboard bannedKingToBB = Bitboard::CreateAllZeroBB();
			int checkersNum = 0;
			Square checkSq;

			// 玉が逃げられない位置の bitboard を生成する。
			// 絶対に王手が掛かっているので、while ではなく、do while
			do {

				// 最初にビットが立っていたマス☆
				checkSq = bb.PopFirstOneFromI9();

				assert(UtilPiece::ToColor(pos.GetPiece(checkSq)) == Them);

				// カウントアップ☆
				++checkersNum;

				
				// checkSq にある駒で王手されたとき、玉はその駒の利きの位置には移動できないので、
				// 移動できない位置を bannnedKingToBB に格納する。
				// 両王手のときには二度連続で呼ばれるため、= ではなく |= を使用している。
				// 最初に呼ばれたときは、bannedKingToBB == allZeroBB() である。
				// todo: FOECE_INLINE と template 省いてNPS比較
				//無理 (PieceAbstract::*(PieceArray::m_makeBanned2KingTo[pos.GetPiece(checkSq)]))(bannedKingToBB, pos, checkSq, ksq);
				//PieceArray::m_pieceAbstractArray[pos.GetPiece(checkSq)].MakeBanned2KingTo(bannedKingToBB, pos, checkSq, ksq);
				//*
				// 白か黒かによって分けている☆
				(Them == Black ?
					MoveGenerator200::DoMakeBannedKingTo_black(bannedKingToBB, pos, checkSq, ksq) :
					MoveGenerator200::DoMakeBannedKingTo_white(bannedKingToBB, pos, checkSq, ksq)
				);
				//*/

			} while (bb.Exists1Bit());


			// 玉が移動出来る移動先を格納。
			bb = bannedKingToBB.NotThisAnd(pos.GetBbOf(US).NotThisAnd(g_kingAttackBb.GetControllBb(ksq)));
			while (bb.Exists1Bit()) {
				const Square to = bb.PopFirstOneFromI9();
				// 移動先に相手駒の利きがあるか調べずに指し手を生成する。
				// attackersTo() が重いので、movePicker か search で合法手か調べる。
				(*pMovestack++).m_move = g_makePromoteMove.MakeNonPromoteMove<Capture>(N08_King, ksq, to, pos);
			}

			// 両王手なら、玉を移動するしか回避方法は無い。
			// 玉の移動は生成したので、ここで終了
			if (1 < checkersNum) {
				return pMovestack;
			}

			// 王手している駒を玉以外で取る手の生成。
			// pin されているかどうかは movePicker か search で調べる。
			const Bitboard target1 = g_betweenBb.GetBetweenBB(checkSq, ksq);
			const Bitboard target2 = target1 | checkers;
			pMovestack = GeneratePieceMoves<Evasion, N01_Pawn, US, ALL>()(pMovestack, pos, target2, ksq);
			pMovestack = GeneratePieceMoves<Evasion, N02_Lance, US, ALL>()(pMovestack, pos, target2, ksq);
			pMovestack = GeneratePieceMoves<Evasion, N03_Knight, US, ALL>()(pMovestack, pos, target2, ksq);
			pMovestack = GeneratePieceMoves<Evasion, N04_Silver, US, ALL>()(pMovestack, pos, target2, ksq);
			pMovestack = GeneratePieceMoves<Evasion, N05_Bishop, US, ALL>()(pMovestack, pos, target2, ksq);
			pMovestack = GeneratePieceMoves<Evasion, N06_Rook, US, ALL>()(pMovestack, pos, target2, ksq);
			pMovestack = GeneratePieceMoves<Evasion, N16_GoldHorseDragon, US, ALL>()(pMovestack, pos, target2, ksq);

			if (target1.Exists1Bit()) {
				pMovestack = generateDropMoves(US, pMovestack, pos, target1);//<US>
			}

			return pMovestack;
		}
	};

	// 部分特殊化
	// 王手が掛かっていないときの指し手生成
	// これには、玉が相手駒の利きのある地点に移動する自殺手と、pin されている駒を動かす自殺手を含む。
	// ここで生成した手は pseudo legal
	template <Color US> struct GenerateMoves<NonEvasion, US> {
		/*FORCE_INLINE*/ MoveStack* operator () (
			MoveStack* pMovestack, const Position& pos
			) {
			Bitboard target = pos.GetEmptyBB();

			pMovestack = generateDropMoves(US, pMovestack, pos, target);//<US>
			target |= pos.GetBbOf(UtilColor::OppositeColor(US));
			const Square ksq = pos.GetKingSquare(UtilColor::OppositeColor(US));

			pMovestack = GeneratePieceMoves<NonEvasion, N01_Pawn, US, false>()(pMovestack, pos, target, ksq);
			pMovestack = GeneratePieceMoves<NonEvasion, N02_Lance, US, false>()(pMovestack, pos, target, ksq);
			pMovestack = GeneratePieceMoves<NonEvasion, N03_Knight, US, false>()(pMovestack, pos, target, ksq);
			pMovestack = GeneratePieceMoves<NonEvasion, N04_Silver, US, false>()(pMovestack, pos, target, ksq);
			pMovestack = GeneratePieceMoves<NonEvasion, N05_Bishop, US, false>()(pMovestack, pos, target, ksq);
			pMovestack = GeneratePieceMoves<NonEvasion, N06_Rook, US, false>()(pMovestack, pos, target, ksq);
			pMovestack = GeneratePieceMoves<NonEvasion, N16_GoldHorseDragon, US, false>()(pMovestack, pos, target, ksq);
			pMovestack = GeneratePieceMoves<NonEvasion, N08_King, US, false>()(pMovestack, pos, target, ksq);

			return pMovestack;
		}
	};

	// 部分特殊化
	// 連続王手の千日手以外の反則手を排除した合法手生成
	// そんなに速度が要求されるところでは呼ばない。
	template <Color US> struct GenerateMoves<Legal, US> {
		FORCE_INLINE MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos
			) {
			MoveStack* curr = moveStackList;
			const Bitboard pinned = pos.GetPinnedBB();

			moveStackList = pos.InCheck() ?
				GenerateMoves<Evasion, US>()(moveStackList, pos) : GenerateMoves<NonEvasion, US>()(moveStackList, pos);

			// 玉の移動による自殺手と、pinされている駒の移動による自殺手を削除
			while (curr != moveStackList) {
				if (!pos.IsPseudoLegalMoveIsLegal<false, false>(curr->m_move, pinned)) {
					curr->m_move = (--moveStackList)->m_move;
				}
				else {
					++curr;
				}
			}

			return moveStackList;
		}
	};

	// 部分特殊化
	// Evasion のときに歩、飛、角と、香の2段目の不成も生成する。
	template <Color US> struct GenerateMoves<LegalAll, US> {
		FORCE_INLINE MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos
			) {
			MoveStack* curr = moveStackList;
			const Bitboard pinned = pos.GetPinnedBB();

			moveStackList = pos.InCheck() ?
				GenerateMoves<Evasion, US, true>()(moveStackList, pos) : GenerateMoves<NonEvasion, US>()(moveStackList, pos);

			// 玉の移動による自殺手と、pinされている駒の移動による自殺手を削除
			while (curr != moveStackList) {
				if (!pos.IsPseudoLegalMoveIsLegal<false, false>(curr->m_move, pinned)) {
					curr->m_move = (--moveStackList)->m_move;
				}
				else {
					++curr;
				}
			}

			return moveStackList;
		}
	};

public:

	template <MoveType MT>
	static MoveStack* GenerateMoves_2(
		MoveStack* moveStackList, const Position& pos
	) {
		return (pos.GetTurn() == Black ?
			GenerateMoves<MT, Black>()(moveStackList, pos) :
			GenerateMoves<MT, White>()(moveStackList, pos));
	}


	// pin は省かない。
	//template <MoveType MT>
	static MoveStack* GenerateMoves_3(
		MoveStack* moveStackList, const Position& pos, const Square to
	) {
		const Color us = pos.GetTurn();

		Bitboard fromBB = pos.GetAttackersTo(us, to, pos.GetOccupiedBB());
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			const PieceType pieceType = UtilPiece::ToPieceType(pos.GetPiece(from));

			// TODO: 配列のリミットチェックをしてないぜ☆（＾ｑ＾）
			PieceTypeArray::m_ptArray[pieceType].Generate2RecaptureMoves(moveStackList, pos, from, to, us);
			//g_ptArray[pieceType]->Generate2RecaptureMoves(moveStackList, pos, from, to, us);
		}
		return moveStackList;
	}

};