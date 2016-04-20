#pragma once

#include "n760_250_thread.hpp"

class Rucksack;


struct MainThread : public Thread {

	explicit MainThread(Rucksack* s) : Thread(s), m_isThinking(true) {}

	virtual void IdleLoop();

	volatile bool m_isThinking;
};
