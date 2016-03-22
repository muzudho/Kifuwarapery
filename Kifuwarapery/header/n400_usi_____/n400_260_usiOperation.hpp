#pragma once

#include "../../header/n400_usi_____/n400_250_usi.hpp"


class UsiOperation {
public:

	void go(const Position& pos, std::istringstream& ssCmd);
#if defined LEARN
	void go(const Position& pos, const Ply depth, const Move move);
#endif

	void setPosition(Position& pos, std::istringstream& ssCmd);

#if !defined NDEBUG
	// for debug
	Move usiToMoveDebug(const Position& pos, const std::string& moveStr);
	Move csaToMoveDebug(const Position& pos, const std::string& moveStr);
#endif

	Move csaToMove(const Position& pos, const std::string& moveStr);
	Move usiToMove(const Position& pos, const std::string& moveStr);

private:
	Move usiToMoveBody(const Position& pos, const std::string& moveStr);
	Move csaToMoveBody(const Position& pos, const std::string& moveStr);
};
