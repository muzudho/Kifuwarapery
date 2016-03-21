#include "stdafx.h"
#include "..\..\header\n050_usoTuple\n050_120_ColorFileRank.h"

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
