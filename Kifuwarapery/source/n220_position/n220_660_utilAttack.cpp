#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n105_color___/n105_100_color.hpp"
#include "../../header/n110_square__/n110_100_square.hpp"
#include "../../header/n113_piece___/n113_100_pieceType.hpp"
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
	case Occupied:  return Bitboard::CreateAllZeroBB();
	case Pawn:      return g_pawnAttackBb.GetControllBb(c, sq);
	case Lance:     return g_lanceAttackBb.GetControllBb(&occupied, c, sq);
	case Knight:    return g_knightAttackBb.GetControllBb(c, sq);
	case Silver:    return g_silverAttackBb.GetControllBb(c, sq);
	case Bishop:    return g_bishopAttackBb.BishopAttack(&occupied, sq);
	case Rook:      return g_rookAttackBb.GetControllBb(&occupied, sq);
	case Gold:		// thru
	case ProPawn:	// thru
	case ProLance:	// thru
	case ProKnight:	// thru
	case ProSilver: return g_goldAttackBb.GetControllBb(c, sq);
	case King:      return g_kingAttackBb.GetControllBb(sq);
	case Horse:     return g_horseAttackBb.GetControllBb(&occupied, sq);
	case Dragon:    return g_dragonAttackBb.GetControllBb(&occupied, sq);
	default:        UNREACHABLE;
	}
	UNREACHABLE;
	return Bitboard::CreateAllOneBB();
}
