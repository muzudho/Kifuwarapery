//*
#include "../../header/n372_genMoveP/n372_070_pieceAbstract.hpp"
#include "../../header/n376_genMoveP/n376_080_pieceNull.hpp"
#include "../../header/n376_genMoveP/n376_190_pieceBProPawn.hpp"
#include "../../header/n376_genMoveP/n376_100_pieceEmpty.hpp"
#include "../../header/n376_genMoveP/n376_110_pieceBPawn.hpp"
#include "../../header/n376_genMoveP/n376_120_pieceBLance.hpp"
#include "../../header/n376_genMoveP/n376_130_pieceBKnight.hpp"
#include "../../header/n376_genMoveP/n376_140_pieceBSilver.hpp"
#include "../../header/n376_genMoveP/n376_150_pieceBBishop.hpp"
#include "../../header/n376_genMoveP/n376_160_pieceBRook.hpp"
#include "../../header/n376_genMoveP/n376_170_pieceBGold.hpp"
#include "../../header/n376_genMoveP/n376_180_pieceBKing.hpp"
#include "../../header/n376_genMoveP/n376_190_pieceBProPawn.hpp"
#include "../../header/n376_genMoveP/n376_200_pieceBProLance.hpp"
#include "../../header/n376_genMoveP/n376_210_pieceBProKnight.hpp"
#include "../../header/n376_genMoveP/n376_220_pieceBProSilver.hpp"
#include "../../header/n376_genMoveP/n376_230_pieceBHorse.hpp"
#include "../../header/n376_genMoveP/n376_240_pieceBDragon.hpp"
#include "../../header/n376_genMoveP/n376_270_pieceWPawn.hpp"
#include "../../header/n376_genMoveP/n376_280_pieceWLance.hpp"
#include "../../header/n376_genMoveP/n376_290_pieceWKnight.hpp"
#include "../../header/n376_genMoveP/n376_300_pieceWSilver.hpp"
#include "../../header/n376_genMoveP/n376_310_pieceWBishop.hpp"
#include "../../header/n376_genMoveP/n376_320_pieceWRook.hpp"
#include "../../header/n376_genMoveP/n376_330_pieceWGold.hpp"
#include "../../header/n376_genMoveP/n376_340_pieceWKing.hpp"
#include "../../header/n376_genMoveP/n376_350_pieceWProPawn.hpp"
#include "../../header/n376_genMoveP/n376_360_pieceWProLance.hpp"
#include "../../header/n376_genMoveP/n376_370_pieceWProKnight.hpp"
#include "../../header/n376_genMoveP/n376_380_pieceWProSilver.hpp"
#include "../../header/n376_genMoveP/n376_390_pieceWHorse.hpp"
#include "../../header/n376_genMoveP/n376_400_pieceWDragon.hpp"
#include "../../header/n376_genMoveP/n376_410_pieceNone.hpp"
//*/
#include "../../header/n376_genMoveP/n376_500_pieceArray.hpp"


//────────────────────────────────────────────────────────────────────────────────
// 静的メンバの実体
//────────────────────────────────────────────────────────────────────────────────
//*
const PieceEmpty PieceArray::m_pieceEmpty;	//[0]
const PieceBPawn PieceArray::m_pieceBPawn;
const PieceBLance PieceArray::m_pieceBLance;
const PieceBKnight PieceArray::m_pieceBKnight;
const PieceBSilver PieceArray::m_pieceBSilver;
const PieceBBishop PieceArray::m_pieceBBishop;	//[5]
const PieceBRook PieceArray::m_pieceBRook;
const PieceBGold PieceArray::m_pieceBGold;
const PieceBKing PieceArray::m_pieceBKing;
const PieceBProPawn PieceArray::m_pieceBProPawn;
const PieceBProLance PieceArray::m_pieceBProLance;	//[10]
const PieceBProKnight PieceArray::m_pieceBProKnight;
const PieceBProSilver PieceArray::m_pieceBProSilver;
const PieceBHorse PieceArray::m_pieceBHorse;
const PieceBDragon PieceArray::m_pieceBDragon;	//[14]
const PieceNull PieceArray::m_pieceNull;	//[15],[16]
const PieceWPawn PieceArray::m_pieceWPawn;
const PieceWLance PieceArray::m_pieceWLance;
const PieceWKnight PieceArray::m_pieceWKnight;
const PieceWSilver PieceArray::m_pieceWSilver;	//[20]
const PieceWBishop PieceArray::m_pieceWBishop;
const PieceWRook PieceArray::m_pieceWRook;
const PieceWGold PieceArray::m_pieceWGold;
const PieceWKing PieceArray::m_pieceWKing;
const PieceWProPawn PieceArray::m_pieceWProPawn;	//[25]
const PieceWProLance PieceArray::m_pieceWProLance;
const PieceWProKnight PieceArray::m_pieceWProKnight;
const PieceWProSilver PieceArray::m_pieceWProSilver;
const PieceWHorse PieceArray::m_pieceWHorse;
const PieceWDragon PieceArray::m_pieceWDragon;	//[30]
const PieceNone PieceArray::m_pieceNone;	//[31]

const PieceAbstract PieceArray::m_pieceAbstractArray[] = {//32
	PieceArray::m_pieceEmpty,	//[0]
	PieceArray::m_pieceBPawn,
	PieceArray::m_pieceBLance,
	PieceArray::m_pieceBKnight,
	PieceArray::m_pieceBSilver,
	PieceArray::m_pieceBBishop,//[5]
	PieceArray::m_pieceBRook,
	PieceArray::m_pieceBGold,
	PieceArray::m_pieceBKing,
	PieceArray::m_pieceBProPawn,
	PieceArray::m_pieceBProLance,//[10]
	PieceArray::m_pieceBProKnight,
	PieceArray::m_pieceBProSilver,
	PieceArray::m_pieceBHorse,
	PieceArray::m_pieceBDragon,//[14]
	PieceArray::m_pieceNull,//[15]欠番
	PieceArray::m_pieceNull,//[16]欠番
	PieceArray::m_pieceWPawn,
	PieceArray::m_pieceWLance,
	PieceArray::m_pieceWKnight,
	PieceArray::m_pieceWSilver,//[20]
	PieceArray::m_pieceWBishop,
	PieceArray::m_pieceWRook,
	PieceArray::m_pieceWGold,
	PieceArray::m_pieceWKing,
	PieceArray::m_pieceWProPawn,//[25]
	PieceArray::m_pieceWProLance,
	PieceArray::m_pieceWProKnight,
	PieceArray::m_pieceWProSilver,
	PieceArray::m_pieceWHorse,
	PieceArray::m_pieceWDragon,//[30]
	PieceArray::m_pieceNone//[31]
};
//*/