#pragma once


#include <algorithm>	//std::max
#include "../n119_score___/n119_090_scoreIndex.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n223_move____/n223_040_nodeType.hpp"
#include "../n223_move____/n223_200_depth.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"
#include "n885_040_rucksack.hpp"	// FIXME:


class HitchhikerQsearchAbstract {
public:

	// Ｑサーチ？
	// N01_PV か、N02_NonPV でだけ使うことができるぜ☆（＾ｑ＾）
	virtual ScoreIndex DoQsearch(
		Rucksack& rucksack,
		bool INCHECK,
		Position& pos,
		Flashlight* ss,
		ScoreIndex alpha,
		ScoreIndex beta,
		const Depth depth
		) const ;

	virtual inline void DoAssert(
		ScoreIndex alpha,
		ScoreIndex beta
		) const = 0;

	virtual inline void SetOldAlpha(
		ScoreIndex& oldAlpha,
		ScoreIndex alpha
		) const = 0;

	virtual inline bool GetCondition01(
		const TTEntry** ppTtEntry,
		ScoreIndex beta,
		ScoreIndex ttScore
		) const = 0;

	virtual inline void SetAlpha(
		ScoreIndex& alpha,
		ScoreIndex bestScore
		) const = 0;

	virtual inline void DoFutilityPruning01(
		bool& isContinue,
		bool& INCHECK,
		bool& givesCheck,
		Move& move,
		Move& ttMove,
		ScoreIndex& futilityScore,
		ScoreIndex& futilityBase,
		Position& pos,
		ScoreIndex& beta,
		ScoreIndex& bestScore,
		const Depth depth
		)const = 0;

	virtual inline void DoContinue01(
		bool& isContinue,
		bool& INCHECK,
		bool& evasionPrunable,
		Move& move,
		Move& ttMove,
		Position& pos
		)const = 0;

	virtual inline void DoByNewScore(
		bool& isReturnWithScore,
		ScoreIndex& returnScore,
		Rucksack& rucksack,
		ScoreIndex& score,
		ScoreIndex& beta,
		ScoreIndex& alpha,
		Move& bestMove,
		Key& posKey,
		Flashlight** ppFlashlight,
		Depth ttDepth,
		Move move
		)const = 0;

	virtual inline Bound GetBound01(
		ScoreIndex& oldAlpha,
		ScoreIndex& bestScore
		)const = 0;
};
