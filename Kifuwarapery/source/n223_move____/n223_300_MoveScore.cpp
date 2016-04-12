#include "../../header/n223_move____/n223_300_moveScore.hpp"

MoveScore::MoveScore(Move move, Score score)
{
	this->m_move = move;
	this->m_score = score;
}

Move MoveScore::GetMove(void)
{
	return this->m_move;
}
