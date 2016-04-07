#pragma once


#include "n160_050_configBits.hpp"
#include "n160_100_bitboard.hpp"


extern ConfigBits g_configBits;


//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// ��
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
class LanceAttackBb {
public:
	// �������ߖ�������A���ʂȃ������������Ă���B
	Bitboard m_controllBb[ColorNum][SquareNum][128];

private:
	// �������ߖ�������A���ʂȃ������������Ă���B
	Bitboard m_controllBbToEdge_[ColorNum][SquareNum];
	Bitboard m_lanceCheckTable_[ColorNum][SquareNum];

public:

	void Initialize();
	void InitCheckTableLance();

	// ��Q���������Ƃ��̗����� Bitboard
	// g_rookAttack, g_bishopAttack, g_lanceAttack ��ݒ肵�Ă���A���̊֐����ĂԂ��ƁB
	void InitializeToEdge();

	// todo: ���Ԃ̋؂��ǂ��ɂ��邩��ɕ������Ă���΁ABitboard �̕Е��̕ϐ������𒲂ׂ�Ηǂ��Ȃ�B
	inline Bitboard GetControllBb(const Bitboard* thisBitboard, const Color c, const Square sq) const {
		const int part = Bitboard::Part(sq);
		const int index = ((*thisBitboard).GetP(part) >> g_configBits.m_slide[sq]) & 127;
		return this->m_controllBb[c][sq][index];
	}

	inline Bitboard GetControllBbToEdge(const Color c, const Square sq) const {
		return this->m_controllBbToEdge_[c][sq];
	}

	inline Bitboard LanceCheckTable(const Color c, const Square sq) const {
		return this->m_lanceCheckTable_[c][sq];
	}

private:

	Bitboard LanceBlockMask(const Square square);

	Bitboard LanceAttackCalc(const Color c, const Square square, const Bitboard& occupied);

};


extern LanceAttackBb g_lanceAttackBb;
