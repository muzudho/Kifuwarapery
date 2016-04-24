#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n760_thread__/n760_350_warrior.hpp"
#include "../../header/n885_searcher/n885_040_rucksack.hpp"


void Warrior::IdleLoop() {
	while (!this->m_exit) {
		{
			std::unique_lock<Mutex> lock(this->m_sleepLock);
			if (!this->m_exit) {
				// ずっと寝る？？（＾ｑ＾）？
				m_sleepCond.wait_for(lock, std::chrono::milliseconds(
					this->m_lifetimeMilliseconds ? this->m_lifetimeMilliseconds : INT_MAX));
			}
		}
		if (this->m_lifetimeMilliseconds) {
			this->m_pRucksack->CheckTime();
		}
	}
}
