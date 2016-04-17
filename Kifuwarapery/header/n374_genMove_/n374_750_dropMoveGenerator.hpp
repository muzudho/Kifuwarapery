#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_205_utilRank.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_230_setMaskBB.hpp"
#include "../n160_board___/n160_600_bitboardAll.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_650_position.hpp"


class DropMoveGenerator {
public:
	// 駒打ちの場合
	// 歩以外の持ち駒は、loop の前に持ち駒の種類の数によって switch で展開している。
	// ループの展開はコードが膨れ上がる事によるキャッシュヒット率の低下と、演算回数のバランスを取って決める必要がある。
	// NPSに影響が出ないならシンプルにした方が良さそう。
	static MoveStack* GenerateDropMoves(
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
			DropMakerArray::m_dropMakerArray[haveHandNum - noKnightLanceIdx]->MakeDropMovesToRank9ExceptNL(target, TRank9BB, pMovestack, haveHandArr, noKnightLanceIdx);

			// 桂馬以外の持ち駒があれば、
			// 二段目に対して、桂馬以外の指し手を生成。
			DropMakerArray::m_dropMakerArray[haveHandNum - noKnightIdx]->MakeDropMovesToRank8ExceptN(target, TRank8BB, pMovestack, haveHandArr, noKnightIdx);

			// 一、二段目以外に対して、全ての持ち駒の指し手を生成。
			Bitboard toBB = target & ~(TRank8BB | TRank9BB);
			DropMakerArray::m_dropMakerArray[haveHandNum]->MakeDropMovesToRank1234567( toBB, pMovestack, haveHandArr);
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

};