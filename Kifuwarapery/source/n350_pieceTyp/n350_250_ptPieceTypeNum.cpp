#include "../../header/n350_pieceTyp/n350_250_ptPieceTypeNum.hpp"


PieceType PtPieceTypeNum::AppendToNextAttackerAndTryPromote(
	const Position& pos,
	const Square to,
	const Bitboard& opponentAttackers,
	Bitboard& occupied,
	Bitboard& attackers,
	const Color turn,
	PieceType nextPT
	) const {
	PieceType PT = PieceType::N15_PieceTypeNum;

	UNREACHABLE;

	// それ以外の駒種類は、そのまま返す☆
	return PT;
}
