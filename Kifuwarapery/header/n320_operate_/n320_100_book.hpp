#pragma once

#include "../../header/n240_position/n240_100_position.hpp"
#include "../../header/n240_position/n240_400_MoveScore.hpp"
#include "../../header/n280_move____/n280_200_mt64bit.hpp"

struct BookEntry {
	Key key;
	u16 fromToPro;
	u16 count;
	Score score;
};

class Book : private std::ifstream {
public:
	Book() : random_(std::chrono::system_clock::now().time_since_epoch().count()) {}
	MoveScore probe(const Position& pos, const std::string& fName, const bool pickBest);
	static void init();
	static Key bookKey(const Position& pos);

private:
	bool open(const char* fName);
	void binary_search(const Key key);

	static MT64bit mt64bit_; // 定跡のhash生成用なので、seedは固定でデフォルト値を使う。
	MT64bit random_; // 時刻をseedにして色々指すようにする。
	std::string fileName_;
	size_t size_;

	static Key ZobPiece[PieceNone][SquareNum];
	static Key ZobHand[HandPieceNum][19];
	static Key ZobTurn;
};

void makeBook(Position& pos, std::istringstream& ssCmd);

