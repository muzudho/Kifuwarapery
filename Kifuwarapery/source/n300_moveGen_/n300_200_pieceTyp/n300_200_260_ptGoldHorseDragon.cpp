#include "../../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_045_pieceTypeSeeEvent.hpp"
#include "../../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_260_ptGoldHorseDragon.hpp"


PieceType PtGoldHorseDragon::AppendToNextAttackerAndTryPromote(
	Bitboard& occupied,
	Bitboard& attackers,
	PieceType nextPT,
	const PieceTypeSeeEvent ptsEvent
	) const {
	PieceType PT = PieceType::N16_GoldHorseDragon;

	UNREACHABLE;

	// それ以外の駒種類は、そのまま返す☆
	return PT;
}
