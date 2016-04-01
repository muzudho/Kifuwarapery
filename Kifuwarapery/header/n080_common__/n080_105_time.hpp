#pragma once

// ミリ秒単位の時間を表すクラス
class Time {
public:

	void restart();

	int elapsed() const;

	static Time currentTime();

private:
	std::chrono::time_point<std::chrono::system_clock> t_;
};
