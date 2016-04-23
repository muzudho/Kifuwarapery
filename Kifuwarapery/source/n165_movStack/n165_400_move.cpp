#include <string>
#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n110_square__/n110_100_square.hpp"
#include "../../header/n110_square__/n110_155_convFile.hpp"
#include "../../header/n110_square__/n110_205_convRank.hpp"
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
	return static_cast<Square>((this->GetValue() >> 0) & 0x7f);
}

Square Move::From() const
{
	return static_cast<Square>((this->GetValue() >> 7) & 0x7f);
}

u32 Move::FromAndTo() const
{
	return (this->GetValue() >> 0) & 0x3fff;
}

u32 Move::ProFromAndTo() const
{
	return (this->GetValue() >> 0) & 0x7fff;
}

PieceType Move::GetCap() const
{
	return static_cast<PieceType>((this->GetValue() >> 20) & 0xf);
}

u32 Move::IsPromotion() const
{
	return this->GetValue() & m_PROMOTE_FLAG;
}

PieceType Move::GetPieceTypeFrom() const
{
	return static_cast<PieceType>((this->GetValue() >> 16) & 0xf);
}

PieceType Move::GetPieceTypeTo() const
{
	return (this->IsDrop() ? this->GetPieceTypeDropped() : this->GetPieceTypeTo(this->GetPieceTypeFrom()));
}

PieceType Move::GetPieceTypeTo(const PieceType ptFrom) const
{
	// これらは同じ意味。
#if 1
	return (ptFrom + static_cast<PieceType>((this->GetValue() & m_PROMOTE_FLAG) >> 11));
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
	return (this->GetValue() & 0xf00000) ? true : false;
}

bool Move::IsCaptureOrPromotion() const
{
	return (this->GetValue() & 0xf04000) ? true : false;
}

bool Move::IsCaptureOrPawnPromotion() const
{
	return this->IsCapture() || (this->IsPromotion() && this->GetPieceTypeFrom() == PieceType::N01_Pawn);
}

PieceType Move::GetPieceTypeDropped() const
{
	return static_cast<PieceType>(this->From() - Square::SquareNum + 1);
}

PieceType Move::GetPieceTypeFromOrDropped() const
{
	return (this->IsDrop() ? this->GetPieceTypeDropped() : this->GetPieceTypeFrom());
}

HandPiece Move::GetHandPieceDropped() const
{
	assert(this->IsDrop());
	return ConvHandPiece::FromPieceType(this->GetPieceTypeDropped());
}

bool Move::IsNone() const
{
	return GetValue() == Move::m_NONE;
}

u32 Move::GetValue() const
{
	return this->m_value_;
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

// (^q^)エイプリーはストリング型だったんだが、なのは風のキャラ配列に変えてみるぜ☆
std::string Move::ToUSI() const {

	// （Ａ）指さないとき☆（＾ｑ＾）
	if (this->IsNone()) {
		return "None";
	}
	// （Ｂ）無いとき☆（＾ｑ＾）
	// 未設定ムーブはない。
	//
	// （Ｃ）持ち駒のとき
	else if (this->IsDrop()) {
		const Square to = this->To();

		// （＾ｑ＾）2の倍数がいいのか☆？ｗｗ なのははバッファーサイズを 8byte にしていたが、
		// 構わず　きつきつ　にしてみようぜ☆ｗｗｗｗ
		char buf[5];//8

		// （イ）持ち駒の種類☆（＾ｑ＾）
		buf[0] = g_CONV_HAND_PIECE_1CHAR_TABLE[this->GetHandPieceDropped()];

		// （ロ）持ち駒を打つ表示☆（＾ｑ＾）
		buf[1] = '*';

		// （ハ）移動先の筋☆（＾ｑ＾）
		buf[2] = ConvFile::TO_CHAR_USI10(ConvSquare::TO_FILE10(to));

		// （ニ）移動先の段☆（＾ｑ＾）
		buf[3] = ConvRank::TO_CHAR_USI10(ConvSquare::TO_RANK10(to));

		// （ホ）終端子☆
		buf[4] = '\0';

		return std::string(buf);
	}
	// （Ｄ）成りのとき
	else if (this->IsPromotion()) {

		const Square to = this->To();
		const Square from = this->From();

		// （＾ｑ＾）2の倍数がいいのか☆？ｗｗ なのははバッファーサイズを 8byte にしていたが、
		// 構わず　きつきつ　にしてみようぜ☆ｗｗｗｗ
		char buf[6];//8

		// （イ）移動元の筋☆（＾ｑ＾）
		buf[0] = ConvFile::TO_CHAR_USI10(ConvSquare::TO_FILE10(from));

		// （ロ）移動元の段☆（＾ｑ＾）
		buf[1] = ConvRank::TO_CHAR_USI10(ConvSquare::TO_RANK10(from));

		// （ハ）移動先の筋☆（＾ｑ＾）
		buf[2] = ConvFile::TO_CHAR_USI10(ConvSquare::TO_FILE10(to));

		// （ニ）移動先の段☆（＾ｑ＾）
		buf[3] = ConvRank::TO_CHAR_USI10(ConvSquare::TO_RANK10(to));

		// （ホ）成りだぜ☆（＾ｑ＾）
		buf[4] = '+';

		// （へ）終端子☆
		buf[5] = '\0';

		return std::string(buf);
	}
	// （Ｅ）成らないとき
	else
	{
		const Square to = this->To();
		const Square from = this->From();

		// （＾ｑ＾）2の倍数がいいのか☆？ｗｗ なのははバッファーサイズを 8byte にしていたが、
		// 構わず　きつきつ　にしてみようぜ☆ｗｗｗｗ
		char buf[5];//8

		// （イ）移動元の筋☆（＾ｑ＾）
		buf[0] = ConvFile::TO_CHAR_USI10(ConvSquare::TO_FILE10(from));

		// （ロ）移動元の段☆（＾ｑ＾）
		buf[1] = ConvRank::TO_CHAR_USI10(ConvSquare::TO_RANK10(from));

		// （ハ）移動先の筋☆（＾ｑ＾）
		buf[2] = ConvFile::TO_CHAR_USI10(ConvSquare::TO_FILE10(to));

		// （ニ）移動先の段☆（＾ｑ＾）
		buf[3] = ConvRank::TO_CHAR_USI10(ConvSquare::TO_RANK10(to));

		// （ホ）終端子☆
		buf[4] = '\0';

		return std::string(buf);
	}
}

std::string Move::ToCSA() const {

	if (this->IsNone()) { return "None"; }

	std::string s = (
		this->IsDrop() ?
		std::string("00") :
		ConvSquare::TO_STRING_CSA40(this->From())
	);

	// 移動先の筋☆（＾ｑ＾）
	// 移動先の段☆（＾ｑ＾）
	s += ConvSquare::TO_STRING_CSA40(this->To()) +
		// 駒の種類☆（＾ｑ＾）
		ConvPieceType::GET_STRING(this->GetPieceTypeTo());

	return s;
}
