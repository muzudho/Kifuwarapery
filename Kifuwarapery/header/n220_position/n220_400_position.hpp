#pragma once

#include <stack>
#include "../n116_hand____/n116_500_hand.hpp"
#include "../n160_board___/n160_200_bitboardMask.hpp"
#include "../n200_score___/n200_200_pieceScore.hpp"
#include "../n220_position/n220_350_stateInfo.hpp"
#include "../n240_position/n240_200_evalList.hpp"

using StateStackPtr = std::unique_ptr<std::stack<StateInfo> >;

class Move;
struct Thread;
struct Searcher;

class Position {
public:
	Position() {}

	explicit Position(Searcher* s) : m_searcher_(s) {}

	Position(const Position& pos) { *this = pos; }

	Position(const Position& pos, Thread* th) {
		*this = pos;
		m_thisThread_ = th;
	}

	Position(const std::string& sfen, Thread* th, Searcher* s) {
		Set(sfen, th);
		setSearcher(s);
	}

	Position& operator = (const Position& pos);

	void Set(const std::string& sfen, Thread* th);

	Bitboard GetBbOf(const PieceType pt) const { return m_byTypeBB_[pt]; }
	Bitboard GetBbOf(const Color c) const { return m_byColorBB_[c]; }
	Bitboard GetBbOf(const PieceType pt, const Color c) const { return this->GetBbOf(pt) & this->GetBbOf(c); }
	Bitboard GetBbOf(const PieceType pt1, const PieceType pt2) const { return this->GetBbOf(pt1) | this->GetBbOf(pt2); }
	Bitboard GetBbOf(const PieceType pt1, const PieceType pt2, const Color c) const { return this->GetBbOf(pt1, pt2) & this->GetBbOf(c); }
	Bitboard GetBbOf(const PieceType pt1, const PieceType pt2, const PieceType pt3) const { return this->GetBbOf(pt1, pt2) | this->GetBbOf(pt3); }
	Bitboard GetBbOf(const PieceType pt1, const PieceType pt2, const PieceType pt3, const PieceType pt4) const {
		return this->GetBbOf(pt1, pt2, pt3) | this->GetBbOf(pt4);
	}
	Bitboard GetBbOf(const PieceType pt1, const PieceType pt2, const PieceType pt3,
		const PieceType pt4, const PieceType pt5) const
	{
		return this->GetBbOf(pt1, pt2, pt3, pt4) | this->GetBbOf(pt5);
	}

	Bitboard GetOccupiedBB() const { return this->GetBbOf(Occupied); }
	// emptyBB() よりもわずかに速いはず。
	// emptyBB() とは異なり、全く使用しない位置(0 から数えて、right の 63bit目、left の 18 ~ 63bit目)
	// の bit が 1 になっても構わないとき、こちらを使う。
	// todo: SSEにビット反転が無いので実はそんなに速くないはず。不要。
	Bitboard GetNOccupiedBB() const { return ~GetOccupiedBB(); }
	Bitboard GetEmptyBB() const { return GetOccupiedBB() ^ Bitboard::AllOneBB(); }
	// 金、成り金 の Bitboard
	Bitboard GetGoldsBB() const { return m_goldsBB_; }
	Bitboard GetGoldsBB(const Color c) const { return GetGoldsBB() & this->GetBbOf(c); }

	Piece GetPiece(const Square sq) const { return m_piece_[sq]; }

	// hand
	Hand GetHand(const Color c) const { return m_hand_[c]; }

	// turn() 側が pin されている Bitboard を返す。
	// checkersBB が更新されている必要がある。
	Bitboard GetPinnedBB() const { return hiddenCheckers<true, true>(); }
	// 間の駒を動かすことで、turn() 側が空き王手が出来る駒のBitboardを返す。
	// checkersBB が更新されている必要はない。
	// BetweenIsUs == true  : 間の駒が自駒。
	// BetweenIsUs == false : 間の駒が敵駒。
	template <bool BetweenIsUs = true>
	Bitboard DiscoveredCheckBB() const { return hiddenCheckers<false, BetweenIsUs>(); }

	// toFile と同じ筋に us の歩がないなら true
	bool NoPawns(const Color us, const File toFile) const { return !this->GetBbOf(Pawn, us).AndIsNot0(BitboardMask::GetFileMask(toFile)); }
	bool IsPawnDropCheckMate(const Color us, const Square sq) const;

	// Pinされているfromの駒がtoに移動出来なければtrueを返す。
	template <bool IsKnight = false>
	bool isPinnedIllegal(const Square from, const Square to, const Square ksq, const Bitboard& pinned) const {
		// 桂馬ならどこに動いても駄目。
		return pinned.IsSet(from) && (IsKnight || !UtilSquare::IsAligned<true>(from, to, ksq));
	}
	// 空き王手かどうか。
	template <bool IsKnight = false>
	bool isDiscoveredCheck(const Square from, const Square to, const Square ksq, const Bitboard& dcBB) const {
		// 桂馬ならどこに動いても空き王手になる。
		return dcBB.IsSet(from) && (IsKnight || !UtilSquare::IsAligned<true>(from, to, ksq));
	}

	Bitboard checkersBB() const { return m_st_->m_checkersBB; }
	Bitboard prevCheckersBB() const { return m_st_->m_previous->m_checkersBB; }
	// 王手が掛かっているか。
	bool inCheck() const { return checkersBB().IsNot0(); }

	Score material() const { return m_st_->m_material; }
	Score materialDiff() const { return m_st_->m_material - m_st_->m_previous->m_material; }

	FORCE_INLINE Square kingSquare(const Color c) const {
		assert(m_kingSquare_[c] == this->GetBbOf(King, c).ConstFirstOneFromI9());
		return m_kingSquare_[c];
	}

	bool moveGivesCheck(const Move m) const;
	bool moveGivesCheck(const Move move, const CheckInfo& ci) const;

	// attacks
	Bitboard attackersTo(const Square sq, const Bitboard& occupied) const;
	Bitboard attackersTo(const Color c, const Square sq) const { return attackersTo(c, sq, GetOccupiedBB()); }
	Bitboard attackersTo(const Color c, const Square sq, const Bitboard& occupied) const;
	Bitboard attackersToExceptKing(const Color c, const Square sq) const;
	// todo: 利きをデータとして持ったとき、attackersToIsNot0() を高速化すること。
	bool attackersToIsNot0(const Color c, const Square sq) const { return attackersTo(c, sq).IsNot0(); }
	bool attackersToIsNot0(const Color c, const Square sq, const Bitboard& occupied) const {
		return attackersTo(c, sq, occupied).IsNot0();
	}
	// 移動王手が味方の利きに支えられているか。false なら相手玉で取れば詰まない。
	bool unDropCheckIsSupported(const Color c, const Square sq) const { return attackersTo(c, sq).IsNot0(); }
	// 利きの生成

	// 任意の occupied に対する利きを生成する。
	template <PieceType PT> static Bitboard attacksFrom(const Color c, const Square sq, const Bitboard& occupied);
	// 任意の occupied に対する利きを生成する。
	template <PieceType PT> Bitboard attacksFrom(const Square sq, const Bitboard& occupied) const {
		static_assert(PT == Bishop || PT == Rook || PT == Horse || PT == Dragon, "");
		// Color は何でも良い。
		return attacksFrom<PT>(ColorNum, sq, occupied);
	}

	template <PieceType PT> Bitboard attacksFrom(const Color c, const Square sq) const {
		static_assert(PT == Gold, ""); // Gold 以外は template 特殊化する。
		return Bitboard::GoldAttack(c, sq);
	}
	template <PieceType PT> Bitboard attacksFrom(const Square sq) const {
		static_assert(PT == Bishop || PT == Rook || PT == King || PT == Horse || PT == Dragon, "");
		// Color は何でも良い。
		return attacksFrom<PT>(ColorNum, sq);
	}
	Bitboard attacksFrom(const PieceType pt, const Color c, const Square sq) const { return attacksFrom(pt, c, sq, GetOccupiedBB()); }
	static Bitboard attacksFrom(const PieceType pt, const Color c, const Square sq, const Bitboard& occupied);

	// 次の手番
	Color turn() const { return m_turn_; }

	// pseudoLegal とは
	// ・玉が相手駒の利きがある場所に移動する
	// ・pin の駒を移動させる
	// ・連続王手の千日手の手を指す
	// これらの反則手を含めた手の事と定義する。
	// よって、打ち歩詰めや二歩の手は pseudoLegal では無い。
	template <bool MUSTNOTDROP, bool FROMMUSTNOTBEKING>
	bool pseudoLegalMoveIsLegal(const Move move, const Bitboard& pinned) const;
	bool pseudoLegalMoveIsEvasion(const Move move, const Bitboard& pinned) const;
	// checkPawnDrop : 二歩と打ち歩詰めも調べるなら true
	bool moveIsPseudoLegal(const Move move, const bool checkPawnDrop = false) const;
#if !defined NDEBUG
	bool moveIsLegal(const Move move) const;
#endif

	void doMove(const Move move, StateInfo& newSt);
	void doMove(const Move move, StateInfo& newSt, const CheckInfo& ci, const bool moveIsCheck);
	void undoMove(const Move move);
	template <bool DO> void doNullMove(StateInfo& backUpSt);

	Score see(const Move move, const int asymmThreshold = 0) const;
	Score seeSign(const Move move) const;

	template <Color US> Move mateMoveIn1Ply();
	Move mateMoveIn1Ply();

	Ply gamePly() const { return m_gamePly_; }

	Key getBoardKey() const { return m_st_->m_boardKey; }
	Key getHandKey() const { return m_st_->m_handKey; }
	Key getKey() const { return m_st_->GetKey(); }
	Key getExclusionKey() const { return m_st_->GetKey() ^ m_zobExclusion_; }
	Key getKeyExcludeTurn() const {
		static_assert(m_zobTurn_ == 1, "");
		return getKey() >> 1;
	}
	void print() const;

	u64 nodesSearched() const { return m_nodes_; }
	void setNodesSearched(const u64 n) { m_nodes_ = n; }
	RepetitionType isDraw(const int checkMaxPly = std::numeric_limits<int>::max()) const;

	Thread* thisThread() const { return m_thisThread_; }

	void setStartPosPly(const Ply ply) { m_gamePly_ = ply; }

	static constexpr int nlist() { return EvalList::ListSize; }
	int list0(const int index) const { return m_evalList_.list0[index]; }
	int list1(const int index) const { return m_evalList_.list1[index]; }
	int squareHandToList(const Square sq) const { return m_evalList_.squareHandToList[sq]; }
	Square listToSquareHand(const int i) const { return m_evalList_.listToSquareHand[i]; }
	int* plist0() { return &m_evalList_.list0[0]; }
	int* plist1() { return &m_evalList_.list1[0]; }
	const int* cplist0() const { return &m_evalList_.list0[0]; }
	const int* cplist1() const { return &m_evalList_.list1[0]; }
	const ChangedLists& cl() const { return m_st_->m_cl; }

	const Searcher* csearcher() const { return m_searcher_; }
	Searcher* searcher() const { return m_searcher_; }
	void setSearcher(Searcher* s) { m_searcher_ = s; }

#if !defined NDEBUG
	// for debug
	bool isOK() const;
#endif

	static void initZobrist();

	static Score pieceScore(const Piece pc) { return g_PieceScore[pc]; }
	// Piece を index としても、 PieceType を index としても、
	// 同じ値が取得出来るようにしているので、PieceType => Piece への変換は必要ない。
	static Score pieceScore(const PieceType pt) { return g_PieceScore[pt]; }
	static Score capturePieceScore(const Piece pc) { return g_CapturePieceScore[pc]; }
	// Piece を index としても、 PieceType を index としても、
	// 同じ値が取得出来るようにしているので、PieceType => Piece への変換は必要ない。
	static Score capturePieceScore(const PieceType pt) { return g_CapturePieceScore[pt]; }
	static Score promotePieceScore(const PieceType pt) {
		assert(pt < Gold);
		return g_PromotePieceScore[pt];
	}

private:
	void clear();
	void setPiece(const Piece piece, const Square sq) {
		const Color c = UtilPiece::ToColor(piece);
		const PieceType pt = UtilPiece::ToPieceType(piece);

		m_piece_[sq] = piece;

		m_byTypeBB_[pt].SetBit(sq);
		m_byColorBB_[c].SetBit(sq);
		m_byTypeBB_[Occupied].SetBit(sq);
	}
	void setHand(const HandPiece hp, const Color c, const int num) { m_hand_[c].OrEqual(num, hp); }
	void setHand(const Piece piece, const int num) {
		const Color c = UtilPiece::ToColor(piece);
		const PieceType pt = UtilPiece::ToPieceType(piece);
		const HandPiece hp = UtilHandPiece::FromPieceType(pt);
		setHand(hp, c, num);
	}

	// 手番側の玉へ check している駒を全て探して checkersBB_ にセットする。
	// 最後の手が何か覚えておけば、attackersTo() を使用しなくても良いはずで、処理が軽くなる。
	void findCheckers() { m_st_->m_checkersBB = attackersToExceptKing(UtilColor::OppositeColor(turn()), kingSquare(turn())); }

	Score computeMaterial() const;

	void xorBBs(const PieceType pt, const Square sq, const Color c);
	// turn() 側が
	// pin されて(して)いる駒の Bitboard を返す。
	// BetweenIsUs == true  : 間の駒が自駒。
	// BetweenIsUs == false : 間の駒が敵駒。
	template <bool FindPinned, bool BetweenIsUs> Bitboard hiddenCheckers() const {
		Bitboard result = Bitboard::AllZeroBB();
		const Color us = turn();
		const Color them = UtilColor::OppositeColor(us);
		// pin する遠隔駒
		// まずは自駒か敵駒かで大雑把に判別
		Bitboard pinners = this->GetBbOf(FindPinned ? them : us);

		const Square ksq = kingSquare(FindPinned ? us : them);

		// 障害物が無ければ玉に到達出来る駒のBitboardだけ残す。
		pinners &= (this->GetBbOf(Lance) & Bitboard::LanceAttackToEdge((FindPinned ? us : them), ksq)) |
			(this->GetBbOf(Rook, Dragon) & Bitboard::RookAttackToEdge(ksq)) | (this->GetBbOf(Bishop, Horse) & Bitboard::BishopAttackToEdge(ksq));

		while (pinners.IsNot0()) {
			const Square sq = pinners.FirstOneFromI9();
			// pin する遠隔駒と玉の間にある駒の位置の Bitboard
			const Bitboard between = Bitboard::BetweenBB(sq, ksq) & GetOccupiedBB();

			// pin する遠隔駒と玉の間にある駒が1つで、かつ、引数の色のとき、その駒は(を) pin されて(して)いる。
			if (between.IsNot0()
				&& between.IsOneBit<false>()
				&& between.AndIsNot0(this->GetBbOf(BetweenIsUs ? us : them)))
			{
				result |= between;
			}
		}

		return result;
	}

#if !defined NDEBUG
	int debugSetEvalList() const;
#endif
	void setEvalList() { m_evalList_.set(*this); }

	Key computeBoardKey() const;
	Key computeHandKey() const;
	Key computeKey() const { return computeBoardKey() + computeHandKey(); }

	void printHand(const Color c) const;

	static Key zobrist(const PieceType pt, const Square sq, const Color c) { return m_zobrist_[pt][sq][c]; }
	static Key zobTurn() { return m_zobTurn_; }
	static Key zobHand(const HandPiece hp, const Color c) { return m_zobHand_[hp][c]; }


	// byTypeBB は敵、味方の駒を区別しない。
	// byColorBB は駒の種類を区別しない。
	Bitboard m_byTypeBB_[PieceTypeNum];
	Bitboard m_byColorBB_[ColorNum];
	Bitboard m_goldsBB_;

	// 各マスの状態
	Piece m_piece_[SquareNum];
	Square m_kingSquare_[ColorNum];

	// 手駒
	Hand m_hand_[ColorNum];
	Color m_turn_;

	EvalList m_evalList_;

	StateInfo m_startState_;
	StateInfo* m_st_;
	// 時間管理に使用する。
	Ply m_gamePly_;
	Thread* m_thisThread_;
	u64 m_nodes_;

	Searcher* m_searcher_;

	static Key m_zobrist_[PieceTypeNum][SquareNum][ColorNum];
	static const Key m_zobTurn_ = 1;
	static Key m_zobHand_[HandPieceNum][ColorNum];
	static Key m_zobExclusion_; // todo: これが必要か、要検討
};
