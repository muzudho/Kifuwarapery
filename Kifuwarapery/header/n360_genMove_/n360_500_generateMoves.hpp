#pragma once

#include "../n105_color___/n105_500_utilColor.hpp"
#include "../n110_square__/n110_250_squareDelta.hpp"
#include "../n110_square__/n110_400_squareRelation.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_102_FileMaskBb.hpp"
#include "../n160_board___/n160_106_inFrontMaskBb.hpp"
#include "../n160_board___/n160_230_setMaskBb.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_600_bitboardAll.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n165_movStack/n165_600_utilMove.hpp"
#include "../n220_position/n220_640_utilAttack.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n220_position/n220_670_makePromoteMove.hpp"

#include "../n350_pieceTyp/n350_070_ptAbstract.hpp"
#include "../n350_pieceTyp/n350_100_ptOccupied.hpp"
#include "../n350_pieceTyp/n350_110_ptPawn.hpp"
#include "../n350_pieceTyp/n350_120_ptLance.hpp"
#include "../n350_pieceTyp/n350_130_ptKnight.hpp"
#include "../n350_pieceTyp/n350_140_ptSilver.hpp"
#include "../n350_pieceTyp/n350_150_ptBishop.hpp"
#include "../n350_pieceTyp/n350_160_ptRook.hpp"
#include "../n350_pieceTyp/n350_170_ptGold.hpp"
#include "../n350_pieceTyp/n350_180_ptKing.hpp"
#include "../n350_pieceTyp/n350_190_ptProPawn.hpp"
#include "../n350_pieceTyp/n350_200_ptProLance.hpp"
#include "../n350_pieceTyp/n350_210_ptProKnight.hpp"
#include "../n350_pieceTyp/n350_220_ptProSilver.hpp"
#include "../n350_pieceTyp/n350_230_ptHorse.hpp"
#include "../n350_pieceTyp/n350_240_ptDragon.hpp"
#include "../n350_pieceTyp/n350_500_ptArray.hpp"
#include "../n358_dropMake/n358_100_dropMakerHand0.hpp"
#include "../n358_dropMake/n358_110_dropMakerHand1.hpp"
#include "../n358_dropMake/n358_120_dropMakerHand2.hpp"
#include "../n358_dropMake/n358_130_dropMakerHand3.hpp"
#include "../n358_dropMake/n358_140_dropMakerHand4.hpp"
#include "../n358_dropMake/n358_150_dropMakerHand5.hpp"
#include "../n358_dropMake/n358_160_dropMakerHand6.hpp"
#include "../n358_dropMake/n358_500_dropMakerArray.hpp"


extern const InFrontMaskBb g_inFrontMaskBb;
extern SquareRelation g_squareRelation;

//extern class MoveGenerator;//ファイルの後半で定義







class MoveGenerator {
public://private: ほんとはプライベートにしたい☆名前空間を使ってもいい。

	// 角, 飛車の場合
	static FORCE_INLINE MoveStack* GenerateBishopOrRookMoves_mt_pt_us_all(
		MoveType MT,
		PieceType PT,
		Color US,
		bool ALL,
		MoveStack* moveStackList,
		const Position& pos,
		const Bitboard& target,
		const Square /*ksq*/
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
					(*moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2_mt(MT, PT, from, to, pos);
					if (MT == NonEvasion || ALL)
						(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove_mt(MT, PT, from, to, pos);
				}
				else // 角、飛車は成れるなら成り、不成は生成しない。
					(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove_mt(MT, PT, from, to, pos);
			}
		}
		return moveStackList;
	}

	// 駒打ちの場合
	// 歩以外の持ち駒は、loop の前に持ち駒の種類の数によって switch で展開している。
	// ループの展開はコードが膨れ上がる事によるキャッシュヒット率の低下と、演算回数のバランスを取って決める必要がある。
	// NPSに影響が出ないならシンプルにした方が良さそう。
	//template <Color US>
	static MoveStack* GenerateDropMoves_color(
		Color US,
		MoveStack* pMovestack,
		const Position& pos,
		const Bitboard& target
		) {
		const Hand hand = pos.GetHand(US);

		// まず、歩に対して指し手を生成
		if (hand.Exists<HPawn>()) {

			Bitboard toBB = target;
			// 一段目には打てない

			const Rank TRank9 = (US == Black ? Rank9 : Rank1);
			toBB.AndEqualNot(g_rankMaskBb.GetRankMask_rank(TRank9));

			// 二歩の回避
			Bitboard pawnsBB = pos.GetBbOf(N01_Pawn, US);
			Square pawnsSquare;
			foreachBB(pawnsBB, pawnsSquare, [&](const int part) {
				toBB.SetP(part, toBB.GetP(part) & ~g_fileMaskBb.GetSquareFileMask(pawnsSquare).GetP(part));
			});

			// 打ち歩詰めの回避
			const Rank TRank1 = (US == Black ? Rank1 : Rank9);
			const SquareDelta TDeltaS = (US == Black ? DeltaS : DeltaN);

			const Square ksq = pos.GetKingSquare(UtilColor::OppositeColor(US));
			// 相手玉が九段目なら、歩で王手出来ないので、打ち歩詰めを調べる必要はない。
			if (UtilSquare::ToRank(ksq) != TRank1) {
				const Square pawnDropCheckSquare = ksq + TDeltaS;
				assert(UtilSquare::ContainsOf(pawnDropCheckSquare));
				if (g_setMaskBb.IsSet(&toBB, pawnDropCheckSquare) && pos.GetPiece(pawnDropCheckSquare) == Empty) {
					if (!pos.IsPawnDropCheckMate(US, pawnDropCheckSquare)) {
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

			const Rank TRank8 = (US == Black ? Rank8 : Rank2);
			const Rank TRank9 = (US == Black ? Rank9 : Rank1);
			const Bitboard TRank8BB = g_rankMaskBb.GetRankMask_rank(TRank8);
			const Bitboard TRank9BB = g_rankMaskBb.GetRankMask_rank(TRank9);


			/*
			// 桂馬、香車 以外の持ち駒があれば、
			// 一段目に対して、桂馬、香車以外の指し手を生成。
			// FIXME: 配列の範囲チェックしてないぜ☆（＾ｑ＾）
			g_dropMakerArray[haveHandNum - noKnightLanceIdx]->MakeDropMovesToRank9ExceptNL(target, TRank9BB, pMoveStackList, haveHand, noKnightLanceIdx);

			// 桂馬以外の持ち駒があれば、
			// 二段目に対して、桂馬以外の指し手を生成。
			g_dropMakerArray[haveHandNum - noKnightIdx]->MakeDropMovesToRank8ExceptN(target, TRank8BB, pMoveStackList, haveHand, noKnightIdx);

			// 一、二段目以外に対して、全ての持ち駒の指し手を生成。
			Bitboard toBB = target & ~(TRank8BB | TRank9BB);
			g_dropMakerArray[haveHandNum]->MakeDropMovesToRank1234567( toBB, pMoveStackList, haveHand);
			*/

			/*
			Bitboard toBB;
			Square to;
			// 桂馬、香車 以外の持ち駒があれば、
			// 一段目に対して、桂馬、香車以外の指し手を生成。
			switch (haveHandNum - noKnightLanceIdx) {
			case 0: g_dropMakerHand0.MakeDropMovesToRank9ExceptNL(target, TRank9BB, pMoveStackList, haveHand, noKnightLanceIdx); break;
			case 1: g_dropMakerHand1.MakeDropMovesToRank9ExceptNL(target, TRank9BB, pMoveStackList, haveHand, noKnightLanceIdx); break;
			case 2: g_dropMakerHand2.MakeDropMovesToRank9ExceptNL(target, TRank9BB, pMoveStackList, haveHand, noKnightLanceIdx); break;
			case 3: g_dropMakerHand3.MakeDropMovesToRank9ExceptNL(target, TRank9BB, pMoveStackList, haveHand, noKnightLanceIdx); break;
			case 4: g_dropMakerHand4.MakeDropMovesToRank9ExceptNL(target, TRank9BB, pMoveStackList, haveHand, noKnightLanceIdx); break;
			default: UNREACHABLE;
			}

			// 桂馬以外の持ち駒があれば、
			// 二段目に対して、桂馬以外の指し手を生成。
			switch (haveHandNum - noKnightIdx) {
			case 0: g_dropMakerHand0.MakeDropMovesToRank8ExceptN(target, TRank8BB, pMoveStackList, haveHand, noKnightIdx); break;
			case 1: g_dropMakerHand1.MakeDropMovesToRank8ExceptN(target, TRank8BB, pMoveStackList, haveHand, noKnightIdx); break;
			case 2: g_dropMakerHand2.MakeDropMovesToRank8ExceptN(target, TRank8BB, pMoveStackList, haveHand, noKnightIdx); break;
			case 3: g_dropMakerHand3.MakeDropMovesToRank8ExceptN(target, TRank8BB, pMoveStackList, haveHand, noKnightIdx); break;
			case 4: g_dropMakerHand4.MakeDropMovesToRank8ExceptN(target, TRank8BB, pMoveStackList, haveHand, noKnightIdx); break;
			case 5: g_dropMakerHand5.MakeDropMovesToRank8ExceptN(target, TRank8BB, pMoveStackList, haveHand, noKnightIdx); break;
			default: UNREACHABLE;
			}

			// 一、二段目以外に対して、全ての持ち駒の指し手を生成。
			toBB = target & ~(TRank8BB | TRank9BB);
			switch (haveHandNum) {
			case 0: g_dropMakerHand0.MakeDropMovesToRank1234567(toBB, pMoveStackList, haveHand); break;
			case 1: g_dropMakerHand1.MakeDropMovesToRank1234567(toBB, pMoveStackList, haveHand); break;
			case 2: g_dropMakerHand2.MakeDropMovesToRank1234567(toBB, pMoveStackList, haveHand); break;
			case 3: g_dropMakerHand3.MakeDropMovesToRank1234567(toBB, pMoveStackList, haveHand); break;
			case 4: g_dropMakerHand4.MakeDropMovesToRank1234567(toBB, pMoveStackList, haveHand); break;
			case 5: g_dropMakerHand5.MakeDropMovesToRank1234567(toBB, pMoveStackList, haveHand); break;
			case 6: g_dropMakerHand6.MakeDropMovesToRank1234567(toBB, pMoveStackList, haveHand); break;
			default: UNREACHABLE;
			}
			*/

			/*NG
			Bitboard toBB;
			Square to;
			// 桂馬、香車 以外の持ち駒があれば、
			// 一段目に対して、桂馬、香車以外の指し手を生成。
			switch (haveHandNum - noKnightLanceIdx) {
			case 0: Func0A(); break;
			case 1: Func1A(target, TRank9BB, haveHand, pMoveStackList, noKnightLanceIdx); break;
			case 2: Func2A(target, TRank9BB, haveHand, pMoveStackList, noKnightLanceIdx); break;
			case 3: Func3A(target, TRank9BB, haveHand, pMoveStackList, noKnightLanceIdx); break;
			case 4: Func4A(target, TRank9BB, haveHand, pMoveStackList, noKnightLanceIdx); break;
			default: UNREACHABLE;
			}

			// 桂馬以外の持ち駒があれば、
			// 二段目に対して、桂馬以外の指し手を生成。
			switch (haveHandNum - noKnightIdx) {
			case 0: Func0B(); break;
			case 1: Func1B(target, TRank8BB, haveHand, pMoveStackList, noKnightIdx); break;
			case 2: Func2B(target, TRank8BB, haveHand, pMoveStackList, noKnightIdx); break;
			case 3: Func3B(target, TRank8BB, haveHand, pMoveStackList, noKnightIdx); break;
			case 4: Func4B(target, TRank8BB, haveHand, pMoveStackList, noKnightIdx); break;
			case 5: Func5B(target, TRank8BB, haveHand, pMoveStackList, noKnightIdx); break;
			default: UNREACHABLE;
			}

			// 一、二段目以外に対して、全ての持ち駒の指し手を生成。
			toBB = target & ~(TRank8BB | TRank9BB);
			switch (haveHandNum) {
			case 0: Func0C(); break;
			case 1: Func1C(toBB, haveHand, pMoveStackList); break;
			case 2: Func2C(toBB, haveHand, pMoveStackList); break;
			case 3: Func3C(toBB, haveHand, pMoveStackList); break;
			case 4: Func4C(toBB, haveHand, pMoveStackList); break;
			case 5: Func5C(toBB, haveHand, pMoveStackList); break;
			case 6: Func6C(toBB, haveHand, pMoveStackList); break;
			default: UNREACHABLE;
			}
			*/

			/*
			//Square iTo;// ループカウンタを兼ねる。
			// 桂馬、香車 以外の持ち駒があれば、
			// 一段目に対して、桂馬、香車以外の指し手を生成。
			switch (haveHandNum - noKnightLanceIdx) {
			case 0:
			{
			Func0A();
			break;
			}
			case 1:
			{
			Func1A(target, TRank9BB, haveHandArr, pMovestack, noKnightLanceIdx);
			break;
			}
			case 2:
			{
			Func2A(target, TRank9BB, haveHandArr, pMovestack, noKnightLanceIdx);
			break;
			}
			case 3:
			{
			Func3A(target, TRank9BB, haveHandArr, pMovestack, noKnightLanceIdx);
			break;
			}
			case 4:
			{
			Func4A(target, TRank9BB, haveHandArr, pMovestack, noKnightLanceIdx);
			break;
			}
			default:
			{
			UNREACHABLE;
			}
			}

			// 桂馬以外の持ち駒があれば、
			// 二段目に対して、桂馬以外の指し手を生成。
			switch (haveHandNum - noKnightIdx) {
			case 0:
			{
			Func0B();
			break;
			}
			case 1:
			{
			Func1B(target, TRank8BB, haveHandArr, pMovestack, noKnightIdx);
			break;
			}
			case 2:
			{
			Func2B(target, TRank8BB, haveHandArr, pMovestack, noKnightIdx);
			break;
			}
			case 3:
			{
			Func3B(target, TRank8BB, haveHandArr, pMovestack, noKnightIdx);
			break;
			}
			case 4:
			{
			Func4B(target, TRank8BB, haveHandArr, pMovestack, noKnightIdx);
			break;
			}
			case 5:
			{
			Func5B(target, TRank8BB, haveHandArr, pMovestack, noKnightIdx);
			break;
			}
			default:
			{
			UNREACHABLE;
			}
			}

			// 一、二段目以外に対して、全ての持ち駒の指し手を生成。
			//Bitboard toBB = target & ~(TRank8BB | TRank9BB);
			switch (haveHandNum) {
			case 0:
			{
			Func0C();
			break;
			}
			case 1:
			{
			Func1C(target, TRank8BB, TRank9BB, haveHandArr, pMovestack);
			break;
			}
			case 2:
			{
			Func2C(target, TRank8BB, TRank9BB, haveHandArr, pMovestack);
			break;
			}
			case 3:
			{
			Func3C(target, TRank8BB, TRank9BB, haveHandArr, pMovestack);
			break;
			}
			case 4:
			{
			Func4C(target, TRank8BB, TRank9BB, haveHandArr, pMovestack);
			break;
			}
			case 5:
			{
			Func5C(target, TRank8BB, TRank9BB, haveHandArr, pMovestack);
			break;
			}
			case 6:
			{
			Func6C(target, TRank8BB, TRank9BB, haveHandArr, pMovestack);
			break;
			}
			default:
			{
			UNREACHABLE;
			}
			}
			*/

			//Square iTo;// ループカウンタを兼ねる。
			// 桂馬、香車 以外の持ち駒があれば、
			// 一段目に対して、桂馬、香車以外の指し手を生成。
			switch (haveHandNum - noKnightLanceIdx) {
			case 0:
			{
				break; // 桂馬、香車 以外の持ち駒がない。
			}
			case 1:
			{
				Bitboard toBB = target & TRank9BB;
				while (toBB.GetP(0)) {
					Square iTo = toBB.PopFirstOneRightFromI9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
				}
				while (toBB.GetP(1)) {
					Square iTo = toBB.PopFirstOneLeftFromB9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
				}
				break;
			}
			case 2:
			{
				Bitboard toBB = target & TRank9BB;
				while (toBB.GetP(0)) {
					Square iTo = toBB.PopFirstOneRightFromI9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
				}
				while (toBB.GetP(1)) {
					Square iTo = toBB.PopFirstOneLeftFromB9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
				}
				break;
			}
			case 3:
			{
				Bitboard toBB = target & TRank9BB;
				while (toBB.GetP(0)) {
					Square iTo = toBB.PopFirstOneRightFromI9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
				}
				while (toBB.GetP(1)) {
					Square iTo = toBB.PopFirstOneLeftFromB9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
				}
				break;
			}
			case 4:
			{
				Bitboard toBB = target & TRank9BB;
				while (toBB.GetP(0)) {
					Square iTo = toBB.PopFirstOneRightFromI9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 3], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
				}
				while (toBB.GetP(1)) {
					Square iTo = toBB.PopFirstOneLeftFromB9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 3], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
				}
				break;
			}
			default:
			{
				UNREACHABLE;
			}
			}

			// 桂馬以外の持ち駒があれば、
			// 二段目に対して、桂馬以外の指し手を生成。
			switch (haveHandNum - noKnightIdx) {
			case 0:
			{
				break; // 桂馬 以外の持ち駒がない。
			}
			case 1:
			{
				Bitboard toBB = target & TRank8BB;
				while (toBB.GetP(0)) {
					Square iTo = toBB.PopFirstOneRightFromI9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
				}
				while (toBB.GetP(1)) {
					Square iTo = toBB.PopFirstOneLeftFromB9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
				}
				break;
			}
			case 2:
			{
				Bitboard toBB = target & TRank8BB;
				while (toBB.GetP(0)) {
					Square iTo = toBB.PopFirstOneRightFromI9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
				}
				while (toBB.GetP(1)) {
					Square iTo = toBB.PopFirstOneLeftFromB9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
				}
				break;
			}
			case 3:
			{
				Bitboard toBB = target & TRank8BB;
				while (toBB.GetP(0)) {
					Square iTo = toBB.PopFirstOneRightFromI9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
				}
				while (toBB.GetP(1)) {
					Square iTo = toBB.PopFirstOneLeftFromB9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
				}
				break;
			}
			case 4:
			{
				Bitboard toBB = target & TRank8BB;
				while (toBB.GetP(0)) {
					Square iTo = toBB.PopFirstOneRightFromI9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 3], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
				}
				while (toBB.GetP(1)) {
					Square iTo = toBB.PopFirstOneLeftFromB9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 3], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
				}
				break;
			}
			case 5:
			{
				Bitboard toBB = target & TRank8BB;
				while (toBB.GetP(0)) {
					Square iTo = toBB.PopFirstOneRightFromI9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 4], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 3], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
				}
				while (toBB.GetP(1)) {
					Square iTo = toBB.PopFirstOneLeftFromB9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 4], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 3], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
				}
				break;
			}
			default:
			{
				UNREACHABLE;
			}
			}

			// 一、二段目以外に対して、全ての持ち駒の指し手を生成。
			//Bitboard toBB = target & ~(TRank8BB | TRank9BB);
			switch (haveHandNum) {
			case 0:
			{
				assert(false); break; // 最適化の為のダミー
			}
			case 1:
			{
				Bitboard toBB = target & ~(TRank8BB | TRank9BB);
				while (toBB.GetP(0)) {
					Square iTo = toBB.PopFirstOneRightFromI9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
				}
				while (toBB.GetP(1)) {
					Square iTo = toBB.PopFirstOneLeftFromB9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
				}
				break;
			}
			case 2:
			{
				Bitboard toBB = target & ~(TRank8BB | TRank9BB);
				while (toBB.GetP(0)) {
					Square iTo = toBB.PopFirstOneRightFromI9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
				}
				while (toBB.GetP(1)) {
					Square iTo = toBB.PopFirstOneLeftFromB9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
				}
				break;
			}
			case 3:
			{
				Bitboard toBB = target & ~(TRank8BB | TRank9BB);
				while (toBB.GetP(0)) {
					Square iTo = toBB.PopFirstOneRightFromI9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
				}
				while (toBB.GetP(1)) {
					Square iTo = toBB.PopFirstOneLeftFromB9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
				}
				break;
			}
			case 4:
			{
				Bitboard toBB = target & ~(TRank8BB | TRank9BB);
				while (toBB.GetP(0)) {
					Square iTo = toBB.PopFirstOneRightFromI9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
				}
				while (toBB.GetP(1)) {
					Square iTo = toBB.PopFirstOneLeftFromB9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
				}
				break;
			}
			case 5:
			{
				Bitboard toBB = target & ~(TRank8BB | TRank9BB);
				while (toBB.GetP(0)) {
					Square iTo = toBB.PopFirstOneRightFromI9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[4], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
				}
				while (toBB.GetP(1)) {
					Square iTo = toBB.PopFirstOneLeftFromB9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[4], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
				}
				break;
			}
			case 6:
			{
				Bitboard toBB = target & ~(TRank8BB | TRank9BB);
				while (toBB.GetP(0)) {
					Square iTo = toBB.PopFirstOneRightFromI9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[5], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[4], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
				}
				while (toBB.GetP(1)) {
					Square iTo = toBB.PopFirstOneLeftFromB9();
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[5], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[4], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
				}
				break;
			}
			default:
			{
				UNREACHABLE;
			}
			}

			/* OK
			//Square iTo;// ループカウンタを兼ねる。
			// 桂馬、香車 以外の持ち駒があれば、
			// 一段目に対して、桂馬、香車以外の指し手を生成。
			switch (haveHandNum - noKnightLanceIdx) {
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
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo);
			};
			}
			while (toBB.GetP(1)) {
			Square iTo = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo);
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
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo);
			};
			}
			while (toBB.GetP(1)) {
			Square iTo = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo);
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
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 2], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo);
			};
			}
			while (toBB.GetP(1)) {
			Square iTo = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 2], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo);
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
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 3], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 2], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo);
			};
			}
			while (toBB.GetP(1)) {
			Square iTo = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 3], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 2], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo);
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
			switch (haveHandNum - noKnightIdx) {
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
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo);
			};
			}
			while (toBB.GetP(1)) {
			Square iTo = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo);
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
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo);
			};
			}
			while (toBB.GetP(1)) {
			Square iTo = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo);
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
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo);
			};
			}
			while (toBB.GetP(1)) {
			Square iTo = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo);
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
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 3], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo);
			};
			}
			while (toBB.GetP(1)) {
			Square iTo = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 3], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo);
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
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 4], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 3], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo);
			};
			}
			while (toBB.GetP(1)) {
			Square iTo = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 4], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 3], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo);
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
			switch (haveHandNum) {
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
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo);
			};
			}
			while (toBB.GetP(1)) {
			Square iTo = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo);
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
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo);
			};
			}
			while (toBB.GetP(1)) {
			Square iTo = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo);
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
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo);
			};
			}
			while (toBB.GetP(1)) {
			Square iTo = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo);
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
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo);
			};
			}
			while (toBB.GetP(1)) {
			Square iTo = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo);
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
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[4], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo);
			};
			}
			while (toBB.GetP(1)) {
			Square iTo = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[4], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo);
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
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[5], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[4], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo);
			};
			}
			while (toBB.GetP(1)) {
			Square iTo = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[5], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[4], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo);
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
			*/

			/* OK
			Bitboard toBB;
			Square to;
			// 桂馬、香車 以外の持ち駒があれば、
			// 一段目に対して、桂馬、香車以外の指し手を生成。
			switch (haveHandNum - noKnightLanceIdx) {
			case 0: break; // 桂馬、香車 以外の持ち駒がない。
			case 1: toBB = target & TRank9BB;
			do {
			while (toBB.GetP(0)) {
			to = toBB.PopFirstOneRightFromI9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 0], to);
			};
			}
			while (toBB.GetP(1)) {
			to = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 0], to);
			};
			}
			} while (false);
			break;
			case 2: toBB = target & TRank9BB;
			do {
			while (toBB.GetP(0)) {
			to = toBB.PopFirstOneRightFromI9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 0], to);
			};
			}
			while (toBB.GetP(1)) {
			to = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 0], to);
			};
			}
			} while (false);
			break;
			case 3: toBB = target & TRank9BB;
			do {
			while (toBB.GetP(0)) {
			to = toBB.PopFirstOneRightFromI9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 0], to);
			};
			}
			while (toBB.GetP(1)) {
			to = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 0], to);
			};
			}
			} while (false);
			break;
			case 4: toBB = target & TRank9BB;
			do {
			while (toBB.GetP(0)) {
			to = toBB.PopFirstOneRightFromI9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 3], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 0], to);
			};
			}
			while (toBB.GetP(1)) {
			to = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 3], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 0], to);
			};
			}
			} while (false);
			break;
			default: UNREACHABLE;
			}

			// 桂馬以外の持ち駒があれば、
			// 二段目に対して、桂馬以外の指し手を生成。
			switch (haveHandNum - noKnightIdx) {
			case 0: break; // 桂馬 以外の持ち駒がない。
			case 1: toBB = target & TRank8BB;
			do {
			while (toBB.GetP(0)) {
			to = toBB.PopFirstOneRightFromI9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
			};
			}
			while (toBB.GetP(1)) {
			to = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
			};
			}
			} while (false);
			break;
			case 2: toBB = target & TRank8BB;
			do {
			while (toBB.GetP(0)) {
			to = toBB.PopFirstOneRightFromI9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
			};
			}
			while (toBB.GetP(1)) {
			to = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
			};
			}
			} while (false);
			break;
			case 3: toBB = target & TRank8BB;
			do {
			while (toBB.GetP(0)) {
			to = toBB.PopFirstOneRightFromI9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
			};
			}
			while (toBB.GetP(1)) {
			to = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
			};
			}
			} while (false);
			break;
			case 4: toBB = target & TRank8BB;
			do {
			while (toBB.GetP(0)) {
			to = toBB.PopFirstOneRightFromI9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 3], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
			};
			}
			while (toBB.GetP(1)) {
			to = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 3], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
			};
			}
			} while (false);
			break;
			case 5: toBB = target & TRank8BB;
			do {
			while (toBB.GetP(0)) {
			to = toBB.PopFirstOneRightFromI9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 4], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 3], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
			};
			}
			while (toBB.GetP(1)) {
			to = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 4], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 3], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
			};
			}
			} while (false);
			break;
			default: UNREACHABLE;
			}

			// 一、二段目以外に対して、全ての持ち駒の指し手を生成。
			toBB = target & ~(TRank8BB | TRank9BB);
			switch (haveHandNum) {
			case 0: assert(false); break; // 最適化の為のダミー
			case 1:
			do {
			while (toBB.GetP(0)) {
			to = toBB.PopFirstOneRightFromI9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
			};
			}
			while (toBB.GetP(1)) {
			to = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
			};
			}
			} while (false);
			break;
			case 2:
			do {
			while (toBB.GetP(0)) {
			to = toBB.PopFirstOneRightFromI9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
			};
			}
			while (toBB.GetP(1)) {
			to = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
			};
			}
			} while (false);
			break;
			case 3:
			do {
			while (toBB.GetP(0)) {
			to = toBB.PopFirstOneRightFromI9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
			};
			}
			while (toBB.GetP(1)) {
			to = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
			};
			}
			} while (false);
			break;
			case 4:
			do {
			while (toBB.GetP(0)) {
			to = toBB.PopFirstOneRightFromI9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[3], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
			};
			}
			while (toBB.GetP(1)) {
			to = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[3], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
			};
			}
			} while (false);
			break;
			case 5:
			do {
			while (toBB.GetP(0)) {
			to = toBB.PopFirstOneRightFromI9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[4], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[3], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
			};
			}
			while (toBB.GetP(1)) {
			to = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[4], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[3], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
			};
			}
			} while (false);
			break;
			case 6:
			do {
			while (toBB.GetP(0)) {
			to = toBB.PopFirstOneRightFromI9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[5], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[4], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[3], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
			};
			}
			while (toBB.GetP(1)) {
			to = toBB.PopFirstOneLeftFromB9();
			{
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[5], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[4], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[3], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
			};
			}
			} while (false);
			break;
			default: UNREACHABLE;
			}
			*/

			/*ok
			Bitboard toBB;
			Square to;
			// 桂馬、香車 以外の持ち駒があれば、
			// 一段目に対して、桂馬、香車以外の指し手を生成。
			switch (haveHandNum - noKnightLanceIdx) {
			case 0: break; // 桂馬、香車 以外の持ち駒がない。
			case 1: toBB = target & TRank9BB; FOREACH_BB(toBB, to, {
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 0], to);
			}); break;
			case 2: toBB = target & TRank9BB; FOREACH_BB(toBB, to, {
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 0], to);
			}); break;
			case 3: toBB = target & TRank9BB; FOREACH_BB(toBB, to, {
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 0], to);
			}); break;
			case 4: toBB = target & TRank9BB; FOREACH_BB(toBB, to, {
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 3], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 0], to);
			}); break;
			default: UNREACHABLE;
			}

			// 桂馬以外の持ち駒があれば、
			// 二段目に対して、桂馬以外の指し手を生成。
			switch (haveHandNum - noKnightIdx) {
			case 0: break; // 桂馬 以外の持ち駒がない。
			case 1: toBB = target & TRank8BB; FOREACH_BB(toBB, to, {
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
			}); break;
			case 2: toBB = target & TRank8BB; FOREACH_BB(toBB, to, {
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
			}); break;
			case 3: toBB = target & TRank8BB; FOREACH_BB(toBB, to, {
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
			}); break;
			case 4: toBB = target & TRank8BB; FOREACH_BB(toBB, to, {
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 3], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
			}); break;
			case 5: toBB = target & TRank8BB; FOREACH_BB(toBB, to, {
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 4], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 3], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
			}); break;
			default: UNREACHABLE;
			}

			// 一、二段目以外に対して、全ての持ち駒の指し手を生成。
			toBB = target & ~(TRank8BB | TRank9BB);
			switch (haveHandNum) {
			case 0: assert(false); break; // 最適化の為のダミー
			case 1: FOREACH_BB(toBB, to, {
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
			}); break;
			case 2: FOREACH_BB(toBB, to, {
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
			}); break;
			case 3: FOREACH_BB(toBB, to, {
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
			}); break;
			case 4: FOREACH_BB(toBB, to, {
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[3], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
			}); break;
			case 5: FOREACH_BB(toBB, to, {
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[4], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[3], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
			}); break;
			case 6: FOREACH_BB(toBB, to, {
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[5], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[4], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[3], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
			}); break;
			default: UNREACHABLE;
			}
			*/

			/*ok
			Bitboard toBB;
			Square to;
			// 桂馬、香車 以外の持ち駒があれば、
			// 一段目に対して、桂馬、香車以外の指し手を生成。
			switch (haveHandNum - noKnightLanceIdx) {
			case 0: break; // 桂馬、香車 以外の持ち駒がない。
			case 1: toBB = target & TRank9BB; FOREACH_BB(toBB, to, { Unroller<1>()([&](const int i) { (*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + i], to); }); }); break;
			case 2: toBB = target & TRank9BB; FOREACH_BB(toBB, to, { Unroller<2>()([&](const int i) { (*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + i], to); }); }); break;
			case 3: toBB = target & TRank9BB; FOREACH_BB(toBB, to, { Unroller<3>()([&](const int i) { (*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + i], to); }); }); break;
			case 4: toBB = target & TRank9BB; FOREACH_BB(toBB, to, { Unroller<4>()([&](const int i) { (*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + i], to); }); }); break;
			default: UNREACHABLE;
			}

			// 桂馬以外の持ち駒があれば、
			// 二段目に対して、桂馬以外の指し手を生成。
			switch (haveHandNum - noKnightIdx) {
			case 0: break; // 桂馬 以外の持ち駒がない。
			case 1: toBB = target & TRank8BB; FOREACH_BB(toBB, to, { Unroller<1>()([&](const int i) { (*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + i], to); }); }); break;
			case 2: toBB = target & TRank8BB; FOREACH_BB(toBB, to, { Unroller<2>()([&](const int i) { (*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + i], to); }); }); break;
			case 3: toBB = target & TRank8BB; FOREACH_BB(toBB, to, { Unroller<3>()([&](const int i) { (*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + i], to); }); }); break;
			case 4: toBB = target & TRank8BB; FOREACH_BB(toBB, to, { Unroller<4>()([&](const int i) { (*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + i], to); }); }); break;
			case 5: toBB = target & TRank8BB; FOREACH_BB(toBB, to, { Unroller<5>()([&](const int i) { (*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + i], to); }); }); break;
			default: UNREACHABLE;
			}

			// 一、二段目以外に対して、全ての持ち駒の指し手を生成。
			toBB = target & ~(TRank8BB | TRank9BB);
			switch (haveHandNum) {
			case 0: assert(false); break; // 最適化の為のダミー
			case 1: FOREACH_BB(toBB, to, { Unroller<1>()([&](const int i) { (*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[i], to); }); }); break;
			case 2: FOREACH_BB(toBB, to, { Unroller<2>()([&](const int i) { (*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[i], to); }); }); break;
			case 3: FOREACH_BB(toBB, to, { Unroller<3>()([&](const int i) { (*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[i], to); }); }); break;
			case 4: FOREACH_BB(toBB, to, { Unroller<4>()([&](const int i) { (*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[i], to); }); }); break;
			case 5: FOREACH_BB(toBB, to, { Unroller<5>()([&](const int i) { (*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[i], to); }); }); break;
			case 6: FOREACH_BB(toBB, to, { Unroller<6>()([&](const int i) { (*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[i], to); }); }); break;
			default: UNREACHABLE;
			}
			*/
		}

		return pMovestack;
	}

	/* NG
	static void Func0A()
	{
	// 桂馬、香車 以外の持ち駒がない。
	}
	static void Func0B()
	{
	// 桂馬 以外の持ち駒がない。
	}
	static void Func0C()
	{
	assert(false); // 最適化の為のダミー
	}
	static void Func1A(const Bitboard& target, const Bitboard& TRank9BB, PieceType haveHandArr[6], MoveStack* pMovestack, const int noKnightLanceIdx)
	{
	Bitboard toBB = target & TRank9BB;
	while (toBB.GetP(0)) {
	Square iTo = toBB.PopFirstOneRightFromI9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
	}
	while (toBB.GetP(1)) {
	Square iTo = toBB.PopFirstOneLeftFromB9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
	}
	}
	static void Func1B(const Bitboard& target, const Bitboard& TRank8BB, PieceType haveHandArr[6], MoveStack* pMovestack, const int noKnightIdx)
	{
	Bitboard toBB = target & TRank8BB;
	while (toBB.GetP(0)) {
	Square iTo = toBB.PopFirstOneRightFromI9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
	}
	while (toBB.GetP(1)) {
	Square iTo = toBB.PopFirstOneLeftFromB9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
	}
	}
	static void Func1C(const Bitboard& target, const Bitboard& TRank8BB, const Bitboard& TRank9BB, PieceType haveHandArr[6], MoveStack* pMovestack)
	{
	Bitboard toBB = target & ~(TRank8BB | TRank9BB);
	while (toBB.GetP(0)) {
	Square iTo = toBB.PopFirstOneRightFromI9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
	}
	while (toBB.GetP(1)) {
	Square iTo = toBB.PopFirstOneLeftFromB9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
	}
	}
	static void Func2A(const Bitboard& target, const Bitboard& TRank9BB, PieceType haveHandArr[6], MoveStack* pMovestack, const int noKnightLanceIdx)
	{
	Bitboard toBB = target & TRank9BB;
	while (toBB.GetP(0)) {
	Square iTo = toBB.PopFirstOneRightFromI9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
	}
	while (toBB.GetP(1)) {
	Square iTo = toBB.PopFirstOneLeftFromB9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
	}
	}
	static void Func2B(const Bitboard& target, const Bitboard& TRank8BB, PieceType haveHandArr[6], MoveStack* pMovestack, const int noKnightIdx)
	{
	Bitboard toBB = target & TRank8BB;
	while (toBB.GetP(0)) {
	Square iTo = toBB.PopFirstOneRightFromI9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
	}
	while (toBB.GetP(1)) {
	Square iTo = toBB.PopFirstOneLeftFromB9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
	}
	}
	static void Func2C(const Bitboard& target, const Bitboard& TRank8BB, const Bitboard& TRank9BB, PieceType haveHandArr[6], MoveStack* pMovestack)
	{
	Bitboard toBB = target & ~(TRank8BB | TRank9BB);
	while (toBB.GetP(0)) {
	Square iTo = toBB.PopFirstOneRightFromI9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
	}
	while (toBB.GetP(1)) {
	Square iTo = toBB.PopFirstOneLeftFromB9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
	}
	}
	static void Func3A(const Bitboard& target, const Bitboard& TRank9BB, PieceType haveHandArr[6], MoveStack* pMovestack, const int noKnightLanceIdx)
	{
	Bitboard toBB = target & TRank9BB;
	while (toBB.GetP(0)) {
	Square iTo = toBB.PopFirstOneRightFromI9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 2], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
	}
	while (toBB.GetP(1)) {
	Square iTo = toBB.PopFirstOneLeftFromB9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 2], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
	}
	}
	static void Func3B(const Bitboard& target, const Bitboard& TRank8BB, PieceType haveHandArr[6], MoveStack* pMovestack, const int noKnightIdx)
	{
	Bitboard toBB = target & TRank8BB;
	while (toBB.GetP(0)) {
	Square iTo = toBB.PopFirstOneRightFromI9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
	}
	while (toBB.GetP(1)) {
	Square iTo = toBB.PopFirstOneLeftFromB9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
	}
	}
	static void Func3C(const Bitboard& target, const Bitboard& TRank8BB, const Bitboard& TRank9BB, PieceType haveHandArr[6], MoveStack* pMovestack)
	{
	Bitboard toBB = target & ~(TRank8BB | TRank9BB);
	while (toBB.GetP(0)) {
	Square iTo = toBB.PopFirstOneRightFromI9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
	}
	while (toBB.GetP(1)) {
	Square iTo = toBB.PopFirstOneLeftFromB9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
	}
	}
	static void Func4A(const Bitboard& target, const Bitboard& TRank9BB, PieceType haveHandArr[6], MoveStack* pMovestack, const int noKnightLanceIdx)
	{
	Bitboard toBB = target & TRank9BB;
	while (toBB.GetP(0)) {
	Square iTo = toBB.PopFirstOneRightFromI9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 3], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 2], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
	}
	while (toBB.GetP(1)) {
	Square iTo = toBB.PopFirstOneLeftFromB9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 3], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 2], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
	}
	}
	static void Func4B(const Bitboard& target, const Bitboard& TRank8BB, PieceType haveHandArr[6], MoveStack* pMovestack, const int noKnightIdx)
	{
	Bitboard toBB = target & TRank8BB;
	while (toBB.GetP(0)) {
	Square iTo = toBB.PopFirstOneRightFromI9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 3], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
	}
	while (toBB.GetP(1)) {
	Square iTo = toBB.PopFirstOneLeftFromB9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 3], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
	}
	}
	static void Func4C(const Bitboard& target, const Bitboard& TRank8BB, const Bitboard& TRank9BB, PieceType haveHandArr[6], MoveStack* pMovestack)
	{
	Bitboard toBB = target & ~(TRank8BB | TRank9BB);
	while (toBB.GetP(0)) {
	Square iTo = toBB.PopFirstOneRightFromI9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
	}
	while (toBB.GetP(1)) {
	Square iTo = toBB.PopFirstOneLeftFromB9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
	}
	}
	static void Func5B(const Bitboard& target, const Bitboard& TRank8BB, PieceType haveHandArr[6], MoveStack* pMovestack, const int noKnightIdx)
	{
	Bitboard toBB = target & TRank8BB;
	while (toBB.GetP(0)) {
	Square iTo = toBB.PopFirstOneRightFromI9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 4], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 3], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
	}
	while (toBB.GetP(1)) {
	Square iTo = toBB.PopFirstOneLeftFromB9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 4], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 3], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
	}
	}
	static void Func5C(const Bitboard& target, const Bitboard& TRank8BB, const Bitboard& TRank9BB, PieceType haveHandArr[6], MoveStack* pMovestack)
	{
	Bitboard toBB = target & ~(TRank8BB | TRank9BB);
	while (toBB.GetP(0)) {
	Square iTo = toBB.PopFirstOneRightFromI9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[4], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
	}
	while (toBB.GetP(1)) {
	Square iTo = toBB.PopFirstOneLeftFromB9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[4], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
	}
	}
	static void Func6C(const Bitboard& target, const Bitboard& TRank8BB, const Bitboard& TRank9BB, PieceType haveHandArr[6], MoveStack* pMovestack)
	{
	Bitboard toBB = target & ~(TRank8BB | TRank9BB);
	while (toBB.GetP(0)) {
	Square iTo = toBB.PopFirstOneRightFromI9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[5], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[4], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
	}
	while (toBB.GetP(1)) {
	Square iTo = toBB.PopFirstOneLeftFromB9();
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[5], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[4], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
	pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
	}
	}
	*/

	/*NG
	void Func0A()
	{
	// 桂馬、香車 以外の持ち駒がない。
	}
	void Func0B()
	{
	// 桂馬 以外の持ち駒がない。
	}
	void Func0C()
	{
	assert(false);// 最適化の為のダミー
	}
	void Func1A(const Bitboard& target, const Bitboard& TRank9BB, PieceType haveHand[6], MoveStack* pMoveStackList, const int noKnightLanceIdx)
	{
	Bitboard toBB = target & TRank9BB;
	Square to;
	do {
	while (toBB.GetP(0)) {
	to = toBB.PopFirstOneRightFromI9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 0], to);
	};
	}
	while (toBB.GetP(1)) {
	to = toBB.PopFirstOneLeftFromB9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 0], to);
	};
	}
	} while (false);
	}
	void Func1B(const Bitboard& target, const Bitboard& TRank8BB, PieceType haveHand[6], MoveStack* pMoveStackList, const int noKnightIdx)
	{
	Bitboard toBB = target & TRank8BB;
	Square to;
	do {
	while (toBB.GetP(0)) {
	to = toBB.PopFirstOneRightFromI9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
	};
	}
	while (toBB.GetP(1)) {
	to = toBB.PopFirstOneLeftFromB9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
	};
	}
	} while (false);
	}
	void Func1C(Bitboard& toBB, PieceType haveHand[6], MoveStack* pMoveStackList)
	{
	Square to;
	do {
	while (toBB.GetP(0)) {
	to = toBB.PopFirstOneRightFromI9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
	};
	}
	while (toBB.GetP(1)) {
	to = toBB.PopFirstOneLeftFromB9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
	};
	}
	} while (false);
	}
	void Func2A(const Bitboard& target, const Bitboard& TRank9BB, PieceType haveHand[6], MoveStack* pMoveStackList, const int noKnightLanceIdx)
	{
	Bitboard toBB = target & TRank9BB;
	Square to;
	do {
	while (toBB.GetP(0)) {
	to = toBB.PopFirstOneRightFromI9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 0], to);
	};
	}
	while (toBB.GetP(1)) {
	to = toBB.PopFirstOneLeftFromB9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 0], to);
	};
	}
	} while (false);
	}
	void Func2B(const Bitboard& target, const Bitboard& TRank8BB, PieceType haveHand[6], MoveStack* pMoveStackList, const int noKnightIdx)
	{
	Bitboard toBB = target & TRank8BB;
	Square to;
	do {
	while (toBB.GetP(0)) {
	to = toBB.PopFirstOneRightFromI9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
	};
	}
	while (toBB.GetP(1)) {
	to = toBB.PopFirstOneLeftFromB9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
	};
	}
	} while (false);
	}
	void Func2C(Bitboard& toBB, PieceType haveHand[6], MoveStack* pMoveStackList)
	{
	Square to;
	do {
	while (toBB.GetP(0)) {
	to = toBB.PopFirstOneRightFromI9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
	};
	}
	while (toBB.GetP(1)) {
	to = toBB.PopFirstOneLeftFromB9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
	};
	}
	} while (false);
	}
	void Func3A(const Bitboard& target, const Bitboard& TRank9BB, PieceType haveHand[6], MoveStack* pMoveStackList, const int noKnightLanceIdx)
	{
	Bitboard toBB = target & TRank9BB;
	Square to;
	do {
	while (toBB.GetP(0)) {
	to = toBB.PopFirstOneRightFromI9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 0], to);
	};
	}
	while (toBB.GetP(1)) {
	to = toBB.PopFirstOneLeftFromB9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 0], to);
	};
	}
	} while (false);
	}
	void Func3B(const Bitboard& target, const Bitboard& TRank8BB, PieceType haveHand[6], MoveStack* pMoveStackList, const int noKnightIdx)
	{
	Bitboard toBB = target & TRank8BB;
	Square to;
	do {
	while (toBB.GetP(0)) {
	to = toBB.PopFirstOneRightFromI9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
	};
	}
	while (toBB.GetP(1)) {
	to = toBB.PopFirstOneLeftFromB9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
	};
	}
	} while (false);
	}
	void Func3C(Bitboard& toBB, PieceType haveHand[6], MoveStack* pMoveStackList)
	{
	Square to;
	do {
	while (toBB.GetP(0)) {
	to = toBB.PopFirstOneRightFromI9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
	};
	}
	while (toBB.GetP(1)) {
	to = toBB.PopFirstOneLeftFromB9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
	};
	}
	} while (false);
	}
	void Func4A(const Bitboard& target, const Bitboard& TRank9BB, PieceType haveHand[6], MoveStack* pMoveStackList, const int noKnightLanceIdx)
	{
	Bitboard toBB = target & TRank9BB;
	Square to;
	do {
	while (toBB.GetP(0)) {
	to = toBB.PopFirstOneRightFromI9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 3], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 0], to);
	};
	}
	while (toBB.GetP(1)) {
	to = toBB.PopFirstOneLeftFromB9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 3], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 0], to);
	};
	}
	} while (false);
	}
	void Func4B(const Bitboard& target, const Bitboard& TRank8BB, PieceType haveHand[6], MoveStack* pMoveStackList, const int noKnightIdx)
	{
	Bitboard toBB = target & TRank8BB;
	Square to;
	do {
	while (toBB.GetP(0)) {
	to = toBB.PopFirstOneRightFromI9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 3], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
	};
	}
	while (toBB.GetP(1)) {
	to = toBB.PopFirstOneLeftFromB9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 3], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
	};
	}
	} while (false);
	}
	void Func4C(Bitboard& toBB, PieceType haveHand[6], MoveStack* pMoveStackList)
	{
	Square to;
	do {
	while (toBB.GetP(0)) {
	to = toBB.PopFirstOneRightFromI9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[3], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
	};
	}
	while (toBB.GetP(1)) {
	to = toBB.PopFirstOneLeftFromB9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[3], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
	};
	}
	} while (false);
	}
	void Func5B(const Bitboard& target, const Bitboard& TRank8BB, PieceType haveHand[6], MoveStack* pMoveStackList, const int noKnightIdx)
	{
	Bitboard toBB = target & TRank8BB;
	Square to;
	do {
	while (toBB.GetP(0)) {
	to = toBB.PopFirstOneRightFromI9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 4], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 3], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
	};
	}
	while (toBB.GetP(1)) {
	to = toBB.PopFirstOneLeftFromB9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 4], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 3], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
	};
	}
	} while (false);
	}
	void Func5C(Bitboard& toBB, PieceType haveHand[6], MoveStack* pMoveStackList)
	{
	Square to;
	do {
	while (toBB.GetP(0)) {
	to = toBB.PopFirstOneRightFromI9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[4], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[3], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
	};
	}
	while (toBB.GetP(1)) {
	to = toBB.PopFirstOneLeftFromB9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[4], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[3], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
	};
	}
	} while (false);
	}
	void Func6C(Bitboard& toBB, PieceType haveHand[6], MoveStack* pMoveStackList)
	{
	Square to;
	do {
	while (toBB.GetP(0)) {
	to = toBB.PopFirstOneRightFromI9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[5], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[4], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[3], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
	};
	}
	while (toBB.GetP(1)) {
	to = toBB.PopFirstOneLeftFromB9();
	{
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[5], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[4], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[3], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
	};
	}
	} while (false);
	}
	*/

	/*NG
	void Func0A()
	{
	// 桂馬、香車 以外の持ち駒がない。
	}
	void Func0B()
	{
	// 桂馬 以外の持ち駒がない。
	}
	void Func0C()
	{
	assert(false);// 最適化の為のダミー
	}
	void Func1A(const Bitboard& target, const Bitboard& TRank9BB, PieceType haveHand[6], MoveStack* pMoveStackList, const int noKnightLanceIdx)
	{
	Bitboard toBB = target & TRank9BB;
	Square to;
	FOREACH_BB(toBB, to, {
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 0], to);
	});
	}
	void Func1B(const Bitboard& target, const Bitboard& TRank8BB, PieceType haveHand[6], MoveStack* pMoveStackList, const int noKnightIdx)
	{
	Bitboard toBB = target & TRank8BB;
	Square to;
	FOREACH_BB(toBB, to, {
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
	});
	}
	void Func1C(Bitboard& toBB, PieceType haveHand[6], MoveStack* pMoveStackList)
	{
	Square to;
	FOREACH_BB(toBB, to, {
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
	});
	}
	void Func2A(const Bitboard& target, const Bitboard& TRank9BB, PieceType haveHand[6], MoveStack* pMoveStackList, const int noKnightLanceIdx)
	{
	Bitboard toBB = target & TRank9BB;
	Square to;
	FOREACH_BB(toBB, to, {
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 0], to);
	});
	}
	void Func2B(const Bitboard& target, const Bitboard& TRank8BB, PieceType haveHand[6], MoveStack* pMoveStackList, const int noKnightIdx)
	{
	Bitboard toBB = target & TRank8BB;
	Square to;
	FOREACH_BB(toBB, to, {
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
	});
	}
	void Func2C(Bitboard& toBB, PieceType haveHand[6], MoveStack* pMoveStackList)
	{
	Square to;
	FOREACH_BB(toBB, to, {
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
	});
	}
	void Func3A(const Bitboard& target, const Bitboard& TRank9BB, PieceType haveHand[6], MoveStack* pMoveStackList, const int noKnightLanceIdx)
	{
	Bitboard toBB = target & TRank9BB;
	Square to;
	FOREACH_BB(toBB, to, {
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 0], to);
	});
	}
	void Func3B(const Bitboard& target, const Bitboard& TRank8BB, PieceType haveHand[6], MoveStack* pMoveStackList, const int noKnightIdx)
	{
	Bitboard toBB = target & TRank8BB;
	Square to;
	FOREACH_BB(toBB, to, {
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
	});
	}
	void Func3C(Bitboard& toBB, PieceType haveHand[6], MoveStack* pMoveStackList)
	{
	Square to;
	FOREACH_BB(toBB, to, {
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
	});
	}
	void Func4A(const Bitboard& target, const Bitboard& TRank9BB, PieceType haveHand[6], MoveStack* pMoveStackList, const int noKnightLanceIdx)
	{
	Bitboard toBB = target & TRank9BB;
	Square to;
	FOREACH_BB(toBB, to, {
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 3], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + 0], to);
	});
	}
	void Func4B(const Bitboard& target, const Bitboard& TRank8BB, PieceType haveHand[6], MoveStack* pMoveStackList, const int noKnightIdx)
	{
	Bitboard toBB = target & TRank8BB;
	Square to;
	FOREACH_BB(toBB, to, {
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 3], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
	});
	}
	void Func4C(Bitboard& toBB, PieceType haveHand[6], MoveStack* pMoveStackList)
	{
	Square to;
	FOREACH_BB(toBB, to, {
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[3], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
	});
	}
	void Func5B(const Bitboard& target, const Bitboard& TRank8BB, PieceType haveHand[6], MoveStack* pMoveStackList, const int noKnightIdx)
	{
	Bitboard toBB = target & TRank8BB;
	Square to;
	FOREACH_BB(toBB, to, {
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 4], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 3], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + 0], to);
	});
	}
	void Func5C(Bitboard& toBB, PieceType haveHand[6], MoveStack* pMoveStackList)
	{
	Square to;
	FOREACH_BB(toBB, to, {
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[4], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[3], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
	});
	}
	void Func6C(Bitboard& toBB, PieceType haveHand[6], MoveStack* pMoveStackList)
	{
	Square to;
	FOREACH_BB(toBB, to, {
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[5], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[4], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[3], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
	(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
	});
	}
	*/


	// checkSq にある駒で王手されたとき、玉はその駒の利きの位置には移動できないので、移動できない位置を bannnedKingToBB に格納する。
	// 両王手のときには二度連続で呼ばれるため、= ではなく |= を使用している。
	// 最初に呼ばれたときは、bannedKingToBB == allZeroBB() である。
	// todo: FOECE_INLINE と template 省いてNPS比較
	//
	// 黒か白で関数を２つ用意しちゃダメなのかだぜ☆？（＾ｑ＾）？
	//
	static FORCE_INLINE void MakeBannedKingTo_them(
		Color THEM,
		Bitboard& bannedKingToBB,
		const Position& pos,
		const Square checkSq,
		const Square ksq
		) {
		switch (THEM)
		{
		case Black:
		{
			//
			// (^q^)黒か白かで最初に分けちゃダメなのかだぜ☆？
			//
			switch (pos.GetPiece(checkSq))
			{

				// (^q^)ビットが 0 のとき☆？
				//		case Empty: assert(false); break; // 最適化の為のダミー

				// (^q^)色が黒なら BPawn、そうでなければ WPawn のケースだぜ☆
			case BPawn:
				// (^q^)色が黒なら BKnight、そうでなければ WKnight のケースだぜ☆
			case BKnight:
				// 歩、桂馬で王手したときは、どこへ逃げても、その駒で取られることはない。
				// よって、ここでは何もしない。
				assert(
					pos.GetPiece(checkSq) == (THEM == Black ? BPawn : WPawn) ||
					pos.GetPiece(checkSq) == (THEM == Black ? BKnight : WKnight)
					);
				break;

			case BLance:
				bannedKingToBB |= g_lanceAttackBb.GetControllBbToEdge(THEM, checkSq);
				break;

			case BSilver:
				bannedKingToBB |= g_silverAttackBb.GetControllBb(THEM, checkSq);
				break;

			case BGold:

			case BProPawn:

			case BProLance:

			case BProKnight:

			case BProSilver:
				bannedKingToBB |= g_goldAttackBb.GetControllBb(THEM, checkSq);
				break;

			case BBishop:
				bannedKingToBB |= g_bishopAttackBb.GetControllBbToEdge(checkSq);
				break;

			case BHorse:
				bannedKingToBB |= g_horseAttackBb.GetControllBbToEdge(checkSq);
				break;

			case BRook:
				bannedKingToBB |= g_rookAttackBb.GetControllBbToEdge(checkSq);
				break;

			case BDragon:
				if (g_squareRelation.GetSquareRelation(checkSq, ksq) & DirecDiag) {
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
		break;
		default://White
		{
			//
			// (^q^)黒か白かで最初に分けちゃダメなのかだぜ☆？
			//
			switch (pos.GetPiece(checkSq))
			{

				// (^q^)ビットが 0 のとき☆？
				//		case Empty: assert(false); break; // 最適化の為のダミー

				// (^q^)色が黒なら BPawn、そうでなければ WPawn のケースだぜ☆
			case WPawn:
				// (^q^)色が黒なら BKnight、そうでなければ WKnight のケースだぜ☆
			case WKnight:
				// 歩、桂馬で王手したときは、どこへ逃げても、その駒で取られることはない。
				// よって、ここでは何もしない。
				assert(
					pos.GetPiece(checkSq) == (THEM == Black ? BPawn : WPawn) ||
					pos.GetPiece(checkSq) == (THEM == Black ? BKnight : WKnight)
					);
				break;

			case WLance:
				bannedKingToBB |= g_lanceAttackBb.GetControllBbToEdge(THEM, checkSq);
				break;

			case WSilver:
				bannedKingToBB |= g_silverAttackBb.GetControllBb(THEM, checkSq);
				break;

			case WGold:

			case WProPawn:

			case WProLance:

			case WProKnight:

			case WProSilver:
				bannedKingToBB |= g_goldAttackBb.GetControllBb(THEM, checkSq);
				break;

			case WBishop:
				bannedKingToBB |= g_bishopAttackBb.GetControllBbToEdge(checkSq);
				break;

			case WHorse:
				bannedKingToBB |= g_horseAttackBb.GetControllBbToEdge(checkSq);
				break;

			case WRook:
				bannedKingToBB |= g_rookAttackBb.GetControllBbToEdge(checkSq);
				break;

			case WDragon:
				if (g_squareRelation.GetSquareRelation(checkSq, ksq) & DirecDiag) {
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
		break;
		}
	}

};









// 部分特殊化
// 駒打ち生成
struct MoveGenerator_Drop_us {
	FORCE_INLINE MoveStack* operator () (
		Color US,
		MoveStack* pMovestack,
		const Position& pos
		) {
		const Bitboard target = pos.GetEmptyBB();
		pMovestack = MoveGenerator::GenerateDropMoves_color(US, pMovestack, pos, target);
		return pMovestack;
	}
};




// 金, 成り金、馬、竜の指し手生成
struct MoveGeneratorPiece_mt_pt_us_all {

	FORCE_INLINE MoveStack* operator () (
		MoveType MT,
		PieceType PT,
		Color US,
		bool ALL,
		MoveStack* moveStackList,
		const Position& pos,
		const Bitboard& target,
		const Square ksq
		) {

		switch (PT) {
		case N01_Pawn:	// 歩の場合
		{
			// Txxx は先手、後手の情報を吸収した変数。数字は先手に合わせている。
			const Rank TRank6 = (US == Black ? Rank6 : Rank4);
			const Bitboard TRank789BB = g_inFrontMaskBb.GetInFrontMask(US, TRank6);
			const SquareDelta TDeltaS = (US == Black ? DeltaS : DeltaN);

			Bitboard toBB = pos.GetBbOf(N01_Pawn, US).PawnAttack(US) & target;

			// 成り
			if (MT != NonCaptureMinusPro) {
				Bitboard toOn789BB = toBB & TRank789BB;
				if (toOn789BB.Exists1Bit()) {

					toBB.AndEqualNot(TRank789BB);

					Square to;

					FOREACH_BB(toOn789BB, to, {
						const Square from = to + TDeltaS;
					(*moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2_mt(MT, N01_Pawn, from, to, pos);
					if (MT == NonEvasion || ALL) {
						const Rank TRank9 = (US == Black ? Rank9 : Rank1);
						if (UtilSquare::ToRank(to) != TRank9) {
							(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove_mt(MT, N01_Pawn, from, to, pos);
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
			(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove_mt(MT, N01_Pawn, from, to, pos);
			});

			return moveStackList;
		}
		break;
		case N02_Lance:// 香車の場合
		{
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
							(*moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2_mt(MT, N02_Lance, from, to, pos);
							if (MT == NonEvasion || ALL) {
								if (UtilSquare::IsBehind<Rank9, Rank1>(US, UtilSquare::ToRank(to))) // 1段目の不成は省く
									(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove_mt(MT, N02_Lance, from, to, pos);
							}
							else if (MT != NonCapture && MT != NonCaptureMinusPro) { // 駒を取らない3段目の不成を省く
								if (UtilSquare::IsBehind<Rank8, Rank2>(US, UtilSquare::ToRank(to))) // 2段目の不成を省く
									(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove_mt(MT, N02_Lance, from, to, pos);
							}
						}
						else
							(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove_mt(MT, N02_Lance, from, to, pos);
					}
					// 駒取り対象は必ず一つ以下なので、loop は不要。最適化で do while が無くなると良い。
				} while (!(MT == Capture || MT == CapturePlusPro) && toBB.Exists1Bit());
			}
			return moveStackList;
		}
		break;
		case N03_Knight:// 桂馬の場合
		{
			Bitboard fromBB = pos.GetBbOf(N03_Knight, US);
			while (fromBB.Exists1Bit()) {
				const Square from = fromBB.PopFirstOneFromI9();
				Bitboard toBB = g_ptKnight.GetAttacks2From(g_nullBitboard, US, from) & target;
				while (toBB.Exists1Bit()) {
					const Square to = toBB.PopFirstOneFromI9();
					const bool toCanPromote = UtilSquare::CanPromote(US, UtilSquare::ToRank(to));
					if (toCanPromote) {
						(*moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2_mt(MT, N03_Knight, from, to, pos);
						if (UtilSquare::IsBehind<Rank8, Rank2>(US, UtilSquare::ToRank(to))) // 1, 2段目の不成は省く
							(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove_mt(MT, N03_Knight, from, to, pos);
					}
					else
						(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove_mt(MT, N03_Knight, from, to, pos);
				}
			}
			return moveStackList;
		}
		break;
		case N04_Silver:// 銀の場合
		{
			Bitboard fromBB = pos.GetBbOf(N04_Silver, US);
			while (fromBB.Exists1Bit()) {
				const Square from = fromBB.PopFirstOneFromI9();
				const bool fromCanPromote = UtilSquare::CanPromote(US, UtilSquare::ToRank(from));
				Bitboard toBB = g_ptSilver.GetAttacks2From(g_nullBitboard, US, from) & target;
				while (toBB.Exists1Bit()) {
					const Square to = toBB.PopFirstOneFromI9();
					const bool toCanPromote = UtilSquare::CanPromote(US, UtilSquare::ToRank(to));
					if (fromCanPromote | toCanPromote)
						(*moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2_mt(MT, N04_Silver, from, to, pos);
					(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove_mt(MT, N04_Silver, from, to, pos);
				}
			}
			return moveStackList;
		}
		break;
		case N05_Bishop:
		{
			return MoveGenerator::GenerateBishopOrRookMoves_mt_pt_us_all(MT, N05_Bishop, US, ALL, moveStackList, pos, target, ksq);
		}
		break;
		case N06_Rook:
		{
			return MoveGenerator::GenerateBishopOrRookMoves_mt_pt_us_all(MT, N06_Rook, US, ALL, moveStackList, pos, target, ksq);
		}
		break;
		// 07 なくて
		case N08_King:
		{
			// 玉の場合
			// 必ず盤上に 1 枚だけあることを前提にすることで、while ループを 1 つ無くして高速化している。
			const Square from = pos.GetKingSquare(US);
			Bitboard toBB = g_ptKing.GetAttacks2From(g_nullBitboard, US, from) & target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove_mt(MT, N08_King, from, to, pos);
			}
			return moveStackList;
		}
		break;
		case N16_GoldHorseDragon:
		{
			assert(PT == N16_GoldHorseDragon, "");
			// 金、成金、馬、竜のbitboardをまとめて扱う。
			Bitboard fromBB = (pos.GetGoldsBB() | pos.GetBbOf(N13_Horse, N14_Dragon)) & pos.GetBbOf(US);
			while (fromBB.Exists1Bit()) {
				const Square from = fromBB.PopFirstOneFromI9();
				// from にある駒の種類を判別
				const PieceType pt = UtilPiece::ToPieceType(pos.GetPiece(from));
				Bitboard toBB = UtilAttack::GetAttacksFrom(pt, US, from, pos.GetOccupiedBB()) & target;
				while (toBB.Exists1Bit()) {
					const Square to = toBB.PopFirstOneFromI9();
					(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove_mt(MT, pt, from, to, pos);
				}
			}
			return moveStackList;
		}
		break;
		}
	}
};




// 部分特殊化
// 王手が掛かっていないときの指し手生成
// これには、玉が相手駒の利きのある地点に移動する自殺手と、pin されている駒を動かす自殺手を含む。
// ここで生成した手は pseudo legal
struct MoveGenerator_NonEvasion_us {
	/*FORCE_INLINE*/ MoveStack* operator () (
		Color US,
		MoveStack* pMovestack,
		const Position& pos
		) {
		Bitboard target = pos.GetEmptyBB();

		pMovestack = MoveGenerator::GenerateDropMoves_color(US, pMovestack, pos, target);

		target |= pos.GetBbOf(UtilColor::OppositeColor(US));
		const Square ksq = pos.GetKingSquare(UtilColor::OppositeColor(US));

		pMovestack = MoveGeneratorPiece_mt_pt_us_all()(NonEvasion, N01_Pawn, US, false, pMovestack, pos, target, ksq);
		pMovestack = MoveGeneratorPiece_mt_pt_us_all()(NonEvasion, N02_Lance, US, false, pMovestack, pos, target, ksq);
		pMovestack = MoveGeneratorPiece_mt_pt_us_all()(NonEvasion, N03_Knight, US, false, pMovestack, pos, target, ksq);
		pMovestack = MoveGeneratorPiece_mt_pt_us_all()(NonEvasion, N04_Silver, US, false, pMovestack, pos, target, ksq);
		pMovestack = MoveGeneratorPiece_mt_pt_us_all()(NonEvasion, N05_Bishop, US, false, pMovestack, pos, target, ksq);
		pMovestack = MoveGeneratorPiece_mt_pt_us_all()(NonEvasion, N06_Rook, US, false, pMovestack, pos, target, ksq);
		pMovestack = MoveGeneratorPiece_mt_pt_us_all()(NonEvasion, N16_GoldHorseDragon, US, false, pMovestack, pos, target, ksq);
		pMovestack = MoveGeneratorPiece_mt_pt_us_all()(NonEvasion, N08_King, US, false, pMovestack, pos, target, ksq);

		return pMovestack;
	}
};




// 指し手生成 functor
// テンプレート引数が複数あり、部分特殊化したかったので、関数ではなく、struct にした。
// ALL == true のとき、歩、飛、角の不成、香の2段目の不成、香の3段目の駒を取らない不成も生成する。
struct MoveGenerator_mt_us {
	MoveStack* operator () (
		MoveType MT,
		Color US,
		MoveStack* moveStackList,
		const Position& pos
		) {
		bool ALL = false;
		assert(MT == Capture || MT == NonCapture || MT == CapturePlusPro || MT == NonCaptureMinusPro, "");
		// Txxx は先手、後手の情報を吸収した変数。数字は先手に合わせている。
		const Rank TRank6 = (US == Black ? Rank6 : Rank4);
		const Rank TRank7 = (US == Black ? Rank7 : Rank3);
		const Rank TRank8 = (US == Black ? Rank8 : Rank2);
		const Bitboard TRank789BB = g_inFrontMaskBb.GetInFrontMask(US, TRank6);
		const Bitboard TRank1_6BB = g_inFrontMaskBb.GetInFrontMask(UtilColor::OppositeColor(US), TRank7);
		const Bitboard TRank1_7BB = g_inFrontMaskBb.GetInFrontMask(UtilColor::OppositeColor(US), TRank8);

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

		moveStackList = MoveGeneratorPiece_mt_pt_us_all()(MT, N01_Pawn, US, ALL, moveStackList, pos, targetPawn, ksq);
		moveStackList = MoveGeneratorPiece_mt_pt_us_all()(MT, N02_Lance, US, ALL, moveStackList, pos, targetOther, ksq);
		moveStackList = MoveGeneratorPiece_mt_pt_us_all()(MT, N03_Knight, US, ALL, moveStackList, pos, targetOther, ksq);
		moveStackList = MoveGeneratorPiece_mt_pt_us_all()(MT, N04_Silver, US, ALL, moveStackList, pos, targetOther, ksq);
		moveStackList = MoveGeneratorPiece_mt_pt_us_all()(MT, N05_Bishop, US, ALL, moveStackList, pos, targetOther, ksq);
		moveStackList = MoveGeneratorPiece_mt_pt_us_all()(MT, N06_Rook, US, ALL, moveStackList, pos, targetOther, ksq);
		moveStackList = MoveGeneratorPiece_mt_pt_us_all()(MT, PieceType::N16_GoldHorseDragon, US, ALL, moveStackList, pos, targetOther, ksq);
		moveStackList = MoveGeneratorPiece_mt_pt_us_all()(MT, N08_King, US, ALL, moveStackList, pos, targetOther, ksq);

		return moveStackList;
	}
};




// 部分特殊化
// 連続王手の千日手以外の反則手を排除した合法手生成
// そんなに速度が要求されるところでは呼ばない。
struct MoveGenerator_Legal_us {
	FORCE_INLINE MoveStack* operator () (
		Color US,
		MoveStack* moveStackList,
		const Position& pos
		) {
		MoveStack* curr = moveStackList;
		const Bitboard pinned = pos.GetPinnedBB();

		moveStackList = pos.InCheck() ?
			MoveGenerator_mt_us()(Evasion, US, moveStackList, pos) :
			MoveGenerator_mt_us()(NonEvasion, US, moveStackList, pos);

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
// 王手回避生成
// 王手をしている駒による王手は避けるが、
// 玉の移動先に敵の利きがある場合と、pinされている味方の駒を動かした場合、非合法手を生成する。
// そのため、pseudo legal である。
struct MoveGenerator_Evasion_us_all {
	/*FORCE_INLINE*/ MoveStack* operator () (
		Color US,
		bool ALL,
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

			// 白か黒かによって分けている☆
			MoveGenerator::MakeBannedKingTo_them(Them, bannedKingToBB, pos, checkSq, ksq);

		} while (bb.Exists1Bit());


		// 玉が移動出来る移動先を格納。
		bb = bannedKingToBB.NotThisAnd(pos.GetBbOf(US).NotThisAnd(g_kingAttackBb.GetControllBb(ksq)));
		while (bb.Exists1Bit()) {
			const Square to = bb.PopFirstOneFromI9();
			// 移動先に相手駒の利きがあるか調べずに指し手を生成する。
			// attackersTo() が重いので、movePicker か search で合法手か調べる。
			(*pMovestack++).m_move = g_makePromoteMove.MakeNonPromoteMove_mt(Capture, N08_King, ksq, to, pos);
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
		pMovestack = MoveGeneratorPiece_mt_pt_us_all()(Evasion, N01_Pawn, US, ALL, pMovestack, pos, target2, ksq);
		pMovestack = MoveGeneratorPiece_mt_pt_us_all()(Evasion, N02_Lance, US, ALL, pMovestack, pos, target2, ksq);
		pMovestack = MoveGeneratorPiece_mt_pt_us_all()(Evasion, N03_Knight, US, ALL, pMovestack, pos, target2, ksq);
		pMovestack = MoveGeneratorPiece_mt_pt_us_all()(Evasion, N04_Silver, US, ALL, pMovestack, pos, target2, ksq);
		pMovestack = MoveGeneratorPiece_mt_pt_us_all()(Evasion, N05_Bishop, US, ALL, pMovestack, pos, target2, ksq);
		pMovestack = MoveGeneratorPiece_mt_pt_us_all()(Evasion, N06_Rook, US, ALL, pMovestack, pos, target2, ksq);
		pMovestack = MoveGeneratorPiece_mt_pt_us_all()(Evasion, N16_GoldHorseDragon, US, ALL, pMovestack, pos, target2, ksq);

		if (target1.Exists1Bit()) {
			pMovestack = MoveGenerator::GenerateDropMoves_color(US, pMovestack, pos, target1);
		}

		return pMovestack;
	}
};




// 部分特殊化
// Evasion のときに歩、飛、角と、香の2段目の不成も生成する。
struct MoveGenerator_LegalAll_us {
	FORCE_INLINE MoveStack* operator () (
		Color US,
		MoveStack* moveStackList,
		const Position& pos
		) {
		MoveStack* curr = moveStackList;
		const Bitboard pinned = pos.GetPinnedBB();

		moveStackList = pos.InCheck() ?
			MoveGenerator_Evasion_us_all()(US, true, moveStackList, pos) :
			MoveGenerator_mt_us()(NonEvasion, US, moveStackList, pos);

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




class MoveGenerator200 {
public:
public:
	// 定義
	static MoveStack* GenerateMoves_mt3(
		MoveType MT,
		MoveStack* moveStackList,
		const Position& pos
		) {
		switch (MT)
		{
		case Capture:
			return (
				pos.GetTurn() == Black ?
				MoveGenerator_mt_us()(Capture, Black, moveStackList, pos) :
				MoveGenerator_mt_us()(Capture, White, moveStackList, pos));
		case NonCapture:
			return (
				pos.GetTurn() == Black ?
				MoveGenerator_mt_us()(NonCapture, Black, moveStackList, pos) :
				MoveGenerator_mt_us()(NonCapture, White, moveStackList, pos));
		case Drop:
			return (
				pos.GetTurn() == Black ?
				MoveGenerator_mt_us()(Drop, Black, moveStackList, pos) :
				MoveGenerator_mt_us()(Drop, White, moveStackList, pos));
		case CapturePlusPro:
			return (
				pos.GetTurn() == Black ?
				MoveGenerator_mt_us()(CapturePlusPro, Black, moveStackList, pos) :
				MoveGenerator_mt_us()(CapturePlusPro, White, moveStackList, pos));
		case NonCaptureMinusPro:
			return (
				pos.GetTurn() == Black ?
				MoveGenerator_mt_us()(NonCaptureMinusPro, Black, moveStackList, pos) :
				MoveGenerator_mt_us()(NonCaptureMinusPro, White, moveStackList, pos));
		case Recapture:
			return (
				pos.GetTurn() == Black ?
				MoveGenerator_mt_us()(Recapture, Black, moveStackList, pos) :
				MoveGenerator_mt_us()(Recapture, White, moveStackList, pos));
		case Evasion:
			return (
				pos.GetTurn() == Black ?
				MoveGenerator_mt_us()(MT, Black, moveStackList, pos) :
				MoveGenerator_mt_us()(MT, White, moveStackList, pos));
		case NonEvasion:
			return (
				pos.GetTurn() == Black ?
				MoveGenerator_mt_us()(NonEvasion, Black, moveStackList, pos) :
				MoveGenerator_mt_us()(NonEvasion, White, moveStackList, pos));
		case Legal:
			return (
				pos.GetTurn() == Black ?
				MoveGenerator_mt_us()(Legal, Black, moveStackList, pos) :
				MoveGenerator_mt_us()(Legal, White, moveStackList, pos));
		case LegalAll:
			return (
				pos.GetTurn() == Black ?
				MoveGenerator_mt_us()(LegalAll, Black, moveStackList, pos) :
				MoveGenerator_mt_us()(LegalAll, White, moveStackList, pos));
		case MoveTypeNone:
			return (
				pos.GetTurn() == Black ?
				MoveGenerator_mt_us()(MoveTypeNone, Black, moveStackList, pos) :
				MoveGenerator_mt_us()(MoveTypeNone, White, moveStackList, pos));
		default:
			UNREACHABLE;
		}
	}


public://private: ほんとはプライベートにしたい☆名前空間を使ってもいい。

	   // pin は省かない。
	static FORCE_INLINE MoveStack* GenerateRecaptureMoves(
		MoveStack* moveStackList, const Position& pos, const Square to, const Color us
		) {
		Bitboard fromBB = pos.GetAttackersTo(us, to, pos.GetOccupiedBB());
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			const PieceType pieceType = UtilPiece::ToPieceType(pos.GetPiece(from));

			// TODO: 配列のリミットチェックをしてないぜ☆（＾ｑ＾）
			g_ptArray[pieceType]->Generate2RecaptureMoves(moveStackList, pos, from, to, us);
		}
		return moveStackList;
	}

public:
	// 定義
	static MoveStack* GenerateMoves_mt4(
		MoveType MT,
		MoveStack* moveStackList,
		const Position& pos,
		const Square to
		) {
		return MoveGenerator200::GenerateRecaptureMoves(moveStackList, pos, to, pos.GetTurn());
	}

};

/*
// 明示的なインスタンス化
// これが無いと、他のファイルから呼んだ時に、
// 実体が無いためにリンクエラーになる。
// ちなみに、特殊化されたテンプレート関数は、明示的なインスタンス化の必要はない。
// 実装を cpp に置くことで、コンパイル時間の短縮が出来る。
//template MoveStack* generateMoves<Capture           >(MoveStack* moveStackList, const Position& pos);
//template MoveStack* generateMoves<NonCapture        >(MoveStack* moveStackList, const Position& pos);
//template MoveStack* generateMoves_mt(MoveType MT, MoveStack* moveStackList, const Position& pos);
//Drop,CapturePlusPro,NonCaptureMinusPro,Evasion,NonEvasion,Legal,(LegalAll),Recapture
//template MoveStack* generateMoves<    >(MoveStack* moveStackList, const Position& pos);
//template MoveStack* generateMoves<>(MoveStack* moveStackList, const Position& pos);
//template MoveStack* generateMoves<           >(MoveStack* moveStackList, const Position& pos);
//template MoveStack* generateMoves<        >(MoveStack* moveStackList, const Position& pos);
//template MoveStack* generateMoves<             >(MoveStack* moveStackList, const Position& pos);
////#if !defined NDEBUG || defined LEARN
////template MoveStack* generateMoves<LegalAll          >(MoveStack* pMovestack, const Position& GetPos);
////#endif
//template MoveStack* generateMoves<Recapture         >(MoveStack* moveStackList, const Position& pos, const Square to);
*/
