#include "..\..\header\n080_common__\n080_105_time.hpp"


void Time::Restart()
{
	this->m_start_ = std::chrono::system_clock::now();
}

int Time::Elapsed() const
{
	using std::chrono::duration_cast;
	using std::chrono::milliseconds;
	return static_cast<int>(
		duration_cast<milliseconds>(
			std::chrono::system_clock::now() - this->m_start_
		).count()
	);
}

Time Time::CurrentTime()
{
	Time t;
	t.Restart();
	return t;
}
