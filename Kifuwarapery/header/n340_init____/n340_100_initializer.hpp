#pragma once

#include "../n080_common__/n080_050_ifdef.hpp"
#include "../n080_common__/n080_100_common.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"

class Initializer {
public:
	void InitTable();

#if defined FIND_MAGIC
	u64 findMagic(const Square sqare, const bool isBishop);
#endif // #if defined FIND_MAGIC

private:
	Bitboard RookBlockMaskCalc(const Square square);

	Bitboard BishopBlockMaskCalc(const Square square);

	Bitboard LanceBlockMask(const Square square);

	Bitboard AttackCalc(const Square square, const Bitboard& occupied, const bool isBishop);

	Bitboard LanceAttackCalc(const Color c, const Square square, const Bitboard& occupied);

	Bitboard IndexToOccupied(const int index, const int bits, const Bitboard& blockMask);

	void InitAttacks(const bool isBishop);

	void InitLanceAttacks();

	void InitKingAttacks();

	void InitGoldAttacks();

	void InitSilverAttacks();

	void InitKnightAttacks();

	void InitPawnAttacks();

	void InitSquareRelation();

	void InitAttackToEdge();

	void InitBetweenBB();

	void InitCheckTable();

	void InitSquareDistance();

};

