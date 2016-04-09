#pragma once


#include "../n223_move____/n223_100_move.hpp"


struct MoveStack {
	Move move;
	int score;
};

// insertionSort() や std::sort() で必要
inline bool operator < (const MoveStack& f, const MoveStack& s) { return f.score < s.score; }
inline bool operator > (const MoveStack& f, const MoveStack& s) { return f.score > s.score; }
