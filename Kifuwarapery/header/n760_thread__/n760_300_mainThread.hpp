﻿#pragma once

#include "n760_250_thread.hpp"

class Searcher;


struct MainThread : public Thread {

	explicit MainThread(Searcher* s) : Thread(s), m_isThinking(true) {}

	virtual void IdleLoop();

	volatile bool m_isThinking;
};