#pragma once


#include "../n119_score___/n119_090_score.hpp"
#include "../n223_move____/n223_200_depth.hpp"



class FutilityMargins {
public:
	Score m_FutilityMargins[16][64]; // [depth][moveCount]

public:
	void Initialize();

	// Search関数で使う。
	inline Score GetFutilityMargin(const Depth depth, const int moveCount) {
		return (depth < 7 * OnePly ?
			this->m_FutilityMargins[std::max(depth, Depth1)][std::min(moveCount, 63)]
			: 2 * ScoreInfinite);
	}

};


extern FutilityMargins g_futilityMargins;
