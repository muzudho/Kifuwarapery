#pragma once

#include "../n080_common__/n080_100_common.hpp"	//u32
#include "../n105_color___/n105_100_color.hpp"	//ColorNum
#include "../n119_score___/n119_090_scoreIndex.hpp"	//Ply

// 元の名前：ＬｉｍｉｔｓＴｙｐｅ
// 時間や探索深さの制限を格納する為の構造体
class LimitsOfThinking {
public:

	LimitsOfThinking() {
		//memset(this, 0, sizeof(LimitsOfThinking));
	}

	// 元の名前：　ｕｓｅＴｉｍｅＭａｎａｇｅｍｅｎｔ
	// 時間管理が新品かどうか☆（＾ｑ＾）？
	// 反復深化探索に潜るために真であることが必要☆
	bool IsBrandnewTimeManagement() const {
		// 以下の全てが 0 の場合に真☆？（＾ｑ＾）
		return !(
			this->m_depth
			|
			this->m_nodes01
			|
			this->GetMoveTime()
			|
			static_cast<int>(this->m_infinite)
			);
	}

	// 深さ☆？
	Ply m_depth = 0;

	u32 m_nodes01 = 0;


	bool m_infinite = false;

	bool m_ponder = false;


public:


	//────────────────────────────────────────────────────────────────────────────────
	// インクリメントとは何なのか☆？（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	// フィッシャータイムの追加時間☆？ということにしておこう☆（＾ｑ＾）
	int m_increment[ColorNum] = { 0 };
	inline int GetIncrement(Color us) const {
		return this->m_increment[us];
	}


	//────────────────────────────────────────────────────────────────────────────────
	// 残り時間だぜ☆（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	// 元の名前：ｔｉｍｅ
	// ミリ秒単位だぜ☆（＾ｑ＾）
	int m_nokoriTime[ColorNum] = { 0 };
	inline int GetNokoriTime(Color us)const {
		return this->m_nokoriTime[us];
	}
	// btime, wtime コマンドをそのまま受け取るぜ☆（＾ｑ＾）
	inline void SetNokoriTimeByStream(Color us, std::istringstream& ssCmd) {
		ssCmd >> this->m_nokoriTime[us];
	}

	//────────────────────────────────────────────────────────────────────────────────
	// ムーブス・ツー・ゴーって何☆？（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	// FIXME: movesToGo というプロパティを持っていたが、"movesToGo" なんていうコマンドは USI から飛んでこないと思ったので削除したぜ☆（＾ｑ＾）
	// 時間管理の movesToGo の最大値だったみたいだぜ☆

	//────────────────────────────────────────────────────────────────────────────────
	// ムーブタイムって何☆？（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	inline int GetMoveTime() const {
		return this->m_moveTime_;
	}
	// 思考開始時、初期化を行った際にある条件下で０クリアする☆？
	inline void ZeroClearMoveTime() {
		this->m_moveTime_ = 0;
	}
	// ポンダーヒットをしたときに、ムーブタイムが０でなければ、消費した時間分、加算するのに使います。
	inline void IncreaseMoveTime(int value) {
		this->m_moveTime_ += value;
	}
	// go byoyomi 等の場合に、マージン分減らすのに使う☆？
	inline void DecrementMoveTime(int value) {
		this->m_moveTime_ -= value;
	}
	inline void SetMoveTimeFromStream(std::istringstream& ssCmd) {
		ssCmd >> this->m_moveTime_;
	}

private:

	// ☆？ ポンダー・ヒットの経過時間が足しこまれる☆？（＾ｑ＾）
	// 秒読みマージンは引かれる☆
	int m_moveTime_ = 0;

};
