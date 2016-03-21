#include "n240_400_MoveScore.h"

MoveScore::MoveScore(Move move, Score score)
{
	this->move = move;
	this->score = score;
}

Move MoveScore::GetMove(void)
{
	return this->move;
}
