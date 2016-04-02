#pragma once

#include "../../header/n240_position/n240_275_depth.hpp"


class TTEntry {
public:
	u32   key() const { return key32_; }
	Depth depth() const { return static_cast<Depth>(depth16_); }
	Score score() const { return static_cast<Score>(score16_); }
	Move  move() const { return static_cast<Move>(move16_); }
	Bound type() const { return static_cast<Bound>(bound_); }
	u8    generation() const { return generation8_; }
	Score evalScore() const { return static_cast<Score>(evalScore_); }
	void setGeneration(const u8 g) { generation8_ = g; }

	void save(const Depth depth, const Score score, const Move move,
		const u32 posKeyHigh32, const Bound bound, const u8 generation,
		const Score evalScore)
	{
		key32_ = posKeyHigh32;
		move16_ = static_cast<u16>(move.value());
		bound_ = static_cast<u8>(bound);
		generation8_ = generation;
		score16_ = static_cast<s16>(score);
		depth16_ = static_cast<s16>(depth);
		evalScore_ = static_cast<s16>(evalScore);
	}

private:
	u32 key32_;
	u16 move16_;
	u8 bound_;
	u8 generation8_;
	s16 score16_;
	s16 depth16_;
	s16 evalScore_;
};
