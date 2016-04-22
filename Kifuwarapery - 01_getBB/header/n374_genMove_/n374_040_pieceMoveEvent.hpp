﻿#pragma once


#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_300_moveType.hpp"
#include "../n220_position/n220_650_position.hpp"


class PieceMoveEvent {
public:
	void* m_pDontUseThisMember__;// FIXME: ／（＾ｑ＾）＼これを消すと bench コマンドの実行時に強制終了してしまう☆ 使ってないけど置いておく必要があるぜ☆（＾～＾）

public:

	const MoveType m_mt;
	const Color m_us;
	const bool m_all;
	const Position& m_pos;
	//const Bitboard& m_target;
	const Square m_ksq;

public:
	PieceMoveEvent (
		const MoveType mt,
		const Color us,
		const bool all,
		const Position& pos,
		//const Bitboard& target,
		const Square ksq
	):
		m_mt(mt),
		m_us(us),
		m_all(all),
		m_pos(pos),
		//m_target(target),
		m_ksq(ksq)
	{
	}

};