#include "../../header/n223_move____/n223_300_moveScoreindex.hpp"

MoveScoreindex::MoveScoreindex(Move move, Score score)
{
	this->m_move = move;
	this->m_score = score;
}

Move MoveScoreindex::GetMove(void)
{
	return this->m_move;
}
