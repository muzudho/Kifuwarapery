#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n119_score___/n119_050_bound.hpp"
#include "../n119_score___/n119_090_scoreIndex.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_600_convMove.hpp"
#include "../n223_move____/n223_200_depth.hpp"


class TTEntry {
public:

	u32   GetKey() const;

	Depth GetDepth() const;

	ScoreIndex GetScore() const;

	Move  GetMove() const;

	Bound GetType() const;

	u8    GetGeneration() const;

	ScoreIndex GetEvalScore() const;

	void SetGeneration(const u8 g);

	void SetSave(const Depth depth, const ScoreIndex score, const Move move,
		const u32 posKeyHigh32, const Bound bound, const u8 generation,
		const ScoreIndex evalScore);

private:

	u32 m_key32_;

	u16 m_move16_;

	u8 m_bound_;

	u8 m_generation8_;

	s16 m_score16_;

	s16 m_depth16_;

	s16 m_evalScore_;

};
