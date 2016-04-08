#include "../../header/n276_genMove_/n276_250_makePromoteMove.hpp"
#include "../../header/n320_searcher/n320_150_search.hpp"
#include "../../header/n450_thread__/n450_250_thread.hpp"
#include "../../header/n450_thread__/n450_350_timerThread.hpp"
//#include "../../header/n450_thread__/n450_400_threadPool.hpp"


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
