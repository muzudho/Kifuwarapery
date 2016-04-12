//#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n105_color___/n105_100_color.hpp"
#include "../../header/n110_square__/n110_100_square.hpp"
#include "../../header/n112_pieceTyp/n112_050_pieceType.hpp"
#include "../../header/n160_board___/n160_100_bitboard.hpp"
#include "../../header/n170_pieceTyp/n170_070_ptAbstract.hpp"
#include "../../header/n170_pieceTyp/n170_500_ptArray.hpp"
#include "../../header/n220_position/n220_660_utilAttack.hpp"

/*
#include "../../header/n160_board___/n160_120_bishopAttackBb.hpp"
#include "../../header/n160_board___/n160_130_lanceAttackBb.hpp"
#include "../../header/n160_board___/n160_150_rookAttackBb.hpp"
#include "../../header/n160_board___/n160_180_knightAttackBb.hpp"
#include "../../header/n160_board___/n160_190_pawnAttackBb.hpp"
#include "../../header/n160_board___/n160_200_horseAttackBb.hpp"
#include "../../header/n160_board___/n160_210_dragonAttackBb.hpp"
#include "../../header/n170_pieceTyp/n170_100_ptOccupied.hpp"
#include "../../header/n170_pieceTyp/n170_110_ptPawn.hpp"
#include "../../header/n170_pieceTyp/n170_120_ptLance.hpp"
#include "../../header/n170_pieceTyp/n170_130_ptKnight.hpp"
#include "../../header/n170_pieceTyp/n170_140_ptSilver.hpp"
#include "../../header/n170_pieceTyp/n170_150_ptBishop.hpp"
#include "../../header/n170_pieceTyp/n170_160_ptRook.hpp"
#include "../../header/n170_pieceTyp/n170_170_ptGold.hpp"
#include "../../header/n170_pieceTyp/n170_180_ptKing.hpp"
#include "../../header/n170_pieceTyp/n170_190_ptProPawn.hpp"
#include "../../header/n170_pieceTyp/n170_200_ptProLance.hpp"
#include "../../header/n170_pieceTyp/n170_210_ptProKnight.hpp"
#include "../../header/n170_pieceTyp/n170_220_ptProSilver.hpp"
#include "../../header/n170_pieceTyp/n170_230_ptHorse.hpp"
#include "../../header/n170_pieceTyp/n170_240_ptDragon.hpp"
//*/


extern PtAbstract* g_ptArray[PieceType::N15_PieceTypeNum];


Bitboard UtilAttack::GetAttacksFrom(const PieceType pType, const Color c, const Square sq, const Bitboard& occupied) {
	// FIXME: 配列の範囲チェックをしてないぜ☆（＾ｑ＾）
	return g_ptArray[pType]->GetAttacks2From(occupied, c, sq);
	/*
	switch (pType) {
	case N00_Occupied:  return g_ptOccupied.GetAttacks2From(occupied, c, sq);//引数はどれも使ってないぜ☆
	case N01_Pawn:      return g_ptPawn.GetAttacks2From(occupied, c, sq);//g_nullBitboard
	case N02_Lance:     return g_ptLance.GetAttacks2From(occupied, c, sq);
	case N03_Knight:    return g_ptKnight.GetAttacks2From(occupied, c, sq);//g_nullBitboard
	case N04_Silver:    return g_ptSilver.GetAttacks2From(occupied, c, sq);//g_nullBitboard
	case N05_Bishop:    return g_ptBishop.GetAttacks2From(occupied, c, sq);//Colorは使ってない。Color::ColorNum
	case N06_Rook:      return g_ptRook.GetAttacks2From(occupied, c, sq);//Colorは使ってない。Color::ColorNum
	case N07_Gold:		return g_ptGold.GetAttacks2From(occupied, c, sq);//g_nullBitboard
	case N08_King:      return g_ptKing.GetAttacks2From(occupied, c, sq);//g_nullBitboard //Color::ColorNum
	case N09_ProPawn:	return g_ptProPawn.GetAttacks2From(occupied, c, sq);//g_nullBitboard
	case N10_ProLance:	return g_ptProLance.GetAttacks2From(occupied, c, sq);//g_nullBitboard
	case N11_ProKnight:	return g_ptProKnight.GetAttacks2From(occupied, c, sq);//g_nullBitboard
	case N12_ProSilver: return g_ptProSilver.GetAttacks2From(occupied, c, sq);//g_nullBitboard
	case N13_Horse:     return g_ptHorse.GetAttacks2From(occupied, c, sq);//Color::ColorNum
	case N14_Dragon:    return g_ptDragon.GetAttacks2From(occupied, c, sq);//Color::ColorNum
	default:        UNREACHABLE;
	}
	UNREACHABLE;
	return Bitboard::CreateAllOneBB();
	//*/
}
