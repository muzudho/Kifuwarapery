#pragma once

// �~���b�P�ʂ̎��Ԃ�\���N���X
class Time {
public:

	void restart();

	int elapsed() const;

	static Time currentTime();

private:
	std::chrono::time_point<std::chrono::system_clock> t_;
};
