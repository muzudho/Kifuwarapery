#pragma once


struct MoveStack {
	Move move;
	int score;
};

// insertionSort() ‚â std::sort() ‚Å•K—v
inline bool operator < (const MoveStack& f, const MoveStack& s) { return f.score < s.score; }
inline bool operator > (const MoveStack& f, const MoveStack& s) { return f.score > s.score; }
