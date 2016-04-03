#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n113_piece___/n113_205_utilHandPiece.hpp"
#include "../../header/n120_brdEntry/n120_250_hand.hpp"

const int Hand::HandPieceShiftBits[HandPieceNum] = {
	HPawnShiftBits,
	HLanceShiftBits,
	HKnightShiftBits,
	HSilverShiftBits,
	HGoldShiftBits,
	HBishopShiftBits,
	HRookShiftBits
};
const u32 Hand::HandPieceMask[HandPieceNum] = {
	HPawnMask,
	HLanceMask,
	HKnightMask,
	HSilverMask,
	HGoldMask,
	HBishopMask,
	HRookMask
};
// 特定の種類の持ち駒を 1 つ増やしたり減らしたりするときに使用するテーブル
const u32 Hand::HandPieceOne[HandPieceNum] = {
	1 << HPawnShiftBits,
	1 << HLanceShiftBits,
	1 << HKnightShiftBits,
	1 << HSilverShiftBits,
	1 << HGoldShiftBits,
	1 << HBishopShiftBits,
	1 << HRookShiftBits
};
