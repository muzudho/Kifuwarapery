#pragma once


#include "../n105_color___/n105_100_color.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n113_piece___/n113_500_hand.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"


class DropMakerEvent {
public:
	void* m_pDontUseThisMember__;// FIXME: ／（＾ｑ＾）＼これを消すと bench コマンドの実行時に強制終了してしまう☆ 使ってないけど置いておく必要があるぜ☆（＾～＾）

public:

	const Color m_us;
	const Position& m_pos;
	const Bitboard& m_target;
	const Hand& m_hand;
	const int m_haveHandNum;
	const int m_noKnightIdx;
	const int m_noKnightLanceIdx;
	const Bitboard& m_tRank8BB;
	const Bitboard& m_tRank9BB;
	//PieceType m_haveHandArr[6];//(^q^)配列はコンストラクタに渡せないぜ☆！

public:

	DropMakerEvent(
		const Color us,
		const Position& pos,
		const Bitboard& target,
		const Hand& hand,
		const int haveHandNum,
		const int noKnightIdx,
		const int noKnightLanceIdx,
		const Bitboard& tRank8BB,
		const Bitboard& tRank9BB
		//PieceType haveHandArr[6]
	) :
		m_us(us),
		m_pos(pos),
		m_target(target),
		m_hand(hand),
		m_haveHandNum(haveHandNum),
		m_noKnightIdx(m_noKnightIdx),
		m_noKnightLanceIdx(m_noKnightLanceIdx),
		m_tRank8BB(tRank8BB),
		m_tRank9BB(tRank9BB)
	{
		//m_haveHandArr = haveHandArr;
	}

};