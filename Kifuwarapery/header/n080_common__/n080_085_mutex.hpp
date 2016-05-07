#pragma once


#if defined _WIN32 && !defined _MSC_VER
// 残念なことに、std::mutex が無いとき☆
#else
// ラッキーなことに、std::mutex が有るとき☆
#include <condition_variable>	// std::mutex
using namespace std;
#endif



//────────────────────────────────────────────────────────────────────────────────
// ミューテックス
//────────────────────────────────────────────────────────────────────────────────
#if defined _WIN32 && !defined _MSC_VER
// 残念なことに、std::mutex が無いとき☆
struct Mutex {
	Mutex() { InitializeCriticalSection(&cs); }
	~Mutex() { DeleteCriticalSection(&cs); }
	void lock() { EnterCriticalSection(&cs); }
	void unlock() { LeaveCriticalSection(&cs); }

private:
	CRITICAL_SECTION cs;
};
#else
// ラッキーなことに、std::mutex が有るとき☆
using Mutex = std::mutex;
#endif
