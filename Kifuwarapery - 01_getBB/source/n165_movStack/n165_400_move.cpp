#include <string>
#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n110_square__/n110_100_square.hpp"
#include "../../header/n110_square__/n110_500_convSquare.hpp"
#include "../../header/n112_pieceTyp/n112_050_pieceType.hpp"
#include "../../header/n113_piece___/n113_105_convPieceType.hpp"
#include "../../header/n113_piece___/n113_200_handPiece.hpp"
#include "../../header/n165_movStack/n165_400_move.hpp"

Move::Move()
{
	// デフォルト・コンストラクタは空っぽだぜ☆（＾ｑ＾）
}

Square Move::To() const
{
	return static_cast<Square>((GetValue() >> 0) & 0x7f);
}

Square Move::From() const
{
	return static_cast<Square>((GetValue() >> 7) & 0x7f);
}

u32 Move::FromAndTo() const
{
	return (GetValue() >> 0) & 0x3fff;
}

u32 Move::ProFromAndTo() const
{
	return (GetValue() >> 0) & 0x7fff;
}

PieceType Move::GetCap() const
{
	return static_cast<PieceType>((GetValue() >> 20) & 0xf);
}

u32 Move::IsPromotion() const
{
	return GetValue() & m_PROMOTE_FLAG;
}

PieceType Move::GetPieceTypeFrom() const
{
	return static_cast<PieceType>((GetValue() >> 16) & 0xf);
}

PieceType Move::GetPieceTypeTo() const
{
	return (IsDrop() ? GetPieceTypeDropped() : GetPieceTypeTo(GetPieceTypeFrom()));
}

PieceType Move::GetPieceTypeTo(const PieceType ptFrom) const
{
	// これらは同じ意味。
#if 1
	return (ptFrom + static_cast<PieceType>((GetValue() & m_PROMOTE_FLAG) >> 11));
#else
	return (IsPromotion()) ? ptFrom + PTPromote : ptFrom;
#endif
}

bool Move::IsDrop() const
{
	return this->From() >= 81;
}

bool Move::IsCapture() const
{
	return (GetValue() & 0xf00000) ? true : false;
}

bool Move::IsCaptureOrPromotion() const
{
	return (GetValue() & 0xf04000) ? true : false;
}

bool Move::IsCaptureOrPawnPromotion() const
{
	return IsCapture() || (IsPromotion() && GetPieceTypeFrom() == N01_Pawn);
}

PieceType Move::GetPieceTypeDropped() const
{
	return static_cast<PieceType>(this->From() - SquareNum + 1);
}

PieceType Move::GetPieceTypeFromOrDropped() const
{
	return (IsDrop() ? GetPieceTypeDropped() : GetPieceTypeFrom());
}

HandPiece Move::GetHandPieceDropped() const
{
	assert(this->IsDrop());
	return ConvHandPiece::FromPieceType(GetPieceTypeDropped());
}

bool Move::IsNone() const
{
	return GetValue() == Move::m_NONE;
}

u32 Move::GetValue() const
{
	return m_value_;
}

Move Move::operator|=(const Move rhs)
{
	this->m_value_ |= rhs.GetValue();
	return *this;
}

Move Move::operator|(const Move rhs) const
{
	return Move(*this) |= rhs;
}

bool Move::operator==(const Move rhs) const
{
	return this->GetValue() == rhs.GetValue();
}

bool Move::operator!=(const Move rhs) const
{
	return !(*this == rhs);
}

bool Move::operator<(const Move rhs) const
{
	return this->GetValue() < rhs.GetValue();
}

std::string Move::GetPromoteFlagToStringUSI() const
{
	return (this->IsPromotion() ? "+" : "");
}

std::string Move::ToUSI() const {
	if (this->IsNone()) { return "None"; }

	const Square from = this->From();
	const Square to = this->To();
	if (this->IsDrop()) {
		return ConvHandPiece::GET_STRING(this->GetHandPieceDropped()) + ConvSquare::TO_STRING_USI40(to);
	}
	std::string usi = ConvSquare::TO_STRING_USI40(from) + ConvSquare::TO_STRING_USI40(to);
	if (this->IsPromotion()) { usi += "+"; }
	return usi;
}

std::string Move::ToCSA() const {
	if (this->IsNone()) { return "None"; }

	std::string s = (this->IsDrop() ? std::string("00") : ConvSquare::TO_STRING_CSA40(this->From()));
	s += ConvSquare::TO_STRING_CSA40(this->To()) + ConvPieceType::GET_STRING(this->GetPieceTypeTo());
	return s;
}
