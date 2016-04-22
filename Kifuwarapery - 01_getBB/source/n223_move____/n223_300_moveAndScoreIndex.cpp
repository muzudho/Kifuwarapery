#include "../../header/n223_move____/n223_300_moveAndScoreIndex.hpp"

MoveAndScoreIndex::MoveAndScoreIndex(Move move, ScoreIndex score):
	m_move(move),
	m_scoreIndex(score)
{
	//this->m_move = move;
	//this->m_score = score;
}

Move MoveAndScoreIndex::GetMove(void)
{
	return this->m_move;
}
