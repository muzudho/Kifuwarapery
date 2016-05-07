#pragma once


#include "../n300_500_piece___/n300_500_070_pieceAbstract.hpp"
#include "../n300_500_piece___/n300_500_080_pieceNull.hpp"
#include "../n300_500_piece___/n300_500_100_pieceEmpty.hpp"
#include "../n300_500_piece___/n300_500_110_pieceBPawn.hpp"
#include "../n300_500_piece___/n300_500_120_pieceBLance.hpp"
#include "../n300_500_piece___/n300_500_130_pieceBKnight.hpp"
#include "../n300_500_piece___/n300_500_140_pieceBSilver.hpp"
#include "../n300_500_piece___/n300_500_150_pieceBBishop.hpp"
#include "../n300_500_piece___/n300_500_160_pieceBRook.hpp"
#include "../n300_500_piece___/n300_500_170_pieceBGold.hpp"
#include "../n300_500_piece___/n300_500_180_pieceBKing.hpp"
#include "../n300_500_piece___/n300_500_190_pieceBProPawn.hpp"
#include "../n300_500_piece___/n300_500_200_pieceBProLance.hpp"
#include "../n300_500_piece___/n300_500_210_pieceBProKnight.hpp"
#include "../n300_500_piece___/n300_500_220_pieceBProSilver.hpp"
#include "../n300_500_piece___/n300_500_230_pieceBHorse.hpp"
#include "../n300_500_piece___/n300_500_240_pieceBDragon.hpp"
#include "../n300_500_piece___/n300_500_270_pieceWPawn.hpp"
#include "../n300_500_piece___/n300_500_280_pieceWLance.hpp"
#include "../n300_500_piece___/n300_500_290_pieceWKnight.hpp"
#include "../n300_500_piece___/n300_500_300_pieceWSilver.hpp"
#include "../n300_500_piece___/n300_500_310_pieceWBishop.hpp"
#include "../n300_500_piece___/n300_500_320_pieceWRook.hpp"
#include "../n300_500_piece___/n300_500_330_pieceWGold.hpp"
#include "../n300_500_piece___/n300_500_340_pieceWKing.hpp"
#include "../n300_500_piece___/n300_500_350_pieceWProPawn.hpp"
#include "../n300_500_piece___/n300_500_360_pieceWProLance.hpp"
#include "../n300_500_piece___/n300_500_370_pieceWProKnight.hpp"
#include "../n300_500_piece___/n300_500_380_pieceWProSilver.hpp"
#include "../n300_500_piece___/n300_500_390_pieceWHorse.hpp"
#include "../n300_500_piece___/n300_500_400_pieceWDragon.hpp"
#include "../n300_500_piece___/n300_500_410_pieceNone.hpp"


class PieceArray {
public:

	// テンプレート関数の中で使うので、スタティックにするんだぜ☆（＾ｑ＾）
	static const PieceEmpty m_pieceEmpty;	//[0]
	static const PieceBPawn m_pieceBPawn;
	static const PieceBLance m_pieceBLance;
	static const PieceBKnight m_pieceBKnight;
	static const PieceBSilver m_pieceBSilver;
	static const PieceBBishop m_pieceBBishop;//[5]
	static const PieceBRook m_pieceBRook;
	static const PieceBGold m_pieceBGold;
	static const PieceBKing m_pieceBKing;
	static const PieceBProPawn m_pieceBProPawn;
	static const PieceBProLance m_pieceBProLance;//[10]
	static const PieceBProKnight m_pieceBProKnight;
	static const PieceBProSilver m_pieceBProSilver;
	static const PieceBHorse m_pieceBHorse;
	static const PieceBDragon m_pieceBDragon;//[14]
	static const PieceNull m_pieceNull;//[15],[16]
	static const PieceWPawn m_pieceWPawn;
	static const PieceWLance m_pieceWLance;
	static const PieceWKnight m_pieceWKnight;
	static const PieceWSilver m_pieceWSilver;//[20]
	static const PieceWBishop m_pieceWBishop;
	static const PieceWRook m_pieceWRook;
	static const PieceWGold m_pieceWGold;
	static const PieceWKing m_pieceWKing;
	static const PieceWProPawn m_pieceWProPawn;//[25]
	static const PieceWProLance m_pieceWProLance;
	static const PieceWProKnight m_pieceWProKnight;
	static const PieceWProSilver m_pieceWProSilver;
	static const PieceWHorse m_pieceWHorse;
	static const PieceWDragon m_pieceWDragon;//[30]
	static const PieceNone m_pieceNone;//[31]

	static const PieceAbstract* m_pieceAbstractArray[32];

};


extern PieceArray g_pieceArray;
