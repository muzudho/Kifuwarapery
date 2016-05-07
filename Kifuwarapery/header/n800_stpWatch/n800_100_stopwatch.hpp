#pragma once


#include <chrono>	// std::chrono
using namespace std;


//────────────────────────────────────────────────────────────────────────────────
// ミリ秒単位の時間を表すクラス (Apery_Twig から持ってきたもの)
//────────────────────────────────────────────────────────────────────────────────
class Stopwatch {
public:

	// ストップ・ウォッチの再起動
	void Restart();

	// 経過時間
	int GetElapsed() const;

	// 現在時刻で開始したストップ・ウォッチの取得
	static Stopwatch CreateStopwatchByCurrentTime();

private:
	// 開始時間（ミリ秒？）
	std::chrono::time_point<std::chrono::system_clock> m_start_;
};
