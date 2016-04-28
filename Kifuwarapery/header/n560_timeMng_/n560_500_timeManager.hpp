#pragma once

#include "../n520_evaluate/n520_700_evaluation09.hpp"
#include "../n560_timeMng_/n560_100_limitsOfThinking.hpp"

class Rucksack;


class TimeManager {
public:

	// 元の名前：Ｉｎｉｔ
	// Think開始時に初期設定をするぜ☆（＾▽＾）
	void InitializeTimeManager_OnHitchhikerThinkStarted(
		bool& isMoveTime0Clear,
		const LimitsOfThinking& limits, // m_moveTimeを 0にする場合があるぜ☆（＾ｑ＾）
		const Ply currentPly,
		const Color us,
		Rucksack* pRucksack
		);

	// 本譜の情勢不安定さ☆？をセットするぜ☆
	void SetPvInstability(const int currChanges, const int prevChanges);

	// 使っていい時間をオーバーしちゃったかどうかだぜ☆（＾ｑ＾）最初の指し手のときに使うぜ☆
	inline bool CanThinking01_TimeOver_AtFirstMove(const int elapsed) { return this->GetTukatteiiTime() < elapsed; }

	// まだ反復深化していいか☆？（＾ｑ＾）
	inline bool CanThinking02_TimeOk_ForIterativeDeepingLoop(const int elapsed) {
		// 使っていい時間の40%が、消費時間未満の場合。
		return this->GetTukatteiiTime() * 40 / 100 < elapsed;
	}

	// 新しく生まれてくる下級戦士の寿命☆（＾ｑ＾）
	inline int GetWarriorLifeTime() { return this->GetTukatteiiTime() / 16; }

	inline bool CanNotNextIteration(const int elapsed) {
		// 使っていい時間の62%が、消費時間（ミリ秒）に満たない場合。
		return (this->GetTukatteiiTime() * 62) / 100 < elapsed;
	}

	// これ以上の時間がないか☆？（＾ｑ＾）
	inline bool IsNoMoreTime(const int timerResolution, const int elapsed) {
		return this->GetSaidaiEnchoTime() - 2 * timerResolution < elapsed;
	}

private:

	//────────────────────────────────────────────────────────────────────────────────
	// 使っていい思考時間
	//────────────────────────────────────────────────────────────────────────────────
	// 元の名前：ＡｖａｉｌａｂｌｅＴｉｍｅ
	// なんだかんだで、使っちゃうつもりの時間☆（＾ｑ＾）
	int GetTukatteiiTime() const {
		return this->GetYoteiSikoTime() + this->GetSikoAsobiTime();
	}

	//────────────────────────────────────────────────────────────────────────────────
	// 今回使おうと思っている思考時間
	//────────────────────────────────────────────────────────────────────────────────
	inline int GetYoteiSikoTime() const {
		return this->m_yoteiSikoTime_;
	}
	inline void SetYoteiSikoTime(int value) {
		this->m_yoteiSikoTime_ = value;
	}
	inline void IncreaseYoteiSikoTime(int value) {
		this->m_yoteiSikoTime_ += value;
	}
	// 少なくなっていた場合、更新します。
	inline void SmallUpdate_YoteiSikoTime(int value) {
		this->SetYoteiSikoTime( std::min(
			this->GetYoteiSikoTime(), // 元の値
			value
			));
	}
	// 大きくなっていた場合、更新します。
	inline void LargeUpdate_YoteiSikoTime(int value) {
		this->SetYoteiSikoTime( std::max(this->m_yoteiSikoTime_, value));
	}

	//────────────────────────────────────────────────────────────────────────────────
	// 思考の最大延長時間
	//────────────────────────────────────────────────────────────────────────────────
	inline int GetSaidaiEnchoTime() const {
		return this->m_saidaiEnchoTime_;
	}
	inline void SetSaidaiEnchoTime(int value) {
		this->m_saidaiEnchoTime_ = value;
	}
	inline void IncreaseSaidaiEnchoTime(int value) {
		this->m_saidaiEnchoTime_ += value;
	}
	// 少なくなっていた場合、更新します。
	inline void SmallUpdate_SaidaiEnchoTime(int value) {
		this->m_saidaiEnchoTime_ = std::min(
			this->m_saidaiEnchoTime_, // マキシマム時間
			value // 思考時間＋残り時間
			);
	}

	//────────────────────────────────────────────────────────────────────────────────
	// 思考の遊び時間
	//────────────────────────────────────────────────────────────────────────────────
	inline int GetSikoAsobiTime() const {
		return this->m_sikoAsobiTime_;
	}
	inline void SetSikoAsobiTime(int value) {
		this->m_sikoAsobiTime_ = value;
	}
	inline void ZeroClearSikoAsobiTime() {
		this->m_sikoAsobiTime_ = 0;
	}

private:

	//────────────────────────────────────────────────────────────────────────────────
	// 今回使おうと思っている思考時間
	//────────────────────────────────────────────────────────────────────────────────
	// 元の名前：ＯｐｔｉｍｕｍＳｅａｒｃｈＴｉｍｅ
	// 理想的な思考時間？　（何かの単位のようだぜ☆）
	int m_yoteiSikoTime_;

	//────────────────────────────────────────────────────────────────────────────────
	// 思考の最大延長時間
	//────────────────────────────────────────────────────────────────────────────────
	// 元の名前：ＭａｘｉｍｕｍＳｅａｒｃｈＴｉｍｅ
	int m_saidaiEnchoTime_;

	//────────────────────────────────────────────────────────────────────────────────
	// 思考の遊び時間
	//────────────────────────────────────────────────────────────────────────────────
	// 元の名前：ｕｎｓｔａｂｌｅＰＶＥｘｔｒａＴｉｍｅ
	int m_sikoAsobiTime_;
};

