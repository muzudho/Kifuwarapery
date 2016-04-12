#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n105_color___/n105_100_color.hpp"
#include "../../header/n110_square__/n110_100_square.hpp"
#include "../../header/n112_pieceTyp/n112_050_pieceType.hpp"
#include "../../header/n160_board___/n160_100_bitboard.hpp"
#include "../../header/n160_board___/n160_120_bishopAttackBb.hpp"
#include "../../header/n160_board___/n160_130_lanceAttackBb.hpp"
#include "../../header/n160_board___/n160_150_rookAttackBb.hpp"
#include "../../header/n160_board___/n160_180_knightAttackBb.hpp"
#include "../../header/n160_board___/n160_190_pawnAttackBb.hpp"
#include "../../header/n160_board___/n160_200_horseAttackBb.hpp"
#include "../../header/n160_board___/n160_210_dragonAttackBb.hpp"
#include "../../header/n220_position/n220_660_utilAttack.hpp"


Bitboard UtilAttack::GetAttacksFrom(const PieceType pType, const Color c, const Square sq, const Bitboard& occupied) {
	switch (pType) {
	case N00_Occupied:  return Bitboard::CreateAllZeroBB();
	case N01_Pawn:      return g_pawnAttackBb.GetControllBb(c, sq);
	case N02_Lance:     return g_lanceAttackBb.GetControllBb(&occupied, c, sq);
	case N03_Knight:    return g_knightAttackBb.GetControllBb(c, sq);
	case N04_Silver:    return g_silverAttackBb.GetControllBb(c, sq);
	case N05_Bishop:    return g_bishopAttackBb.BishopAttack(&occupied, sq);
	case N06_Rook:      return g_rookAttackBb.GetControllBb(&occupied, sq);
	case N07_Gold:		// thru
	case N09_ProPawn:	// thru
	case N10_ProLance:	// thru
	case N11_ProKnight:	// thru
	case N12_ProSilver: return g_goldAttackBb.GetControllBb(c, sq);
	case N08_King:      return g_kingAttackBb.GetControllBb(sq);
	case N13_Horse:     return g_horseAttackBb.GetControllBb(&occupied, sq);
	case N14_Dragon:    return g_dragonAttackBb.GetControllBb(&occupied, sq);
	default:        UNREACHABLE;
	}
	UNREACHABLE;
	return Bitboard::CreateAllOneBB();
}
