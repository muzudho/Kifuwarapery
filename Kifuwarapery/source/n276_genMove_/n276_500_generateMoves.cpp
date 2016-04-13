#include "../../header/n105_color___/n105_500_utilColor.hpp"
#include "../../header/n110_square__/n110_250_squareDelta.hpp"
#include "../../header/n110_square__/n110_400_squareRelation.hpp"
#include "../../header/n112_pieceTyp/n112_050_pieceType.hpp"
#include "../../header/n160_board___/n160_102_FileMaskBb.hpp"
#include "../../header/n160_board___/n160_106_inFrontMaskBb.hpp"
#include "../../header/n160_board___/n160_230_setMaskBb.hpp"
#include "../../header/n160_board___/n160_600_bitboardAll.hpp"
#include "../../header/n165_movStack/n165_500_moveStack.hpp"
#include "../../header/n220_position/n220_640_utilAttack.hpp"
#include "../../header/n220_position/n220_650_position.hpp"
#include "../../header/n220_position/n220_670_makePromoteMove.hpp"
#include "../../header/n230_pieceTyp/n230_070_ptAbstract.hpp"
#include "../../header/n230_pieceTyp/n230_100_ptOccupied.hpp"
#include "../../header/n230_pieceTyp/n230_110_ptPawn.hpp"
#include "../../header/n230_pieceTyp/n230_120_ptLance.hpp"
#include "../../header/n230_pieceTyp/n230_130_ptKnight.hpp"
#include "../../header/n230_pieceTyp/n230_140_ptSilver.hpp"
#include "../../header/n230_pieceTyp/n230_150_ptBishop.hpp"
#include "../../header/n230_pieceTyp/n230_160_ptRook.hpp"
#include "../../header/n230_pieceTyp/n230_170_ptGold.hpp"
#include "../../header/n230_pieceTyp/n230_180_ptKing.hpp"
#include "../../header/n230_pieceTyp/n230_190_ptProPawn.hpp"
#include "../../header/n230_pieceTyp/n230_200_ptProLance.hpp"
#include "../../header/n230_pieceTyp/n230_210_ptProKnight.hpp"
#include "../../header/n230_pieceTyp/n230_220_ptProSilver.hpp"
#include "../../header/n230_pieceTyp/n230_230_ptHorse.hpp"
#include "../../header/n230_pieceTyp/n230_240_ptDragon.hpp"
#include "../../header/n230_pieceTyp/n230_500_ptArray.hpp"


extern const InFrontMaskBb g_inFrontMaskBb;
extern SquareRelation g_squareRelation;


namespace {
	// 角, 飛車の場合
	template <MoveType MT, PieceType PT, Color US, bool ALL>
	FORCE_INLINE MoveStack* generateBishopOrRookMoves(
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
	template <Color US>
	MoveStack* generateDropMoves(
		MoveStack* moveStackList, const Position& pos, const Bitboard& target
	) {
		const Hand hand = pos.GetHand(US);
		// まず、歩に対して指し手を生成
		if (hand.Exists<HPawn>()) {
			Bitboard toBB = target;
			// 一段目には打てない
			const Rank TRank9 = (US == Black ? Rank9 : Rank1);
			toBB.AndEqualNot(g_rankMaskBb.GetRankMask<TRank9>());

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
				if (g_setMaskBb.IsSet(&toBB,pawnDropCheckSquare) && pos.GetPiece(pawnDropCheckSquare) == Empty) {
					if (!pos.IsPawnDropCheckMate(US, pawnDropCheckSquare)) {
						// ここで clearBit だけして MakeMove しないことも出来る。
						// 指し手が生成される順番が変わり、王手が先に生成されるが、後で問題にならないか?
						(*moveStackList++).m_move = UtilMove::MakeDropMove(N01_Pawn, pawnDropCheckSquare);
					}
					g_setMaskBb.XorBit(&toBB,pawnDropCheckSquare);
				}
			}

			Square to;
			FOREACH_BB(toBB, to, {
				(*moveStackList++).m_move = UtilMove::MakeDropMove(N01_Pawn, to);
			});
		}

		// 歩 以外の駒を持っているか
		if (hand.ExceptPawnExists()) {
			PieceType haveHand[6]; // 歩以外の持ち駒。vector 使いたいけど、速度を求めるので使わない。
			int haveHandNum = 0; // 持ち駒の駒の種類の数

								 // 桂馬、香車、それ以外の順番で格納する。(駒を打てる位置が限定的な順)
			if (hand.Exists<HKnight>()) { haveHand[haveHandNum++] = N03_Knight; }
			const int noKnightIdx = haveHandNum; // 桂馬を除く駒でループするときのループの初期値
			if (hand.Exists<HLance >()) { haveHand[haveHandNum++] = N02_Lance; }
			const int noKnightLanceIdx = haveHandNum; // 桂馬, 香車を除く駒でループするときのループの初期値
			if (hand.Exists<HSilver>()) { haveHand[haveHandNum++] = N04_Silver; }
			if (hand.Exists<HGold  >()) { haveHand[haveHandNum++] = N07_Gold; }
			if (hand.Exists<HBishop>()) { haveHand[haveHandNum++] = N05_Bishop; }
			if (hand.Exists<HRook  >()) { haveHand[haveHandNum++] = N06_Rook; }

			const Rank TRank8 = (US == Black ? Rank8 : Rank2);
			const Rank TRank9 = (US == Black ? Rank9 : Rank1);
			const Bitboard TRank8BB = g_rankMaskBb.GetRankMask<TRank8>();
			const Bitboard TRank9BB = g_rankMaskBb.GetRankMask<TRank9>();

			Bitboard toBB;
			Square to;
			// 桂馬、香車 以外の持ち駒があれば、
			// 一段目に対して、桂馬、香車以外の指し手を生成。
			switch (haveHandNum - noKnightLanceIdx) {
			case 0: break; // 桂馬、香車 以外の持ち駒がない。
			case 1: toBB = target & TRank9BB; FOREACH_BB(toBB, to, { Unroller<1>()([&](const int i) { (*moveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + i], to); }); }); break;
			case 2: toBB = target & TRank9BB; FOREACH_BB(toBB, to, { Unroller<2>()([&](const int i) { (*moveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + i], to); }); }); break;
			case 3: toBB = target & TRank9BB; FOREACH_BB(toBB, to, { Unroller<3>()([&](const int i) { (*moveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + i], to); }); }); break;
			case 4: toBB = target & TRank9BB; FOREACH_BB(toBB, to, { Unroller<4>()([&](const int i) { (*moveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + i], to); }); }); break;
			default: UNREACHABLE;
			}

			// 桂馬以外の持ち駒があれば、
			// 二段目に対して、桂馬以外の指し手を生成。
			switch (haveHandNum - noKnightIdx) {
			case 0: break; // 桂馬 以外の持ち駒がない。
			case 1: toBB = target & TRank8BB; FOREACH_BB(toBB, to, { Unroller<1>()([&](const int i) { (*moveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + i], to); }); }); break;
			case 2: toBB = target & TRank8BB; FOREACH_BB(toBB, to, { Unroller<2>()([&](const int i) { (*moveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + i], to); }); }); break;
			case 3: toBB = target & TRank8BB; FOREACH_BB(toBB, to, { Unroller<3>()([&](const int i) { (*moveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + i], to); }); }); break;
			case 4: toBB = target & TRank8BB; FOREACH_BB(toBB, to, { Unroller<4>()([&](const int i) { (*moveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + i], to); }); }); break;
			case 5: toBB = target & TRank8BB; FOREACH_BB(toBB, to, { Unroller<5>()([&](const int i) { (*moveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + i], to); }); }); break;
			default: UNREACHABLE;
			}

			// 一、二段目以外に対して、全ての持ち駒の指し手を生成。
			toBB = target & ~(TRank8BB | TRank9BB);
			switch (haveHandNum) {
			case 0: assert(false); break; // 最適化の為のダミー
			case 1: FOREACH_BB(toBB, to, { Unroller<1>()([&](const int i) { (*moveStackList++).m_move = UtilMove::MakeDropMove(haveHand[i], to); }); }); break;
			case 2: FOREACH_BB(toBB, to, { Unroller<2>()([&](const int i) { (*moveStackList++).m_move = UtilMove::MakeDropMove(haveHand[i], to); }); }); break;
			case 3: FOREACH_BB(toBB, to, { Unroller<3>()([&](const int i) { (*moveStackList++).m_move = UtilMove::MakeDropMove(haveHand[i], to); }); }); break;
			case 4: FOREACH_BB(toBB, to, { Unroller<4>()([&](const int i) { (*moveStackList++).m_move = UtilMove::MakeDropMove(haveHand[i], to); }); }); break;
			case 5: FOREACH_BB(toBB, to, { Unroller<5>()([&](const int i) { (*moveStackList++).m_move = UtilMove::MakeDropMove(haveHand[i], to); }); }); break;
			case 6: FOREACH_BB(toBB, to, { Unroller<6>()([&](const int i) { (*moveStackList++).m_move = UtilMove::MakeDropMove(haveHand[i], to); }); }); break;
			default: UNREACHABLE;
			}
		}

		return moveStackList;
	}

	// 金, 成り金、馬、竜の指し手生成
	template <MoveType MT, PieceType PT, Color US, bool ALL> struct GeneratePieceMoves {
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
	template <MoveType MT, Color US, bool ALL> struct GeneratePieceMoves<MT, N02_Lance, US, ALL> {
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
	template <MoveType MT, Color US, bool ALL> struct GeneratePieceMoves<MT, N03_Knight, US, ALL> {
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
	template <MoveType MT, Color US, bool ALL> struct GeneratePieceMoves<MT, N04_Silver, US, ALL> {
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
	template <MoveType MT, Color US, bool ALL> struct GeneratePieceMoves<MT, N05_Bishop, US, ALL> {
		FORCE_INLINE MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square ksq
		) {
			return generateBishopOrRookMoves<MT, N05_Bishop, US, ALL>(moveStackList, pos, target, ksq);
		}
	};
	template <MoveType MT, Color US, bool ALL> struct GeneratePieceMoves<MT, N06_Rook, US, ALL> {
		FORCE_INLINE MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square ksq
		) {
			return generateBishopOrRookMoves<MT, N06_Rook, US, ALL>(moveStackList, pos, target, ksq);
		}
	};
	// 玉の場合
	// 必ず盤上に 1 枚だけあることを前提にすることで、while ループを 1 つ無くして高速化している。
	template <MoveType MT, Color US, bool ALL> struct GeneratePieceMoves<MT, N08_King, US, ALL> {
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

	// pin は省かない。
	FORCE_INLINE MoveStack* generateRecaptureMoves(
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

	// 指し手生成 functor
	// テンプレート引数が複数あり、部分特殊化したかったので、関数ではなく、struct にした。
	// ALL == true のとき、歩、飛、角の不成、香の2段目の不成、香の3段目の駒を取らない不成も生成する。
	template <MoveType MT, Color US, bool ALL = false> struct GenerateMoves {
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
	template <Color US> struct GenerateMoves<Drop, US> {
		FORCE_INLINE MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos
		) {
			const Bitboard target = pos.GetEmptyBB();
			moveStackList = generateDropMoves<US>(moveStackList, pos, target);
			return moveStackList;
		}
	};

	// checkSq にある駒で王手されたとき、玉はその駒の利きの位置には移動できないので、移動できない位置を bannnedKingToBB に格納する。
	// 両王手のときには二度連続で呼ばれるため、= ではなく |= を使用している。
	// 最初に呼ばれたときは、bannedKingToBB == allZeroBB() である。
	// todo: FOECE_INLINE と template 省いてNPS比較
	template <Color THEM>
	FORCE_INLINE void makeBannedKingTo(
		Bitboard& bannedKingToBB, const Position& pos, const Square checkSq, const Square ksq
	){
		switch (pos.GetPiece(checkSq)) {
			//		case Empty: assert(false); break; // 最適化の為のダミー
		case (THEM == Black ? BPawn : WPawn) :
		case (THEM == Black ? BKnight : WKnight) :
			// 歩、桂馬で王手したときは、どこへ逃げても、その駒で取られることはない。
			// よって、ここでは何もしない。
			assert(
				pos.GetPiece(checkSq) == (THEM == Black ? BPawn : WPawn) ||
				pos.GetPiece(checkSq) == (THEM == Black ? BKnight : WKnight)
				);
			break;
		case (THEM == Black ? BLance : WLance) :
			bannedKingToBB |= g_lanceAttackBb.GetControllBbToEdge(THEM, checkSq);
			break;
		case (THEM == Black ? BSilver : WSilver) :
			bannedKingToBB |= g_silverAttackBb.GetControllBb(THEM, checkSq);
			break;
		case (THEM == Black ? BGold : WGold) :
		case (THEM == Black ? BProPawn : WProPawn) :
		case (THEM == Black ? BProLance : WProLance) :
		case (THEM == Black ? BProKnight : WProKnight) :
		case (THEM == Black ? BProSilver : WProSilver) :
			bannedKingToBB |= g_goldAttackBb.GetControllBb(THEM, checkSq);
			break;
		case (THEM == Black ? BBishop : WBishop) :
			bannedKingToBB |= g_bishopAttackBb.GetControllBbToEdge(checkSq);
			break;
		case (THEM == Black ? BHorse : WHorse) :
			bannedKingToBB |= g_horseAttackBb.GetControllBbToEdge(checkSq);
			break;
		case (THEM == Black ? BRook : WRook) :
			bannedKingToBB |= g_rookAttackBb.GetControllBbToEdge(checkSq);
			break;
		case (THEM == Black ? BDragon : WDragon) :
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

	// 部分特殊化
	// 王手回避生成
	// 王手をしている駒による王手は避けるが、
	// 玉の移動先に敵の利きがある場合と、pinされている味方の駒を動かした場合、非合法手を生成する。
	// そのため、pseudo legal である。
	template <Color US, bool ALL> struct GenerateMoves<Evasion, US, ALL> {
		/*FORCE_INLINE*/ MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos
		) {
			assert(pos.IsOK());
			assert(pos.InCheck());

			const Square ksq = pos.GetKingSquare(US);
			const Color Them = UtilColor::OppositeColor(US);
			const Bitboard checkers = pos.GetCheckersBB();
			Bitboard bb = checkers;
			Bitboard bannedKingToBB = Bitboard::CreateAllZeroBB();
			int checkersNum = 0;
			Square checkSq;

			// 玉が逃げられない位置の bitboard を生成する。
			// 絶対に王手が掛かっているので、while ではなく、do while
			do {
				checkSq = bb.PopFirstOneFromI9();
				assert(UtilPiece::ToColor(pos.GetPiece(checkSq)) == Them);
				++checkersNum;
				makeBannedKingTo<Them>(bannedKingToBB, pos, checkSq, ksq);
			} while (bb.Exists1Bit());

			// 玉が移動出来る移動先を格納。
			bb = bannedKingToBB.NotThisAnd(pos.GetBbOf(US).NotThisAnd(g_kingAttackBb.GetControllBb(ksq)));
			while (bb.Exists1Bit()) {
				const Square to = bb.PopFirstOneFromI9();
				// 移動先に相手駒の利きがあるか調べずに指し手を生成する。
				// attackersTo() が重いので、movePicker か search で合法手か調べる。
				(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<Capture>(N08_King, ksq, to, pos);
			}

			// 両王手なら、玉を移動するしか回避方法は無い。
			// 玉の移動は生成したので、ここで終了
			if (1 < checkersNum) {
				return moveStackList;
			}

			// 王手している駒を玉以外で取る手の生成。
			// pin されているかどうかは movePicker か search で調べる。
			const Bitboard target1 = g_betweenBb.GetBetweenBB(checkSq, ksq);
			const Bitboard target2 = target1 | checkers;
			moveStackList = GeneratePieceMoves<Evasion, N01_Pawn, US, ALL>()(moveStackList, pos, target2, ksq);
			moveStackList = GeneratePieceMoves<Evasion, N02_Lance, US, ALL>()(moveStackList, pos, target2, ksq);
			moveStackList = GeneratePieceMoves<Evasion, N03_Knight, US, ALL>()(moveStackList, pos, target2, ksq);
			moveStackList = GeneratePieceMoves<Evasion, N04_Silver, US, ALL>()(moveStackList, pos, target2, ksq);
			moveStackList = GeneratePieceMoves<Evasion, N05_Bishop, US, ALL>()(moveStackList, pos, target2, ksq);
			moveStackList = GeneratePieceMoves<Evasion, N06_Rook, US, ALL>()(moveStackList, pos, target2, ksq);
			moveStackList = GeneratePieceMoves<Evasion, N16_GoldHorseDragon, US, ALL>()(moveStackList, pos, target2, ksq);

			if (target1.Exists1Bit()) {
				moveStackList = generateDropMoves<US>(moveStackList, pos, target1);
			}

			return moveStackList;
		}
	};

	// 部分特殊化
	// 王手が掛かっていないときの指し手生成
	// これには、玉が相手駒の利きのある地点に移動する自殺手と、pin されている駒を動かす自殺手を含む。
	// ここで生成した手は pseudo legal
	template <Color US> struct GenerateMoves<NonEvasion, US> {
		/*FORCE_INLINE*/ MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos
		) {
			Bitboard target = pos.GetEmptyBB();

			moveStackList = generateDropMoves<US>(moveStackList, pos, target);
			target |= pos.GetBbOf(UtilColor::OppositeColor(US));
			const Square ksq = pos.GetKingSquare(UtilColor::OppositeColor(US));

			moveStackList = GeneratePieceMoves<NonEvasion, N01_Pawn, US, false>()(moveStackList, pos, target, ksq);
			moveStackList = GeneratePieceMoves<NonEvasion, N02_Lance, US, false>()(moveStackList, pos, target, ksq);
			moveStackList = GeneratePieceMoves<NonEvasion, N03_Knight, US, false>()(moveStackList, pos, target, ksq);
			moveStackList = GeneratePieceMoves<NonEvasion, N04_Silver, US, false>()(moveStackList, pos, target, ksq);
			moveStackList = GeneratePieceMoves<NonEvasion, N05_Bishop, US, false>()(moveStackList, pos, target, ksq);
			moveStackList = GeneratePieceMoves<NonEvasion, N06_Rook, US, false>()(moveStackList, pos, target, ksq);
			moveStackList = GeneratePieceMoves<NonEvasion, N16_GoldHorseDragon, US, false>()(moveStackList, pos, target, ksq);
			moveStackList = GeneratePieceMoves<NonEvasion, N08_King, US, false>()(moveStackList, pos, target, ksq);

			return moveStackList;
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
}

template <MoveType MT>
MoveStack* generateMoves(
	MoveStack* moveStackList, const Position& pos
) {
	return (pos.GetTurn() == Black ?
		GenerateMoves<MT, Black>()(moveStackList, pos) : GenerateMoves<MT, White>()(moveStackList, pos));
}

template <MoveType MT>
MoveStack* generateMoves(
	MoveStack* moveStackList, const Position& pos, const Square to
) {
	return generateRecaptureMoves(moveStackList, pos, to, pos.GetTurn());
}


// 明示的なインスタンス化
// これが無いと、他のファイルから呼んだ時に、
// 実体が無いためにリンクエラーになる。
// ちなみに、特殊化されたテンプレート関数は、明示的なインスタンス化の必要はない。
// 実装を cpp に置くことで、コンパイル時間の短縮が出来る。
//template MoveStack* generateMoves<Capture           >(MoveStack* moveStackList, const Position& pos);
//template MoveStack* generateMoves<NonCapture        >(MoveStack* moveStackList, const Position& pos);
template MoveStack* generateMoves<Drop              >(MoveStack* moveStackList, const Position& pos);
template MoveStack* generateMoves<CapturePlusPro    >(MoveStack* moveStackList, const Position& pos);
template MoveStack* generateMoves<NonCaptureMinusPro>(MoveStack* moveStackList, const Position& pos);
template MoveStack* generateMoves<Evasion           >(MoveStack* moveStackList, const Position& pos);
template MoveStack* generateMoves<NonEvasion        >(MoveStack* moveStackList, const Position& pos);
template MoveStack* generateMoves<Legal             >(MoveStack* moveStackList, const Position& pos);
#if !defined NDEBUG || defined LEARN
template MoveStack* generateMoves<LegalAll          >(MoveStack* moveStackList, const Position& GetPos);
#endif
template MoveStack* generateMoves<Recapture         >(MoveStack* moveStackList, const Position& pos, const Square to);
