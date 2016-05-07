#include "../../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_045_pieceTypeSeeEvent.hpp"
#include "../../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_250_ptPieceTypeNum.hpp"


PieceType PtPieceTypeNum::AppendToNextAttackerAndTryPromote(
	Bitboard& occupied,
	Bitboard& attackers,
	PieceType nextPT,
	const PieceTypeSeeEvent ptsEvent
	) const {
	PieceType PT = PieceType::N15_PieceTypeNum;

	UNREACHABLE;

	// それ以外の駒種類は、そのまま返す☆
	return PT;
}
