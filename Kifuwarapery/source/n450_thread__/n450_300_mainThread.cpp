#include "../../header/n276_genMove_/n276_250_makePromoteMove.hpp"
#include "../../header/n320_searcher/n320_550_search.hpp"
#include "../../header/n450_thread__/n450_250_thread.hpp"
#include "../../header/n450_thread__/n450_300_mainThread.hpp"
//#include "../../header/n450_thread__/n450_400_threadPool.hpp"


void MainThread::IdleLoop() {
	while (true) {
		{
			std::unique_lock<Mutex> lock(m_sleepLock);
			m_isThinking = false;
			while (!m_isThinking && !m_exit) {
				// UI �֘A������v��Ȃ��̂����B
				m_pSearcher->m_threads.m_sleepCond_.notify_one();
				m_sleepCond.wait(lock);
			}
		}

		if (m_exit) {
			return;
		}

		m_searching = true;
		m_pSearcher->Think();
		assert(m_searching);
		m_searching = false;
	}
}
