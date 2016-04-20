#include "../../header/n800_learn___/n800_100_stopwatch.hpp"


void Stopwatch::Restart()
{
	this->m_start_ = std::chrono::system_clock::now();
}

// 経過ミリ秒☆？
int Stopwatch::GetElapsed() const
{
	using std::chrono::duration_cast;
	using std::chrono::milliseconds;
	return static_cast<int>(
		duration_cast<milliseconds>(
			std::chrono::system_clock::now() - this->m_start_
		).count()
	);
}

Stopwatch Stopwatch::CreateStopwatchByCurrentTime()
{
	Stopwatch t;
	t.Restart();
	return t;
}
