#include "..\..\header\n080_common__\n080_105_time.hpp"

void Time::restart()
{
	t_ = std::chrono::system_clock::now();
}

int Time::elapsed() const
{
	using std::chrono::duration_cast;
	using std::chrono::milliseconds;
	return static_cast<int>(duration_cast<milliseconds>(std::chrono::system_clock::now() - t_).count());
}

Time Time::currentTime()
{
	Time t;
	t.restart();
	return t;
}
