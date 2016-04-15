#include "../../header/n220_position/n220_670_makePromoteMove.hpp"
#include "../../header/n760_thread__/n760_250_thread.hpp"
#include "../../header/n760_thread__/n760_300_mainThread.hpp"
#include "../../header/n885_searcher/n885_500_searcher.hpp"


void MainThread::IdleLoop() {
	while (true) {
		{
			std::unique_lock<Mutex> lock(m_sleepLock);
			m_isThinking = false;
			while (!m_isThinking && !m_exit) {
				// UI 関連だから要らないのかも。
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
