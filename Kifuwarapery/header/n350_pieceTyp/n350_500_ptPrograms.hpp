#pragma once


#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "n350_040_ptEvent.hpp"
#include "n350_070_ptAbstract.hpp"
#include "n350_100_ptOccupied.hpp"
#include "n350_110_ptPawn.hpp"
#include "n350_120_ptLance.hpp"
#include "n350_130_ptKnight.hpp"
#include "n350_140_ptSilver.hpp"
#include "n350_150_ptBishop.hpp"
#include "n350_160_ptRook.hpp"
#include "n350_170_ptGold.hpp"
#include "n350_180_ptKing.hpp"
#include "n350_190_ptProPawn.hpp"
#include "n350_200_ptProLance.hpp"
#include "n350_210_ptProKnight.hpp"
#include "n350_220_ptProSilver.hpp"
#include "n350_230_ptHorse.hpp"
#include "n350_240_ptDragon.hpp"


// （＾ｑ＾）プログラムが配列に入っているような使い方をするんだぜ☆
// これは、駒の種類別になっているプログラムだな☆（＾▽＾）
class PiecetypePrograms {
public:

	static const PtOccupied m_OCCUPIED;
	static const PtPawn m_PAWN;
	static const PtLance m_LANCE;
	static const PtKnight m_KNIGHT;
	static const PtSilver m_SILVER;
	static const PtBishop m_BISHOP;
	static const PtRook m_ROOK;
	static const PtGold m_GOLD;
	static const PtKing m_KING;
	static const PtProPawn m_PRO_PAWN;
	static const PtProLance m_PRO_LANCE;
	static const PtProKnight m_PRO_KNIGHT;
	static const PtProSilver m_PRO_SILVER;
	static const PtHorse m_HORSE;
	static const PtDragon m_DRAGON;

	static const PtAbstract* m_PIECETYPE_PROGRAMS[g_PIECETYPE_NUM];

};
