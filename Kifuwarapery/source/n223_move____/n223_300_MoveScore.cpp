#include "../../header/n223_move____/n223_300_moveScore.hpp"

MoveScore::MoveScore(Move move, Score score)
{
	this->move = move;
	this->score = score;
}

Move MoveScore::GetMove(void)
{
	return this->move;
}
