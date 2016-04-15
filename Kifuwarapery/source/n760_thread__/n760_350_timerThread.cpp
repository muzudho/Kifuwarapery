#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n760_thread__/n760_350_timerThread.hpp"
#include "../../header/n885_searcher/n885_500_searcher.hpp"


void TimerThread::IdleLoop() {
	while (!m_exit) {
		{
			std::unique_lock<Mutex> lock(m_sleepLock);
			if (!m_exit) {
				m_sleepCond.wait_for(lock, std::chrono::milliseconds(m_msec ? m_msec : INT_MAX));
			}
		}
		if (m_msec) {
			m_pSearcher->CheckTime();
		}
	}
}
