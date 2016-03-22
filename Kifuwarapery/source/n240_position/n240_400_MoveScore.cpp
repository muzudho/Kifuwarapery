#include "../../header/n240_position/n240_400_MoveScore.hpp"

MoveScore::MoveScore(Move move, Score score)
{
	this->move = move;
	this->score = score;
}

Move MoveScore::GetMove(void)
{
	return this->move;
}
