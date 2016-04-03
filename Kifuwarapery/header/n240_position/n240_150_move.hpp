#pragma once

#include "../../header/n110_square__/n110_500_utilSquare.hpp"
#include "../../header/n113_piece___/n113_205_utilHandPiece.hpp"
#include "../../header/n220_position/n220_500_charToPieceUSI.hpp"

// xxxxxxxx xxxxxxxx xxxxxxxx x1111111  移動先
// xxxxxxxx xxxxxxxx xx111111 1xxxxxxx  移動元。駒打ちの際には、PieceType + SquareNum - 1
// xxxxxxxx xxxxxxxx x1xxxxxx xxxxxxxx  1 なら成り
// xxxxxxxx xxxx1111 xxxxxxxx xxxxxxxx  移動する駒の PieceType 駒打ちの際には使用しない。
// xxxxxxxx 1111xxxx xxxxxxxx xxxxxxxx  取られた駒の PieceType

// todo: piece to move と captured piece は指し手としてパックする必要あるの？
//       from, to , promo だけだったら、16bit で済む。
class Move {
public:

	Move() {}

	explicit Move(const u32 u) : value_(u) {}

	Move& operator = (const Move& m) { value_ = m.value_; return *this; }

	Move& operator = (const volatile Move& m) { value_ = m.value_; return *this; }

	// volatile Move& 型の *this を返すとなぜか警告が出るので、const Move& 型の m を返すことにする。
	const Move& operator = (const Move& m) volatile { value_ = m.value_; return m; }

	Move(const Move& m) { value_ = m.value_; }

	Move(const volatile Move& m) { value_ = m.value_; }

	// 移動先
	Square to() const;

	// 移動元
	Square from() const;

	// 移動元、移動先
	u32 fromAndTo() const;

	// 成り、移動元、移動先
	u32 proFromAndTo() const;

	// 取った駒の種類
	PieceType cap() const;

	// 成るかどうか
	u32 isPromotion() const;

	// 移動する駒の種類
	PieceType pieceTypeFrom() const;

	// 移動した後の駒の種類
	PieceType pieceTypeTo() const;

	// 移動前の PieceType を引数に取り、移動後の PieceType を返す。
	// 高速化の為、ptFrom が確定しているときに使用する。
	PieceType pieceTypeTo(const PieceType ptFrom) const;

	bool isDrop() const;

	// 0xf00000 は 取られる駒のマスク
	bool isCapture() const;

	// 0xf04000 は 取られる駒と成のマスク
	bool isCaptureOrPromotion() const;

	bool isCaptureOrPawnPromotion() const;

	// 打つ駒の種類
	PieceType pieceTypeDropped() const;

	PieceType pieceTypeFromOrDropped() const;

	HandPiece handPieceDropped() const;

	// 値が入っているか。
	bool isNone() const;

	// メンバ変数 value_ の取得
	u32 value() const;

	Move operator |= (const Move rhs);

	Move operator | (const Move rhs) const;

	bool operator == (const Move rhs) const;
	bool operator != (const Move rhs) const;
	bool operator < (const Move rhs) const;
	std::string promoteFlagToStringUSI() const;
	std::string toUSI() const;
	std::string toCSA() const;

	static Move moveNone();
	static Move moveNull();

	// 学習時に、正解の手のPV、その他の手のPVを MoveNone で区切りながら 1 次元配列に格納していく。
	// 格納するその他のPVの最後に MovePVsEnd を格納する。それをフラグに次の指し手に遷移する。
	// 正解のPV, MoveNone, その他0のPV, MoveNone, その他1のPV, MoveNone, MovePVsEnd という感じに並ぶ。
	static Move movePVsEnd();

	static const u32 PromoteFlag = 1 << 14;
	static const u32 MoveNone    = 0;
	static const u32 MoveNull    = 129;
	static const u32 MovePVsEnd  = 1 << 15; // for learn

private:
	u32 value_;
};

// 成り flag
inline Move promoteFlag() { return static_cast<Move>(Move::PromoteFlag); }
inline Move moveNone() { return static_cast<Move>(Move::MoveNone); }

// 移動先から指し手に変換
inline Move to2Move(const Square to) { return static_cast<Move>(to << 0); }
// 移動元から指し手に変換
inline Move from2Move(const Square from) { return static_cast<Move>(from << 7); }

// 駒打ちの駒の種類から移動元に変換
// todo: PieceType を HandPiece に変更
inline Square drop2From(const PieceType pt) { return static_cast<Square>(SquareNum - 1 + pt); }

// 駒打ち(移動元)から指し手に変換
inline Move drop2Move(const PieceType pt) { return static_cast<Move>(drop2From(pt) << 7); }

// 移動する駒の種類から指し手に変換
inline Move pieceType2Move(const PieceType pt) { return static_cast<Move>(pt << 16); }

// 移動元から駒打ちの駒の種類に変換
inline PieceType from2Drop(const Square from) { return static_cast<PieceType>(from - SquareNum + 1); }

// 取った駒の種類から指し手に変換
inline Move capturedPieceType2Move(const PieceType captured) { return static_cast<Move>(captured << 20); }
// 移動先と、Position から 取った駒の種類を判別し、指し手に変換
// 駒を取らないときは、0 (MoveNone) を返す。
inline Move capturedPieceType2Move(const Square to, const Position& pos) {
	const PieceType captured = UtilPiece::pieceToPieceType( pos.piece(to) );
	return capturedPieceType2Move(captured);
}

// 移動元、移動先、移動する駒の種類から指し手に変換
inline Move makeMove(const PieceType pt, const Square from, const Square to) {
	return pieceType2Move(pt) | from2Move(from) | to2Move(to);
}

// 取った駒を判別する必要がある。
// この関数は駒を取らないときにも使える。
inline Move makeCaptureMove(const PieceType pt, const Square from, const Square to, const Position& pos) {
	return capturedPieceType2Move(to, pos) | makeMove(pt, from, to);
}

// makeCaptureMove() かつ 成り
inline Move makeCapturePromoteMove(const PieceType pt, const Square from, const Square to, const Position& pos) {
	return makeCaptureMove(pt, from, to, pos) | promoteFlag();
}

// 駒打ちの makeMove()
// todo: PieceType を HandPiece に変更
inline Move makeDropMove(const PieceType pt, const Square to) { return from2Move(drop2From(pt)) | to2Move(to); }

