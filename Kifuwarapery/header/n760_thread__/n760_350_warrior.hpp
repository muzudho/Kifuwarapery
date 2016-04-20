#pragma once


#include "n760_250_military.hpp"

class Rucksack;


// 元の名前：　ＴｉｍｅｒＴｈｒｅａｄ
struct Warrior : public Military {

	explicit Warrior(Rucksack* s) : Military(s), m_msec(0) {}

	virtual void IdleLoop();

	int m_msec;
};
