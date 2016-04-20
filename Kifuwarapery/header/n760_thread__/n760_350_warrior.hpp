#pragma once


#include "n760_250_thread.hpp"

class Rucksack;


// 元の名前：　ＴｉｍｅｒＴｈｒｅａｄ
struct Warrior : public Thread {

	explicit Warrior(Rucksack* s) : Thread(s), m_msec(0) {}

	virtual void IdleLoop();

	int m_msec;
};
