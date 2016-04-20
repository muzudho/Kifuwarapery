#include "../../header/n220_position/n220_670_makePromoteMove.hpp"
#include "../../header/n760_thread__/n760_250_military.hpp"
#include "../../header/n760_thread__/n760_300_captain.hpp"
#include "../../header/n885_searcher/n885_500_rucksack.hpp"


void Captain::IdleLoop() {
	while (true) {// エグジットするまで　ずっといるぜ☆
		{
			std::unique_lock<Mutex> lock(this->m_sleepLock);
			this->m_isThinking = false;
			while (!this->m_isThinking && !m_exit) {
				// UI 関連だから要らないのかも。
				this->m_pSearcher->m_ownerHerosPub.m_sleepCond_.notify_one();
				this->m_sleepCond.wait(lock);
			}
		}

		if (this->m_exit) {
			return;
		}

		this->m_searching = true;
		this->m_pSearcher->Think();
		assert(this->m_searching);
		this->m_searching = false;
	}
}
