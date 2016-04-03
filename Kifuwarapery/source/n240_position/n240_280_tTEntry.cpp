#include "../../header/n240_position/n240_280_tTEntry.hpp"

u32 TTEntry::key() const
{
	return this->key32_;
}

Depth TTEntry::depth() const
{
	return static_cast<Depth>(this->depth16_);
}

Score TTEntry::score() const
{
	return static_cast<Score>(this->score16_);
}

Move TTEntry::move() const
{
	return static_cast<Move>(this->move16_);
}

Bound TTEntry::type() const
{
	return static_cast<Bound>(this->bound_);
}

u8 TTEntry::generation() const
{
	return this->generation8_;
}

Score TTEntry::evalScore() const
{
	return static_cast<Score>(this->evalScore_);
}

void TTEntry::setGeneration(const u8 g)
{
	this->generation8_ = g;
}

void TTEntry::save(const Depth depth, const Score score, const Move move, const u32 posKeyHigh32, const Bound bound, const u8 generation, const Score evalScore)
{
	this->key32_ = posKeyHigh32;
	this->move16_ = static_cast<u16>(move.GetValue());
	this->bound_ = static_cast<u8>(bound);
	this->generation8_ = generation;
	this->score16_ = static_cast<s16>(score);
	this->depth16_ = static_cast<s16>(depth);
	this->evalScore_ = static_cast<s16>(evalScore);
}
