#pragma once


#include "../../n165_movStack/n165_300_moveType.hpp"
#include "../../n220_position/n220_650_position.hpp"
#include "../../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../../n300_moveGen_/n300_400_genMove_/n300_400_350_PieceMovesGenerator.hpp"
#include "../../n300_moveGen_/n300_400_genMove_/n300_400_750_dropMoveGenerator.hpp"
#include "../../n300_moveGen_/n300_500_piece___/n300_500_070_pieceAbstract.hpp"
#include "../../n300_moveGen_/n300_500_piece___/n300_500_500_pieceArray.hpp"
#include "n300_600_070_mtAbstract.hpp"


class MoveTypeEvasion : public MoveTypeAbstract {
public:
	// 部分特殊化
	// 王手回避生成
	// 王手をしている駒による王手は避けるが、
	// 玉の移動先に敵の利きがある場合と、pinされている味方の駒を動かした場合、非合法手を生成する。
	// そのため、pseudo legal である。
	MoveStack* GenerateMove(MoveStack* moveStackList, const Position& pos, bool all = false
		) const override {

		if (pos.GetTurn() == Color::Black)
		{
			moveStackList = MoveTypeEvasion::GENERATE_MOVE_<Color::Black, Color::White>(moveStackList, pos, all);
		}
		else
		{
			moveStackList = MoveTypeEvasion::GENERATE_MOVE_<Color::White, Color::Black>(moveStackList, pos, all);
		}

		return moveStackList;
	}

private:

	template<Color US, Color THEM>
	static inline MoveStack* GENERATE_MOVE_(
		MoveStack* moveStackList,
		const Position& pos,
		bool all = false
		) {

		assert(pos.IsOK());
		assert(pos.InCheck());

		// 玉の位置
		const Square ksq = pos.GetKingSquare(US);

		// 手番側の玉に攻撃を仕掛けている駒の位置☆
		const Bitboard checkers = pos.GetCheckersBB();

		Bitboard bb = checkers;
		Bitboard bannedKingToBB = Bitboard::CreateAllZeroBB();

		// 王手を仕掛けてきている駒の数☆
		int checkersNum = 0;

		Square checkSq;

		// 玉が逃げられない位置の bitboard を生成する。
		// 絶対に王手が掛かっているので、while ではなく、do while
		do {

			// 最初にビットが立っていたマス☆
			checkSq = bb.PopFirstOneFromI9();

			assert(ConvPiece::TO_COLOR10(pos.GetPiece(checkSq)) == THEM);

			// カウントアップ☆
			++checkersNum;


			// checkSq にある駒で王手されたとき、玉はその駒の利きの位置には移動できないので、
			// 移動できない位置を bannnedKingToBB に格納する。
			// 両王手のときには二度連続で呼ばれるため、= ではなく |= を使用している。
			// 最初に呼ばれたときは、bannedKingToBB == allZeroBB() である。
			// todo: FOECE_INLINE と template 省いてNPS比較
			g_pieceArray.m_pieceAbstractArray[pos.GetPiece(checkSq)]->MakeBanned2KingTo(bannedKingToBB, pos, checkSq, ksq);

		} while (bb.Exists1Bit()); // ビットが立っている限り☆

		//────────────────────────────────────────────────────────────────────────────────
		// 玉が移動出来る移動先を格納。
		//────────────────────────────────────────────────────────────────────────────────
		bb = bannedKingToBB.NotThisAnd(pos.GetBbOf10(US).NotThisAnd(g_kingAttackBb.GetControllBb(ksq)));
		while (bb.Exists1Bit()) {
			const Square to = bb.PopFirstOneFromI9();
			// 移動先に相手駒の利きがあるか調べずに指し手を生成する。
			// attackersTo() が重いので、ＭｏｖｅＰｉｃｋｅｒ か search で合法手か調べる。
			moveStackList->m_move = g_makePromoteMove.BuildCard_CaptureCategory(
				pos, g_PTKING_ONBOARD_AS_MOVE, ksq, to);
			moveStackList++;
		}

		// 両王手なら、玉を移動するしか回避方法は無い。
		// 玉の移動は生成したので、ここで終了
		if (1 < checkersNum) {
			return moveStackList;
		}


		//────────────────────────────────────────────────────────────────────────────────
		// 王手している駒を玉以外で取る手の生成。
		//────────────────────────────────────────────────────────────────────────────────
		// pin されているかどうかは ＭｏｖｅＰｉｃｋｅｒ か search で調べる。

		// 2点間 a,b を結ぶ線（２点 a,b は含まない）。
		const Bitboard segmentBits = g_betweenBb.GetBetweenBB(
			checkSq,	// 手番側の玉に王手を掛けていた駒のうち、最後のものか☆？（＾ｑ＾）
			ksq			// 手番側の玉の位置☆
			);//target1
		// 移動してほしい升。王手を仕掛けている駒の位置＋２点間の線分
		const Bitboard destinationBb = segmentBits | checkers; //target2

		// 王手回避を指定（意味はない）
		const PieceMoveEvent pmEvent(MoveType::N06_Evasion, all, pos, ksq);

		moveStackList = PieceMovesGenerator::BuildCards_N01_Pawn<US>(moveStackList, pmEvent, destinationBb);
		moveStackList = PieceMovesGenerator::BuildCards_N02_Lance<US>(moveStackList, pmEvent, destinationBb);
		moveStackList = PieceMovesGenerator::BuildCards_N03_Knight<US>(moveStackList, pmEvent, destinationBb);
		moveStackList = PieceMovesGenerator::BuildCards_N04_Silver<US>(moveStackList, pmEvent, destinationBb);
		moveStackList = PieceMovesGenerator::BuildCards_N05_Bishop<US>(moveStackList, pmEvent, destinationBb);
		moveStackList = PieceMovesGenerator::BuildCards_N06_Rook<US>(moveStackList, pmEvent, destinationBb);
		moveStackList = PieceMovesGenerator::BuildCards_N16_GoldHorseDragon<US>(moveStackList, pmEvent, destinationBb);

		if (segmentBits.Exists1Bit()) {
			moveStackList = g_dropMoveGenerator.BuildCards_Drop<US,THEM>(moveStackList, pos, segmentBits);
		}

		return moveStackList;
	}

};


