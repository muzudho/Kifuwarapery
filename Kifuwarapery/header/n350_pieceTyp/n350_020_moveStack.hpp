#pragma once


#include "../n165_movStack/n165_400_move.hpp" //TODO:


struct MoveStack {

	Move m_move;

	int m_score;

};

// insertionSort() や std::sort() で必要
inline bool operator < (const MoveStack& f, const MoveStack& s) { return f.m_score < s.m_score; }
inline bool operator > (const MoveStack& f, const MoveStack& s) { return f.m_score > s.m_score; }
