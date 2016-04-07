#include "../../header/n240_tt______/n240_280_tTEntry.hpp"

u32 TTEntry::GetKey() const
{
	return this->m_key32_;
}

Depth TTEntry::GetDepth() const
{
	return static_cast<Depth>(this->m_depth16_);
}

Score TTEntry::GetScore() const
{
	return static_cast<Score>(this->m_score16_);
}

Move TTEntry::GetMove() const
{
	return static_cast<Move>(this->m_move16_);
}

Bound TTEntry::GetType() const
{
	return static_cast<Bound>(this->m_bound_);
}

u8 TTEntry::GetGeneration() const
{
	return this->m_generation8_;
}

Score TTEntry::GetEvalScore() const
{
	return static_cast<Score>(this->m_evalScore_);
}

void TTEntry::SetGeneration(const u8 g)
{
	this->m_generation8_ = g;
}

void TTEntry::SetSave(const Depth depth, const Score score, const Move move, const u32 posKeyHigh32, const Bound bound, const u8 generation, const Score evalScore)
{
	this->m_key32_ = posKeyHigh32;
	this->m_move16_ = static_cast<u16>(move.GetValue());
	this->m_bound_ = static_cast<u8>(bound);
	this->m_generation8_ = generation;
	this->m_score16_ = static_cast<s16>(score);
	this->m_depth16_ = static_cast<s16>(depth);
	this->m_evalScore_ = static_cast<s16>(evalScore);
}
