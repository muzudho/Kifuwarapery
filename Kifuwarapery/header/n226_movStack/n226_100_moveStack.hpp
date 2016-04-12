#pragma once


#include "../n223_move____/n223_100_move.hpp"


struct MoveStack {

	Move m_move;

	int m_score;

};

// insertionSort() や std::sort() で必要
inline bool operator < (const MoveStack& f, const MoveStack& s) { return f.m_score < s.m_score; }
inline bool operator > (const MoveStack& f, const MoveStack& s) { return f.m_score > s.m_score; }
