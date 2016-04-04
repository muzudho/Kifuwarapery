#pragma once

#include "../../header/n223_move____/n223_200_depth.hpp"


class TTEntry {
public:
	u32   GetKey() const;

	Depth GetDepth() const;

	Score GetScore() const;

	Move  GetMove() const;

	Bound GetType() const;

	u8    GetGeneration() const;

	Score GetEvalScore() const;

	void SetGeneration(const u8 g);

	void SetSave(const Depth depth, const Score score, const Move move,
		const u32 posKeyHigh32, const Bound bound, const u8 generation,
		const Score evalScore);

private:
	u32 m_key32_;
	u16 m_move16_;
	u8 m_bound_;
	u8 m_generation8_;
	s16 m_score16_;
	s16 m_depth16_;
	s16 m_evalScore_;
};
