#pragma once


#include <string>
#include <sstream>
#include <fstream>
#include "../n080_common__/n080_100_common.hpp"
#include "../n113_piece___/n113_150_piece.hpp"
#include "../n223_move____/n223_300_moveAndScoreIndex.hpp"
#include "../n600_book____/n600_100_mt64bit.hpp"


class Position;


class Book : private std::ifstream {
public:

	Book() : m_random_(std::chrono::system_clock::now().time_since_epoch().count()) {}

	MoveAndScoreIndex GetProbe(const Position& pos, const std::string& fName, const bool pickBest);

	static void Init();

	static Key GetBookKey(const Position& pos);

private:

	// 旧名：Ｏｐｅｎ
	bool OpenBook(const char* fName);

	void Binary_search(const Key key);

	static MT64bit	m_mt64bit_; // 定跡のhash生成用なので、seedは固定でデフォルト値を使う。

	MT64bit			m_random_; // 時刻をseedにして色々指すようにする。

	std::string		m_fileName_;

	size_t			m_size_;

	static Key m_ZobPiece[N31_PieceNone][SquareNum];

	static Key m_ZobHand[HandPieceNum][19];

	static Key m_ZobTurn;

};

void MakeBook(Position& pos, std::istringstream& ssCmd);

