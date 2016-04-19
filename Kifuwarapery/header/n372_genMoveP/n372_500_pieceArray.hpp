#pragma once


#include "../n372_genMoveP/n372_070_PieceAbstract.hpp"
#include "../n372_genMoveP/n372_080_pieceNull.hpp"
#include "../n372_genMoveP/n372_190_pieceBProPawn.hpp"
#include "../n372_genMoveP/n372_100_pieceEmpty.hpp"
#include "../n372_genMoveP/n372_110_pieceBPawn.hpp"
#include "../n372_genMoveP/n372_120_pieceBLance.hpp"
#include "../n372_genMoveP/n372_130_pieceBKnight.hpp"
#include "../n372_genMoveP/n372_140_pieceBSilver.hpp"
#include "../n372_genMoveP/n372_150_pieceBBishop.hpp"
#include "../n372_genMoveP/n372_160_pieceBRook.hpp"
#include "../n372_genMoveP/n372_170_pieceBGold.hpp"
#include "../n372_genMoveP/n372_180_pieceBKing.hpp"
#include "../n372_genMoveP/n372_190_pieceBProPawn.hpp"
#include "../n372_genMoveP/n372_200_pieceBProLance.hpp"
#include "../n372_genMoveP/n372_210_pieceBProKnight.hpp"
#include "../n372_genMoveP/n372_220_pieceBProSilver.hpp"
#include "../n372_genMoveP/n372_230_pieceBHorse.hpp"
#include "../n372_genMoveP/n372_240_pieceBDragon.hpp"
#include "../n372_genMoveP/n372_270_pieceWPawn.hpp"
#include "../n372_genMoveP/n372_280_pieceWLance.hpp"
#include "../n372_genMoveP/n372_290_pieceWKnight.hpp"
#include "../n372_genMoveP/n372_300_pieceWSilver.hpp"
#include "../n372_genMoveP/n372_310_pieceWBishop.hpp"
#include "../n372_genMoveP/n372_320_pieceWRook.hpp"
#include "../n372_genMoveP/n372_330_pieceWGold.hpp"
#include "../n372_genMoveP/n372_340_pieceWKing.hpp"
#include "../n372_genMoveP/n372_350_pieceWProPawn.hpp"
#include "../n372_genMoveP/n372_360_pieceWProLance.hpp"
#include "../n372_genMoveP/n372_370_pieceWProKnight.hpp"
#include "../n372_genMoveP/n372_380_pieceWProSilver.hpp"
#include "../n372_genMoveP/n372_390_pieceWHorse.hpp"
#include "../n372_genMoveP/n372_400_pieceWDragon.hpp"
#include "../n372_genMoveP/n372_410_pieceNone.hpp"


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
