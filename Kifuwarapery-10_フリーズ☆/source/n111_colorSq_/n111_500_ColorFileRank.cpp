#include "../../header/n111_colorSq_/n111_500_ColorFileRank.h"


ColorFileRank::ColorFileRank(Color color, File file, Rank rank)
{
	this->m_color = color;
	this->m_file = file;
	this->m_rank = rank;
}

int ColorFileRank::GetOrder()
{
	return this->m_color*10000 + this->m_file*100 + this->m_rank;
}
