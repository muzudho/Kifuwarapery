#pragma once

#include "../n119_score___/n119_090_score.hpp"
#include "../n165_movStack/n165_400_move.hpp"


// 指し手と、評価値。
class MoveScore {
public:

	// 指し手
	Move m_move;

	// 評価値
	enum Score m_score;

public:

	MoveScore(Move move, enum Score score);
	
	// 指し手
	Move GetMove(void);

};