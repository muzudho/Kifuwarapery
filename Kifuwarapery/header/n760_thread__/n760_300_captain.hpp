#pragma once

#include "n760_250_military.hpp"

class Rucksack;


// 元の名前：ＭａｉｎＴｈｒｅａｄ
//
struct Captain : public Military {

	explicit Captain(Rucksack* s) : Military(s), m_isThinking(true) {}

	virtual void IdleLoop();

	volatile bool m_isThinking;
};
