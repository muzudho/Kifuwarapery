#pragma once

#include "n760_250_military.hpp"

class Rucksack;


// 元の名前：ＭａｉｎＴｈｒｅａｄ
//
class Captain : public Military {
public:

	explicit Captain(Rucksack* s) : Military(s), m_isThinking(true) {}

	virtual void IdleLoop();

	volatile bool m_isThinking;
};
