#pragma once

#include "../../header/n110_square__/n110_500_utilSquare.hpp"
#include "../../header/n113_piece___/n113_205_utilHandPiece.hpp"
#include "../../header/n220_position/n220_500_charToPieceUSI.hpp"

// xxxxxxxx xxxxxxxx xxxxxxxx x1111111  �ړ���
// xxxxxxxx xxxxxxxx xx111111 1xxxxxxx  �ړ����B��ł��̍ۂɂ́APieceType + SquareNum - 1
// xxxxxxxx xxxxxxxx x1xxxxxx xxxxxxxx  1 �Ȃ琬��
// xxxxxxxx xxxx1111 xxxxxxxx xxxxxxxx  �ړ������� PieceType ��ł��̍ۂɂ͎g�p���Ȃ��B
// xxxxxxxx 1111xxxx xxxxxxxx xxxxxxxx  ���ꂽ��� PieceType

// todo: piece to move �� captured piece �͎w����Ƃ��ăp�b�N����K�v����́H
//       from, to , promo ������������A16bit �ōςށB
class Move {
public:

	Move() {}

	explicit Move(const u32 u) : value_(u) {}

	Move& operator = (const Move& m) { value_ = m.value_; return *this; }

	Move& operator = (const volatile Move& m) { value_ = m.value_; return *this; }

	// volatile Move& �^�� *this ��Ԃ��ƂȂ����x�����o��̂ŁAconst Move& �^�� m ��Ԃ����Ƃɂ���B
	const Move& operator = (const Move& m) volatile { value_ = m.value_; return m; }

	Move(const Move& m) { value_ = m.value_; }

	Move(const volatile Move& m) { value_ = m.value_; }

	// �ړ���
	Square To() const;

	// �ړ���
	Square From() const;

	// �ړ����A�ړ���
	u32 FromAndTo() const;

	// ����A�ړ����A�ړ���
	u32 ProFromAndTo() const;

	// �������̎��
	PieceType GetCap() const;

	// ���邩�ǂ���
	u32 IsPromotion() const;

	// �ړ������̎��
	PieceType GetPieceTypeFrom() const;

	// �ړ�������̋�̎��
	PieceType GetPieceTypeTo() const;

	// �ړ��O�� PieceType �������Ɏ��A�ړ���� PieceType ��Ԃ��B
	// �������ׁ̈AptFrom ���m�肵�Ă���Ƃ��Ɏg�p����B
	PieceType GetPieceTypeTo(const PieceType ptFrom) const;

	bool IsDrop() const;

	// 0xf00000 �� ������̃}�X�N
	bool IsCapture() const;

	// 0xf04000 �� ������Ɛ��̃}�X�N
	bool IsCaptureOrPromotion() const;

	bool IsCaptureOrPawnPromotion() const;

	// �ł�̎��
	PieceType GetPieceTypeDropped() const;

	PieceType GetPieceTypeFromOrDropped() const;

	HandPiece GetHandPieceDropped() const;

	// �l�������Ă��邩�B
	bool IsNone() const;

	// �����o�ϐ� value_ �̎擾
	u32 GetValue() const;

	Move operator |= (const Move rhs);

	Move operator | (const Move rhs) const;

	bool operator == (const Move rhs) const;
	bool operator != (const Move rhs) const;
	bool operator < (const Move rhs) const;
	std::string GetPromoteFlagToStringUSI() const;
	std::string ToUSI() const;
	std::string ToCSA() const;

	static Move GetMoveNone();
	static Move GetMoveNull();

	// �w�K���ɁA�����̎��PV�A���̑��̎��PV�� MoveNone �ŋ�؂�Ȃ��� 1 �����z��Ɋi�[���Ă����B
	// �i�[���邻�̑���PV�̍Ō�� MovePVsEnd ���i�[����B������t���O�Ɏ��̎w����ɑJ�ڂ���B
	// ������PV, MoveNone, ���̑�0��PV, MoveNone, ���̑�1��PV, MoveNone, MovePVsEnd �Ƃ��������ɕ��ԁB
	static Move GetMovePVsEnd();

	static const u32 m_PromoteFlag = 1 << 14;
	static const u32 m_MoveNone = 0;
	static const u32 m_MoveNull = 129;
	static const u32 m_MovePVsEnd = 1 << 15; // for learn

private:
	u32 value_;
};
