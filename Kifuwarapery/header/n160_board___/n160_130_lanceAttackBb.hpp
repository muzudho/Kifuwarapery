#pragma once

#include "n160_100_bitboard.hpp"


//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// ��
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
class LanceAttackBb {
public:
	// �������ߖ�������A���ʂȃ������������Ă���B
	Bitboard m_controllBb[ColorNum][SquareNum][128];
	Bitboard m_controllBbToEdge[ColorNum][SquareNum];
	Bitboard m_lanceCheckTable[ColorNum][SquareNum];

public:

	void Initialize();
	void InitCheckTableLance();

	// ��Q���������Ƃ��̗����� Bitboard
	// g_rookAttack, g_bishopAttack, g_lanceAttack ��ݒ肵�Ă���A���̊֐����ĂԂ��ƁB
	void InitializeToEdge();

	// todo: ���Ԃ̋؂��ǂ��ɂ��邩��ɕ������Ă���΁ABitboard �̕Е��̕ϐ������𒲂ׂ�Ηǂ��Ȃ�B
	inline Bitboard GetControllBb(const Bitboard* thisBitboard, const Color c, const Square sq) const {
		const int part = Bitboard::Part(sq);
		const int index = ((*thisBitboard).GetP(part) >> ConfigBits::m_slide[sq]) & 127;
		return this->m_controllBb[c][sq][index];
	}

	inline Bitboard GetControllBbToEdge(const Color c, const Square sq) const {
		return this->m_controllBbToEdge[c][sq];
	}

	inline Bitboard LanceCheckTable(const Color c, const Square sq) const {
		return this->m_lanceCheckTable[c][sq];
	}

private:

	Bitboard LanceBlockMask(const Square square);

	Bitboard LanceAttackCalc(const Color c, const Square square, const Bitboard& occupied);

};
