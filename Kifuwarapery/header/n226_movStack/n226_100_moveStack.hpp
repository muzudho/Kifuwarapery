#pragma once


struct MoveStack {
	Move move;
	int score;
};

// insertionSort() や std::sort() で必要
inline bool operator < (const MoveStack& f, const MoveStack& s) { return f.score < s.score; }
inline bool operator > (const MoveStack& f, const MoveStack& s) { return f.score > s.score; }
