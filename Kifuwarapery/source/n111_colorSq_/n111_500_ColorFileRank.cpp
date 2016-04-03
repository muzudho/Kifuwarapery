#include "../../header/n111_colorSq_/n111_500_ColorFileRank.h"

ColorFileRank::ColorFileRank(Color color, File file, Rank rank)
{
	this->color = color;
	this->file = file;
	this->rank = rank;
}

int ColorFileRank::GetOrder()
{
	return this->color*10000 + this->file*100 + this->rank;
}
