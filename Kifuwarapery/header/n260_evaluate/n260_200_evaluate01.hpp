#pragma once

#include "../../header/n120_brdEntry/n120_150_square.hpp"
#include "../../header/n260_evaluate/n260_100_evaluateEnum.hpp"


class Evaluation01 {
public:
	inline static Square kppIndexToSquare(const int i) {
		const auto it = std::upper_bound(std::begin(KPPIndexArray), std::end(KPPIndexArray), i);
		return static_cast<Square>(i - *(it - 1));
	}

	inline static int kppIndexBegin(const int i) {
		return *(std::upper_bound(std::begin(KPPIndexArray), std::end(KPPIndexArray), i) - 1);
	}

	inline static bool kppIndexIsBlack(const int i) {
		// f_xxx と e_xxx が交互に配列に格納されているので、インデックスが偶数の時は Black
		return !((std::upper_bound(std::begin(KPPIndexArray), std::end(KPPIndexArray), i) - 1) - std::begin(KPPIndexArray) & 1);
	}

	inline static int kppBlackIndexToWhiteBegin(const int i) {
		assert(kppIndexIsBlack(i));
		return *std::upper_bound(std::begin(KPPIndexArray), std::end(KPPIndexArray), i);
	}


	inline static int kppWhiteIndexToBlackBegin(const int i) {
		return *(std::upper_bound(std::begin(KPPIndexArray), std::end(KPPIndexArray), i) - 2);
	}

	inline static int kppIndexToOpponentBegin(const int i, const bool isBlack) {
		return *(std::upper_bound(std::begin(KPPIndexArray), std::end(KPPIndexArray), i) - static_cast<int>(!isBlack) * 2);
	}


	inline static int kppIndexToOpponentBegin(const int i) {
		// todo: 高速化
		return kppIndexToOpponentBegin(i, kppIndexIsBlack(i));
	}

	inline static int inverseFileIndexIfLefterThanMiddle(const int index) {
		if (index < fe_hand_end) return index;
		const int begin = kppIndexBegin(index);
		const Square sq = static_cast<Square>(index - begin);
		if (sq <= Square::E1) return index;
		return static_cast<int>(begin + inverseFile(sq));
	};


	inline static int inverseFileIndexIfOnBoard(const int index) {
		if (index < fe_hand_end) return index;
		const int begin = kppIndexBegin(index);
		const Square sq = static_cast<Square>(index - begin);
		return static_cast<int>(begin + inverseFile(sq));
	};

	inline static int inverseFileIndexOnBoard(const int index) {
		assert(f_pawn <= index);
		const int begin = kppIndexBegin(index);
		const Square sq = static_cast<Square>(index - begin);
		return static_cast<int>(begin + inverseFile(sq));
	};
};
