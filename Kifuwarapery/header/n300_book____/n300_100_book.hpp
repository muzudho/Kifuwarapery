#pragma once

#include "../n220_position/n220_500_charToPieceUSI.hpp"
#include "../n223_move____/n223_300_moveScore.hpp"
#include "../n280_move____/n280_200_mt64bit.hpp"

struct BookEntry {
	Key		m_key;
	u16		m_fromToPro;
	u16		m_count;
	Score	m_score;
};

class Book : private std::ifstream {
public:

	Book() : m_random_(std::chrono::system_clock::now().time_since_epoch().count()) {}

	MoveScore GetProbe(const Position& pos, const std::string& fName, const bool pickBest);

	static void Init();

	static Key GetBookKey(const Position& pos);

private:
	bool Open(const char* fName);
	void Binary_search(const Key key);

	static MT64bit	m_mt64bit_; // 定跡のhash生成用なので、seedは固定でデフォルト値を使う。
	MT64bit			m_random_; // 時刻をseedにして色々指すようにする。
	std::string		m_fileName_;
	size_t			m_size_;

	static Key m_ZobPiece[PieceNone][SquareNum];
	static Key m_ZobHand[HandPieceNum][19];
	static Key m_ZobTurn;
};

void MakeBook(Position& pos, std::istringstream& ssCmd);

