#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n110_square__/n110_400_squareRelation.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_110_silverAttackBb.hpp"
#include "../n160_board___/n160_120_bishopAttackBb.hpp"
#include "../n160_board___/n160_130_lanceAttackBb.hpp"
#include "../n160_board___/n160_140_goldAttackBb.hpp"
#include "../n160_board___/n160_150_rookAttackBb.hpp"
#include "../n160_board___/n160_200_horseAttackBb.hpp"
#include "../n160_board___/n160_210_dragonAttackBb.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n350_pieceTyp/n350_500_ptArray.hpp"


class BannedKingToMaker {
public:
	//*
	// checkSq にある駒で王手されたとき、玉はその駒の利きの位置には移動できないので、移動できない位置を bannnedKingToBB に格納する。
	// 両王手のときには二度連続で呼ばれるため、= ではなく |= を使用している。
	// 最初に呼ばれたときは、bannedKingToBB == allZeroBB() である。
	// todo: FOECE_INLINE と template 省いてNPS比較
	//
	// 黒か白で関数を２つ用意しちゃダメなのかだぜ☆？（＾ｑ＾）？
	//
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
			assert(pos.GetPiece(checkSq) == N01_BPawn || pos.GetPiece(checkSq) == N03_BKnight);
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
				bannedKingToBB |= PieceTypeArray::m_dragon.GetAttacks2From(pos.GetOccupiedBB(), Color::ColorNum, checkSq);
			}
			else {
				bannedKingToBB |= g_dragonAttackBb.GetControllBbToEdge(checkSq);
			}
			break;
		default:
			UNREACHABLE;
		}
	}
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
				bannedKingToBB |= PieceTypeArray::m_dragon.GetAttacks2From(pos.GetOccupiedBB(), Color::ColorNum, checkSq);
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

};