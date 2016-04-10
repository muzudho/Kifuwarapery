#pragma once


#include "n450_250_thread.hpp"

class Searcher;


struct TimerThread : public Thread {
	explicit TimerThread(Searcher* s) : Thread(s), m_msec(0) {}
	virtual void IdleLoop();
	int m_msec;
};
