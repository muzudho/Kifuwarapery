#include "../../header/n223_move____/n223_500_move.hpp"

namespace {
	const std::string HandPieceToStringTable[HandPieceNum] = {"P*", "L*", "N*", "S*", "G*", "B*", "R*"};
	inline std::string handPieceToString(const HandPiece hp) { return HandPieceToStringTable[hp]; }

	const std::string PieceTypeToStringTable[PieceTypeNum] = {
		"", "FU", "KY", "KE", "GI", "KA", "HI", "KI", "OU", "TO", "NY", "NK", "NG", "UM", "RY"
	};
	inline std::string pieceTypeToString(const PieceType pt) { return PieceTypeToStringTable[pt]; }
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
	return GetValue() & m_PromoteFlag;
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
	return (ptFrom + static_cast<PieceType>((GetValue() & m_PromoteFlag) >> 11));
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
	return IsCapture() || (IsPromotion() && GetPieceTypeFrom() == Pawn);
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
	return UtilHandPiece::FromPieceType(GetPieceTypeDropped());
}

bool Move::IsNone() const
{
	return (GetValue() == m_MoveNone);
}

u32 Move::GetValue() const
{
	return value_;
}

Move Move::operator|=(const Move rhs)
{
	this->value_ |= rhs.GetValue();
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
		return handPieceToString(this->GetHandPieceDropped()) + UtilSquare::ToStringUSI(to);
	}
	std::string usi = UtilSquare::ToStringUSI(from) + UtilSquare::ToStringUSI(to);
	if (this->IsPromotion()) { usi += "+"; }
	return usi;
}

std::string Move::ToCSA() const {
	if (this->IsNone()) { return "None"; }

	std::string s = (this->IsDrop() ? std::string("00") : UtilSquare::ToStringCSA(this->From()));
	s += UtilSquare::ToStringCSA(this->To()) + pieceTypeToString(this->GetPieceTypeTo());
	return s;
}

Move Move::GetMoveNone()
{
	return Move(m_MoveNone);
}

Move Move::GetMoveNull()
{
	return Move(m_MoveNull);
}

Move Move::GetMovePVsEnd()
{
	return Move(m_MovePVsEnd);
}
