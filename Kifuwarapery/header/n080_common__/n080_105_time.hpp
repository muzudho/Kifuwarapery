#pragma once

#include <chrono>	// std::chrono
using namespace std;

// �~���b�P�ʂ̎��Ԃ�\���N���X
class Time {
public:

	// �X�g�b�v�E�E�H�b�`�̍ċN��
	void Restart();

	// �o�ߎ���
	int Elapsed() const;

	// ���ݎ����ŊJ�n�����X�g�b�v�E�E�H�b�`�̎擾
	static Time CurrentTime();

private:
	// �J�n����
	std::chrono::time_point<std::chrono::system_clock> m_start_;
};
