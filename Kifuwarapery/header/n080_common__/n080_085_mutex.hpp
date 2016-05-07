#pragma once


#if defined _WIN32 && !defined _MSC_VER
// �c�O�Ȃ��ƂɁAstd::mutex �������Ƃ���
#else
// ���b�L�[�Ȃ��ƂɁAstd::mutex ���L��Ƃ���
#include <condition_variable>	// std::mutex
using namespace std;
#endif



//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// �~���[�e�b�N�X
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
#if defined _WIN32 && !defined _MSC_VER
// �c�O�Ȃ��ƂɁAstd::mutex �������Ƃ���
struct Mutex {
	Mutex() { InitializeCriticalSection(&cs); }
	~Mutex() { DeleteCriticalSection(&cs); }
	void lock() { EnterCriticalSection(&cs); }
	void unlock() { LeaveCriticalSection(&cs); }

private:
	CRITICAL_SECTION cs;
};
#else
// ���b�L�[�Ȃ��ƂɁAstd::mutex ���L��Ƃ���
using Mutex = std::mutex;
#endif
