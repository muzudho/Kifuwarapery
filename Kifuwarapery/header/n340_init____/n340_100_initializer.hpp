#pragma once

#include "../n080_common__/n080_050_ifdef.hpp"
#include "../n080_common__/n080_100_common.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"

class Initializer {
public:
	void initTable();

#if defined FIND_MAGIC
	u64 findMagic(const Square sqare, const bool isBishop);
#endif // #if defined FIND_MAGIC

private:
	Bitboard rookBlockMaskCalc(const Square square);

	Bitboard bishopBlockMaskCalc(const Square square);

	Bitboard lanceBlockMask(const Square square);

	Bitboard attackCalc(const Square square, const Bitboard& occupied, const bool isBishop);

	Bitboard lanceAttackCalc(const Color c, const Square square, const Bitboard& occupied);

	Bitboard indexToOccupied(const int index, const int bits, const Bitboard& blockMask);

	void initAttacks(const bool isBishop);

	void initLanceAttacks();

	void initKingAttacks();

	void initGoldAttacks();

	void initSilverAttacks();

	void initKnightAttacks();

	void initPawnAttacks();

	void initSquareRelation();

	void initAttackToEdge();

	void initBetweenBB();

	void initCheckTable();

	void initSquareDistance();

};

