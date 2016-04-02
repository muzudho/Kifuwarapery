#include "../../header/n220_position/n220_100_repetitionType.hpp"

class Position;

struct CheckInfo {
	explicit CheckInfo(const Position&);
	Bitboard dcBB; // discoverd check candidates bitboard
	Bitboard pinned;
	Bitboard checkBB[PieceTypeNum];
};
