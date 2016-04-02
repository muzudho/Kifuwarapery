#include "../../header/n240_position/n240_150_move.hpp"

namespace {
	const std::string HandPieceToStringTable[HandPieceNum] = {"P*", "L*", "N*", "S*", "G*", "B*", "R*"};
	inline std::string handPieceToString(const HandPiece hp) { return HandPieceToStringTable[hp]; }

	const std::string PieceTypeToStringTable[PieceTypeNum] = {
		"", "FU", "KY", "KE", "GI", "KA", "HI", "KI", "OU", "TO", "NY", "NK", "NG", "UM", "RY"
	};
	inline std::string pieceTypeToString(const PieceType pt) { return PieceTypeToStringTable[pt]; }
}

Square Move::to() const
{
	return static_cast<Square>((value() >> 0) & 0x7f);
}

Square Move::from() const
{
	return static_cast<Square>((value() >> 7) & 0x7f);
}

u32 Move::fromAndTo() const
{
	return (value() >> 0) & 0x3fff;
}

u32 Move::proFromAndTo() const
{
	return (value() >> 0) & 0x7fff;
}

PieceType Move::cap() const
{
	return static_cast<PieceType>((value() >> 20) & 0xf);
}

u32 Move::isPromotion() const
{
	return value() & PromoteFlag;
}

PieceType Move::pieceTypeFrom() const
{
	return static_cast<PieceType>((value() >> 16) & 0xf);
}

PieceType Move::pieceTypeTo() const
{
	return (isDrop() ? pieceTypeDropped() : pieceTypeTo(pieceTypeFrom()));
}

PieceType Move::pieceTypeTo(const PieceType ptFrom) const
{
	// これらは同じ意味。
#if 1
	return (ptFrom + static_cast<PieceType>((value() & PromoteFlag) >> 11));
#else
	return (isPromotion()) ? ptFrom + PTPromote : ptFrom;
#endif
}

bool Move::isDrop() const
{
	return this->from() >= 81;
}

bool Move::isCapture() const
{
	return (value() & 0xf00000) ? true : false;
}

bool Move::isCaptureOrPromotion() const
{
	return (value() & 0xf04000) ? true : false;
}

bool Move::isCaptureOrPawnPromotion() const
{
	return isCapture() || (isPromotion() && pieceTypeFrom() == Pawn);
}

PieceType Move::pieceTypeDropped() const
{
	return static_cast<PieceType>(this->from() - SquareNum + 1);
}

PieceType Move::pieceTypeFromOrDropped() const
{
	return (isDrop() ? pieceTypeDropped() : pieceTypeFrom());
}

HandPiece Move::handPieceDropped() const
{
	assert(this->isDrop());
	return pieceTypeToHandPiece(pieceTypeDropped());
}

bool Move::isNone() const
{
	return (value() == MoveNone);
}

u32 Move::value() const
{
	return value_;
}

Move Move::operator|=(const Move rhs)
{
	this->value_ |= rhs.value();
	return *this;
}

Move Move::operator|(const Move rhs) const
{
	return Move(*this) |= rhs;
}

bool Move::operator==(const Move rhs) const
{
	return this->value() == rhs.value();
}

bool Move::operator!=(const Move rhs) const
{
	return !(*this == rhs);
}

bool Move::operator<(const Move rhs) const
{
	return this->value() < rhs.value();
}

std::string Move::promoteFlagToStringUSI() const
{
	return (this->isPromotion() ? "+" : "");
}

std::string Move::toUSI() const {
	if (this->isNone()) { return "None"; }

	const Square from = this->from();
	const Square to = this->to();
	if (this->isDrop()) {
		return handPieceToString(this->handPieceDropped()) + UtilSquare::SquareToStringUSI(to);
	}
	std::string usi = UtilSquare::SquareToStringUSI(from) + UtilSquare::SquareToStringUSI(to);
	if (this->isPromotion()) { usi += "+"; }
	return usi;
}

std::string Move::toCSA() const {
	if (this->isNone()) { return "None"; }

	std::string s = (this->isDrop() ? std::string("00") : UtilSquare::squareToStringCSA(this->from()));
	s += UtilSquare::squareToStringCSA(this->to()) + pieceTypeToString(this->pieceTypeTo());
	return s;
}

Move Move::moveNone()
{
	return Move(MoveNone);
}

Move Move::moveNull()
{
	return Move(MoveNull);
}

Move Move::movePVsEnd()
{
	return Move(MovePVsEnd);
}
