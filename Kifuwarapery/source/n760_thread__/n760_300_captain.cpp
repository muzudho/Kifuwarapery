#include "../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_030_moveMaker_ExceptPromote.hpp"
#include "../../header/n760_thread__/n760_250_military.hpp"
#include "../../header/n760_thread__/n760_300_captain.hpp"
#include "../../header/n885_searcher/n885_040_rucksack.hpp"


void Captain::IdleLoop() {
	while (true) {// エグジットするまで　ずっといるぜ☆
		{
			std::unique_lock<Mutex> lock(this->m_sleepLock);
			this->m_isThinking = false;
			while (!this->m_isThinking && !m_exit) {
				// UI 関連だから要らないのかも。
				this->m_pRucksack->m_ownerHerosPub.m_sleepCond_.notify_one();
				this->m_sleepCond.wait(lock);
			}
		}

		if (this->m_exit) {
			return;
		}

		this->m_searching = true;
		Hitchhiker::Think(*this->m_pRucksack);
		assert(this->m_searching);
		this->m_searching = false;
	}
}
