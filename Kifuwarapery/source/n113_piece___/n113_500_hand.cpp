#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n113_piece___/n113_205_utilHandPiece.hpp"
#include "../../header/n113_piece___/n113_500_hand.hpp"

const int Hand::m_HandPieceShiftBits[HandPieceNum] = {
	m_HPawnShiftBits,
	m_HLanceShiftBits,
	m_HKnightShiftBits,
	m_HSilverShiftBits,
	m_HGoldShiftBits,
	m_HBishopShiftBits,
	m_HRookShiftBits
};
const u32 Hand::m_HandPieceMask[HandPieceNum] = {
	m_HPawnMask,
	m_HLanceMask,
	m_HKnightMask,
	m_HSilverMask,
	m_HGoldMask,
	m_HBishopMask,
	m_HRookMask
};
// 特定の種類の持ち駒を 1 つ増やしたり減らしたりするときに使用するテーブル
const u32 Hand::m_HandPieceOne[HandPieceNum] = {
	1 << m_HPawnShiftBits,
	1 << m_HLanceShiftBits,
	1 << m_HKnightShiftBits,
	1 << m_HSilverShiftBits,
	1 << m_HGoldShiftBits,
	1 << m_HBishopShiftBits,
	1 << m_HRookShiftBits
};

u32 Hand::Value() const
{
	return this->m_value_;
}

u32 Hand::NumOf(const HandPiece handPiece) const
{
	return (Value() & m_HandPieceMask[handPiece]) >> m_HandPieceShiftBits[handPiece];
}

u32 Hand::Exists(const HandPiece handPiece) const
{
	return Value() & m_HandPieceMask[handPiece];
}

u32 Hand::ExceptPawnExists() const
{
	return Value() & m_HandPieceExceptPawnMask;
}

void Hand::OrEqual(const int num, const HandPiece handPiece)
{
	m_value_ |= num << m_HandPieceShiftBits[handPiece];
}

void Hand::PlusOne(const HandPiece handPiece)
{
	m_value_ += m_HandPieceOne[handPiece];
}

void Hand::MinusOne(const HandPiece handPiece)
{
	m_value_ -= m_HandPieceOne[handPiece];
}

bool Hand::operator==(const Hand rhs) const
{
	return this->Value() == rhs.Value();
}

bool Hand::operator!=(const Hand rhs) const
{
	return this->Value() != rhs.Value();
}

bool Hand::IsEqualOrSuperior(const Hand ref) const
{
#if 0
	// 全ての駒が ref 以上の枚数なので、true
	return (ref.Exists<HKnight>() <= this->Exists<HKnight>()
		&& ref.Exists<HSilver>() <= this->Exists<HSilver>()
		&& ref.Exists<HGold  >() <= this->Exists<HGold  >()
		&& ref.Exists<HBishop>() <= this->Exists<HBishop>()
		&& ref.Exists<HRook  >() <= this->Exists<HRook  >());
#else
	// こちらは、同じ意味でより高速
	// ref の方がどれか一つでも多くの枚数の駒を持っていれば、Borrow の位置のビットが立つ。
	return ((this->Value() - ref.Value()) & m_BorrowMask) == 0;
#endif
}

