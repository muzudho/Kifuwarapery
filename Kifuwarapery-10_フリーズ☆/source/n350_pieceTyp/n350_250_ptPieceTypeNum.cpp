#include "../../header/n350_pieceTyp/n350_045_pieceTypeSeeEvent.hpp"
#include "../../header/n350_pieceTyp/n350_250_ptPieceTypeNum.hpp"


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
