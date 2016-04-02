#pragma once

#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n080_common__/n080_150_overloadEnumOperators.hpp"
#include "../../header/n120_brdEntry/n120_300_ColorFileRank.h"
#include "../../header/n200_score___/n200_200_pieceScore.hpp"
#include "../../header/n220_position/n220_500_charToPieceUSI.hpp"
#include "../../header/n260_evaluate/n260_600_EvalSum.hpp"




class Position;
struct SearchStack;

const size_t EvaluateTableSize = 0x400000; // 134MB
//const size_t EvaluateTableSize = 0x10000000; // 8GB
//const size_t EvaluateTableSize = 0x20000000; // 17GB

using EvaluateHashEntry = EvalSum;
struct EvaluateHashTable : HashTable<EvaluateHashEntry, EvaluateTableSize> {};
extern EvaluateHashTable g_evalTable;


class Evaluation09 {
public:
	Score evaluateUnUseDiff(const Position& pos);
	Score evaluate(Position& pos, SearchStack* ss);
private:

	EvalSum doapc(const Position& pos, const int index[2]);
	std::array<s32, 2> doablack(const Position& pos, const int index[2]);
	std::array<s32, 2> doawhite(const Position& pos, const int index[2]);

#if defined INANIWA_SHIFT
	Score inaniwaScoreBody(const Position& pos);
	inline Score inaniwaScore(const Position& pos);
#endif

	bool calcDifference(Position& pos, SearchStack* ss);
	int make_list_unUseDiff(const Position& pos, int list0[EvalList::ListSize], int list1[EvalList::ListSize], int nlist);
	void evaluateBody(Position& pos, SearchStack* ss);
};

