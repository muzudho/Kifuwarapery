#include "../../header/n276_genMove_/n276_140_makePromoteMove.hpp"
#include "../../header/n450_thread__/n450_250_thread.hpp"
#include "../../header/n450_thread__/n450_350_timerThread.hpp"
#include "../../header/n900_main____/n900_200_searcher.hpp"		//TODO: ‚±‚ê‚ğŠO‚·‚Ì‚ª‚Ş‚¸‚©‚µ‚¢B


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
