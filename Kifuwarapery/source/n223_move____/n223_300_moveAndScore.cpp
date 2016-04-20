#include "../../header/n223_move____/n223_300_moveAndScore.hpp"

MoveAndScore::MoveAndScore(Move move, Score score):
	m_move(move),
	m_score(score)
{
	//this->m_move = move;
	//this->m_score = score;
}

Move MoveAndScore::GetMove(void)
{
	return this->m_move;
}
