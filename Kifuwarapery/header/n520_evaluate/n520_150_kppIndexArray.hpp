#pragma once

#include <algorithm> //std::upper_bound
#include "../n110_square__/n110_500_convSquare.hpp"
#include "../n520_evaluate/n520_100_evaluateEnum.hpp"



const int g_arrKPPIndex[] = {
	f_hand_pawn, e_hand_pawn, f_hand_lance, e_hand_lance, f_hand_knight,
	e_hand_knight, f_hand_silver, e_hand_silver, f_hand_gold, e_hand_gold,
	f_hand_bishop, e_hand_bishop, f_hand_rook, e_hand_rook, /*fe_hand_end,*/
	f_pawn, e_pawn, f_lance, e_lance, f_knight, e_knight, f_silver, e_silver,
	f_gold, e_gold, f_bishop, e_bishop, f_horse, e_horse, f_rook, e_rook,
	f_dragon, e_dragon, fe_end
};

class UtilKppIndex {
public:

	inline static Square ToSquare(const int i) {
		const auto it = std::upper_bound(std::begin(g_arrKPPIndex), std::end(g_arrKPPIndex), i);
		return static_cast<Square>(i - *(it - 1));
	}

	inline static int GetBegin(const int i) {
		return *(std::upper_bound(std::begin(g_arrKPPIndex), std::end(g_arrKPPIndex), i) - 1);
	}

	inline static bool IsBlack(const int i) {
		// f_xxx と e_xxx が交互に配列に格納されているので、インデックスが偶数の時は Black
		return !((std::upper_bound(std::begin(g_arrKPPIndex), std::end(g_arrKPPIndex), i) - 1) - std::begin(g_arrKPPIndex) & 1);
	}


	inline static int KppBlackIndexToWhiteBegin(const int i) {
		assert(UtilKppIndex::IsBlack(i));
		return *std::upper_bound(std::begin(g_arrKPPIndex), std::end(g_arrKPPIndex), i);
	}


	inline static int KppWhiteIndexToBlackBegin(const int i) {
		return *(std::upper_bound(std::begin(g_arrKPPIndex), std::end(g_arrKPPIndex), i) - 2);
	}

	inline static int ToOpponentBegin(const int i, const bool isBlack) {
		return *(std::upper_bound(std::begin(g_arrKPPIndex), std::end(g_arrKPPIndex), i) - static_cast<int>(!isBlack) * 2);
	}


	inline static int ToOpponentBegin(const int i) {
		// todo: 高速化
		return ToOpponentBegin(i, UtilKppIndex::IsBlack(i));
	}




	inline static int InverseFileIndexIfLefterThanMiddle(const int index) {
		if (index < fe_hand_end) return index;
		const int begin = UtilKppIndex::GetBegin(index);
		const Square sq = static_cast<Square>(index - begin);
		if (sq <= Square::E1) return index;
		return static_cast<int>(begin + ConvSquare::INVERSE_FILE40(sq));
	};


	inline static int InverseFileIndexIfOnBoard(const int index) {
		if (index < fe_hand_end) return index;
		const int begin = UtilKppIndex::GetBegin(index);
		const Square sq = static_cast<Square>(index - begin);
		return static_cast<int>(begin + ConvSquare::INVERSE_FILE40(sq));
	};

	inline static int InverseFileIndexOnBoard(const int index) {
		assert(f_pawn <= index);
		const int begin = UtilKppIndex::GetBegin(index);
		const Square sq = static_cast<Square>(index - begin);
		return static_cast<int>(begin + ConvSquare::INVERSE_FILE40(sq));
	};

};