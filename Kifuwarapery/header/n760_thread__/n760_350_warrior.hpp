#pragma once


#include "n760_250_military.hpp"

class Rucksack;


// 元の名前：　ＴｉｍｅｒＴｈｒｅａｄ
class Warrior : public Military {
public:

	explicit Warrior(Rucksack* s) : Military(s), m_lifetimeMilliseconds(0) {}

	virtual void IdleLoop()override;

	// ｍｓｅｃはミリ秒☆？ 0にするとタイマーが止まるらしい☆（＾ｑ＾）？
	int m_lifetimeMilliseconds;
};
