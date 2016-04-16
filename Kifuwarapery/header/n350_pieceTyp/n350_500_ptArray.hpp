#pragma once


#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n350_pieceTyp/n350_070_ptAbstract.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
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


extern const PtAbstract* g_ptArray[PieceType::N15_PieceTypeNum];


class PieceTypeArray {
public:

	static const PtOccupied m_occupied;
	static const PtPawn m_pawn;
	static const PtLance m_lance;
	static const PtKnight m_knight;
	static const PtSilver m_silver;
	static const PtBishop m_bishop;
	static const PtRook m_rook;
	static const PtGold m_gold;
	static const PtKing m_king;
	static const PtProPawn m_proPawn;
	static const PtProLance m_proLance;
	static const PtProKnight m_proKnight;
	static const PtProSilver m_proSilver;
	static const PtHorse m_horse;
	static const PtDragon m_dragon;

	static const PtAbstract m_ptArray[PieceType::N15_PieceTypeNum];

};
