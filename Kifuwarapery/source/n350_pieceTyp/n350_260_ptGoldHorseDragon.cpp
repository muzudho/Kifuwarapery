#include "../../header/n350_pieceTyp/n350_260_ptGoldHorseDragon.hpp"


PieceType PtGoldHorseDragon::AppendToNextAttackerAndTryPromote(
	const Position& pos,
	const Square to,
	const Bitboard& opponentAttackers,
	Bitboard& occupied,
	Bitboard& attackers,
	const Color turn,
	PieceType nextPT
	) const {
	PieceType PT = PieceType::N16_GoldHorseDragon;

	UNREACHABLE;

	// それ以外の駒種類は、そのまま返す☆
	return PT;
}
