#include "stdafx.h"
#include "../../header/n050_usoTuple/n050_100_MoveScore.h"

MoveScore::MoveScore(Move move, Score score)
{
	this->move = move;
	this->score = score;
}

Move MoveScore::GetMove(void)
{
	return this->move;
}
