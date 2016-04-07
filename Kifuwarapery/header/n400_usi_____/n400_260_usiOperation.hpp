#pragma once


class UsiOperation {
public:

	void Go(const Position& pos, std::istringstream& ssCmd);
#if defined LEARN
	void Go(const Position& GetPos, const Ply GetDepth, const Move GetMove);
#endif

	void SetPosition(Position& pos, std::istringstream& ssCmd);

#if !defined NDEBUG
	// for debug
	Move usiToMoveDebug(const Position& GetPos, const std::string& moveStr);
	Move csaToMoveDebug(const Position& GetPos, const std::string& moveStr);
#endif

	Move CsaToMove(const Position& pos, const std::string& moveStr);
	Move UsiToMove(const Position& pos, const std::string& moveStr);

private:
	Move UsiToMoveBody(const Position& pos, const std::string& moveStr);
	Move CsaToMoveBody(const Position& pos, const std::string& moveStr);
};
