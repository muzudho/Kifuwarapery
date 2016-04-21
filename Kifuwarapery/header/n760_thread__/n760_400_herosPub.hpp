#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n223_move____/n223_040_nodeType.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "../n480_tt______/n480_300_tt.hpp"
#include "../n520_evaluate/n520_700_evaluation09.hpp"
#include "../n560_timeMng_/n560_100_limitsOfThinking.hpp"
#include "../n640_searcher/n640_440_splitedNode.hpp"
#include "n760_250_military.hpp"
#include "n760_300_captain.hpp"
#include "n760_350_warrior.hpp"

const int g_MaxThreads = 64;

class Rucksack;


// 元の名前：　ＴｈｒｅａｄＰｏｏｌ
// ベクター型。
class HerosPub : public std::vector<Military*> {
public:
	// 初期化？
	void Init(Rucksack* s);

	// 終了？
	void Exit();

	// メインスレッド？
	Captain* GetFirstCaptain() { return static_cast<Captain*>((*this)[0]); }

	// 最小の深さ？
	Depth GetMinSplitDepth() const { return m_minimumSplitDepth_; }

	// タイマースレッド？
	Warrior* GetCurrWarrior() { return this->m_pWarrior_; }

	// 起きろ？
	void WakeUp(Rucksack* rucksack);
	// 寝ろ？
	void Sleep();


	// USIオプションを読め？
	void ReadUSIOptions(Rucksack* s);

	// スレーブ？
	Military* GetAvailableSlave(Military* master) const;

	// タイマー・スレッドをセット？
	void SetCurrWorrior(const int maxPly);//ｍａｘＰｌｙ＝ｍｓｅｃ

	// 考えが終わるまで待て？
	void WaitForThinkFinished();

	// 考えを始めろ？
	void StartThinking(const Position& pos, const LimitsOfThinking& limits,
					   const std::vector<Move>& searchMoves);

public:
	// 寝ているか？
	bool m_isSleepWhileIdle_;

	// マックス・スレッド？
	size_t m_maxThreadsPerSplitedNode_;

	// ミューテックス？
	Mutex m_mutex_;

	// 寝ている条件？
	ConditionVariable m_sleepCond_;

private:

	// タイマースレッド☆
	Warrior* m_pWarrior_;

	Depth m_minimumSplitDepth_;
};

