#pragma once

#include "../../header/n240_position/n240_275_depth.hpp"


class TTEntry {
public:
	u32   key() const;

	Depth depth() const;

	Score score() const;

	Move  move() const;

	Bound type() const;

	u8    generation() const;

	Score evalScore() const;

	void setGeneration(const u8 g);

	void save(const Depth depth, const Score score, const Move move,
		const u32 posKeyHigh32, const Bound bound, const u8 generation,
		const Score evalScore);

private:
	u32 key32_;
	u16 move16_;
	u8 bound_;
	u8 generation8_;
	s16 score16_;
	s16 depth16_;
	s16 evalScore_;
};
