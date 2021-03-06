#pragma once


#include <iostream>
using namespace std;


//────────────────────────────────────────────────────────────────────────────────
// 同期入出力 (Apery_Twig から持ってきたもの)
//────────────────────────────────────────────────────────────────────────────────
enum SyncCout {
	IOLock,
	IOUnlock
};
std::ostream& operator << (std::ostream& os, SyncCout sc);
#define SYNCCOUT std::cout << IOLock
#define SYNCENDL std::endl << IOUnlock

#if defined LEARN
#undef SYNCCOUT
#undef SYNCENDL
class Eraser {};
extern Eraser SYNCCOUT;
extern Eraser SYNCENDL;
template <typename T> Eraser& operator << (Eraser& temp, const T&) { return temp; }
#endif
