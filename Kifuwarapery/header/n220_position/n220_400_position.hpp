#pragma once

#include <stack>
#include "../n116_hand____/n116_500_hand.hpp"
#include "../n160_board___/n160_400_printBb.hpp"
#include "../n160_board___/n160_500_bitboardMask.hpp"
#include "../n119_score___/n119_200_pieceScore.hpp"
#include "../n220_position/n220_350_stateInfo.hpp"
#include "../n220_position/n220_600_evalList.hpp"

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
		SetSearcher(s);
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
	Bitboard GetPinnedBB() const { return GetHiddenCheckers<true, true>(); }
	// 間の駒を動かすことで、turn() 側が空き王手が出来る駒のBitboardを返す。
	// checkersBB が更新されている必要はない。
	// BetweenIsUs == true  : 間の駒が自駒。
	// BetweenIsUs == false : 間の駒が敵駒。
	template <bool BetweenIsUs = true>
	Bitboard DiscoveredCheckBB() const { return GetHiddenCheckers<false, BetweenIsUs>(); }

	// toFile と同じ筋に us の歩がないなら true
	bool NoPawns(const Color us, const File toFile) const {
		return !this->GetBbOf(Pawn, us).AndIsNot0( BitboardMask::GetFileMask(toFile));
	}
	bool IsPawnDropCheckMate(const Color us, const Square sq) const;

	// Pinされているfromの駒がtoに移動出来なければtrueを返す。
	template <bool IsKnight = false>
	bool IsPinnedIllegal(const Square from, const Square to, const Square ksq, const Bitboard& pinned) const {
		// 桂馬ならどこに動いても駄目。
		return g_setMaskBb.IsSet(&pinned,from) && (IsKnight || !UtilSquare::IsAligned<true>(from, to, ksq));
	}
	// 空き王手かどうか。
	template <bool IsKnight = false>
	bool IsDiscoveredCheck(const Square from, const Square to, const Square ksq, const Bitboard& dcBB) const {
		// 桂馬ならどこに動いても空き王手になる。
		return g_setMaskBb.IsSet(&dcBB,from) && (IsKnight || !UtilSquare::IsAligned<true>(from, to, ksq));
	}

	Bitboard GetCheckersBB() const { return m_st_->m_checkersBB; }
	Bitboard GetPrevCheckersBB() const { return m_st_->m_previous->m_checkersBB; }
	// 王手が掛かっているか。
	bool InCheck() const { return GetCheckersBB().IsNot0(); }

	Score GetMaterial() const { return m_st_->m_material; }
	Score GetMaterialDiff() const { return m_st_->m_material - m_st_->m_previous->m_material; }

	FORCE_INLINE Square GetKingSquare(const Color c) const {
		assert(m_kingSquare_[c] == this->GetBbOf(King, c).ConstFirstOneFromI9());
		return m_kingSquare_[c];
	}

	bool IsMoveGivesCheck(const Move m) const;
	bool IsMoveGivesCheck(const Move move, const CheckInfo& ci) const;

	// attacks
	Bitboard GetAttackersTo(const Square sq, const Bitboard& occupied) const;
	Bitboard GetAttackersTo(const Color c, const Square sq) const { return GetAttackersTo(c, sq, GetOccupiedBB()); }
	Bitboard GetAttackersTo(const Color c, const Square sq, const Bitboard& occupied) const;
	Bitboard GetAttackersToExceptKing(const Color c, const Square sq) const;
	// todo: 利きをデータとして持ったとき、attackersToIsNot0() を高速化すること。
	bool IsAttackersToIsNot0(const Color c, const Square sq) const { return GetAttackersTo(c, sq).IsNot0(); }
	bool IsAttackersToIsNot0(const Color c, const Square sq, const Bitboard& occupied) const {
		return GetAttackersTo(c, sq, occupied).IsNot0();
	}
	// 移動王手が味方の利きに支えられているか。false なら相手玉で取れば詰まない。
	bool IsUnDropCheckIsSupported(const Color c, const Square sq) const { return GetAttackersTo(c, sq).IsNot0(); }
	// 利きの生成

	// 任意の occupied に対する利きを生成する。
	template <PieceType PT>
	static Bitboard GetAttacksFrom(const Color c, const Square sq, const Bitboard& occupied);

	// 任意の occupied に対する利きを生成する。
	template <PieceType PT>
	Bitboard GetAttacksFrom(const Square sq, const Bitboard& occupied) const {
		static_assert(PT == Bishop || PT == Rook || PT == Horse || PT == Dragon, "");
		// Color は何でも良い。
		return GetAttacksFrom<PT>(ColorNum, sq, occupied);
	}

	template <PieceType PT>
	Bitboard GetAttacksFrom(const Color c, const Square sq) const {
		static_assert(PT == Gold, ""); // Gold 以外は template 特殊化する。
		return g_goldAttackBb.GoldAttack(c, sq);
	}
	template <PieceType PT>
	Bitboard GetAttacksFrom(const Square sq) const {
		static_assert(PT == Bishop || PT == Rook || PT == King || PT == Horse || PT == Dragon, "");
		// Color は何でも良い。
		return GetAttacksFrom<PT>(ColorNum, sq);
	}
	Bitboard GetAttacksFrom(const PieceType pt, const Color c, const Square sq) const { return GetAttacksFrom(pt, c, sq, GetOccupiedBB()); }
	static Bitboard GetAttacksFrom(const PieceType pt, const Color c, const Square sq, const Bitboard& occupied);

	// 次の手番
	Color GetTurn() const { return m_turn_; }

	// pseudoLegal とは
	// ・玉が相手駒の利きがある場所に移動する
	// ・pin の駒を移動させる
	// ・連続王手の千日手の手を指す
	// これらの反則手を含めた手の事と定義する。
	// よって、打ち歩詰めや二歩の手は pseudoLegal では無い。
	template <bool MUSTNOTDROP, bool FROMMUSTNOTBEKING>
	bool IsPseudoLegalMoveIsLegal(const Move move, const Bitboard& pinned) const;

	bool IsPseudoLegalMoveIsEvasion(const Move move, const Bitboard& pinned) const;

	// checkPawnDrop : 二歩と打ち歩詰めも調べるなら true
	bool MoveIsPseudoLegal(const Move move, const bool checkPawnDrop = false) const;

#if !defined NDEBUG
	bool MoveIsLegal(const Move GetMove) const;
#endif

	void DoMove(const Move move, StateInfo& newSt);
	void DoMove(const Move move, StateInfo& newSt, const CheckInfo& ci, const bool moveIsCheck);
	void UndoMove(const Move move);
	template <bool DO>
	void DoNullMove(StateInfo& backUpSt);

	Score GetSee(const Move move, const int asymmThreshold = 0) const;
	Score GetSeeSign(const Move move) const;

	template <Color US>
	Move GetMateMoveIn1Ply();
	Move GetMateMoveIn1Ply();

	Ply GetGamePly() const { return m_gamePly_; }

	Key GetBoardKey() const { return m_st_->m_boardKey; }
	Key GetHandKey() const { return m_st_->m_handKey; }
	Key GetKey() const { return m_st_->GetKey(); }
	Key GetExclusionKey() const { return m_st_->GetKey() ^ m_zobExclusion_; }
	Key GetKeyExcludeTurn() const {
		static_assert(m_zobTurn_ == 1, "");
		return GetKey() >> 1;
	}
	void Print() const;

	u64 GetNodesSearched() const { return m_nodes_; }
	void SetNodesSearched(const u64 n) { m_nodes_ = n; }
	RepetitionType IsDraw(const int checkMaxPly = std::numeric_limits<int>::max()) const;

	Thread* GetThisThread() const { return m_thisThread_; }

	void SetStartPosPly(const Ply ply) { m_gamePly_ = ply; }

	static constexpr int GetNlist() { return EvalList::m_ListSize; }

	int GetList0(const int index) const { return m_evalList_.m_list0[index]; }

	int GetList1(const int index) const { return m_evalList_.m_list1[index]; }

	int GetSquareHandToList(const Square sq) const { return m_evalList_.m_squareHandToList[sq]; }

	Square GetListToSquareHand(const int i) const { return m_evalList_.m_listToSquareHand[i]; }

	int* GetPlist0() { return &m_evalList_.m_list0[0]; }

	int* GetPlist1() { return &m_evalList_.m_list1[0]; }

	const int* GetCplist0() const { return &m_evalList_.m_list0[0]; }

	const int* GetCplist1() const { return &m_evalList_.m_list1[0]; }

	const ChangedLists& GetCl() const { return m_st_->m_cl; }

	const Searcher* GetCsearcher() const { return m_searcher_; }

	Searcher* GetSearcher() const { return m_searcher_; }
	void SetSearcher(Searcher* s) { m_searcher_ = s; }

#if !defined NDEBUG
	// for debug
	bool IsOK() const;
#endif

	static void InitZobrist();

	static Score GetPieceScore(const Piece pc) { return g_PieceScore[pc]; }
	// Piece を index としても、 PieceType を index としても、
	// 同じ値が取得出来るようにしているので、PieceType => Piece への変換は必要ない。
	static Score GetPieceScore(const PieceType pt) { return g_PieceScore[pt]; }
	static Score GetCapturePieceScore(const Piece pc) { return g_CapturePieceScore[pc]; }
	// Piece を index としても、 PieceType を index としても、
	// 同じ値が取得出来るようにしているので、PieceType => Piece への変換は必要ない。
	static Score GetCapturePieceScore(const PieceType pt) { return g_CapturePieceScore[pt]; }
	static Score GetPromotePieceScore(const PieceType pt) {
		assert(pt < Gold);
		return g_PromotePieceScore[pt];
	}

private:
	void Clear();
	void SetPiece(const Piece piece, const Square sq) {
		const Color c = UtilPiece::ToColor(piece);
		const PieceType pt = UtilPiece::ToPieceType(piece);

		m_piece_[sq] = piece;

		g_setMaskBb.SetBit(&m_byTypeBB_[pt], sq);
		g_setMaskBb.SetBit(&m_byColorBB_[c], sq);
		g_setMaskBb.SetBit(&m_byTypeBB_[Occupied], sq);
	}
	void SetHand(const HandPiece hp, const Color c, const int num) { m_hand_[c].OrEqual(num, hp); }
	void SetHand(const Piece piece, const int num) {
		const Color c = UtilPiece::ToColor(piece);
		const PieceType pt = UtilPiece::ToPieceType(piece);
		const HandPiece hp = UtilHandPiece::FromPieceType(pt);
		SetHand(hp, c, num);
	}

	// 手番側の玉へ check している駒を全て探して checkersBB_ にセットする。
	// 最後の手が何か覚えておけば、attackersTo() を使用しなくても良いはずで、処理が軽くなる。
	void FindCheckers() { m_st_->m_checkersBB = GetAttackersToExceptKing(UtilColor::OppositeColor(GetTurn()), GetKingSquare(GetTurn())); }

	Score ComputeMaterial() const;

	void XorBBs(const PieceType pt, const Square sq, const Color c);

	// turn() 側が
	// pin されて(して)いる駒の Bitboard を返す。
	// BetweenIsUs == true  : 間の駒が自駒。
	// BetweenIsUs == false : 間の駒が敵駒。
	template <bool FindPinned, bool BetweenIsUs>
	Bitboard GetHiddenCheckers() const {
		Bitboard result = Bitboard::AllZeroBB();
		const Color us = GetTurn();
		const Color them = UtilColor::OppositeColor(us);
		// pin する遠隔駒
		// まずは自駒か敵駒かで大雑把に判別
		Bitboard pinners = this->GetBbOf(FindPinned ? them : us);

		const Square ksq = GetKingSquare(FindPinned ? us : them);

		// 障害物が無ければ玉に到達出来る駒のBitboardだけ残す。
		pinners &=	(
						this->GetBbOf(Lance) &
						g_lanceAttackBb.GetControllBbToEdge((FindPinned ? us : them), ksq)
					) |
					(
						this->GetBbOf(Rook, Dragon) &
						g_rookAttackBb.RookAttackToEdge(ksq)
					) |
					(
						this->GetBbOf(Bishop, Horse) &
						g_bishopAttackBb.GetControllBbToEdge(ksq)
					);

		while (pinners.IsNot0()) {
			const Square sq = pinners.FirstOneFromI9();
			// pin する遠隔駒と玉の間にある駒の位置の Bitboard
			const Bitboard between = g_betweenBb.BetweenBB(sq, ksq) & GetOccupiedBB();

			// pin する遠隔駒と玉の間にある駒が1つで、かつ、引数の色のとき、その駒は(を) pin されて(して)いる。
			if (between.IsNot0()
				&& between.IsOneBit<false>()
				&& between.AndIsNot0( this->GetBbOf(BetweenIsUs ? us : them)))
			{
				result |= between;
			}
		}

		return result;
	}

#if !defined NDEBUG
	int GetDebugSetEvalList() const;
#endif
	void SetEvalList() { m_evalList_.Set(*this); }

	Key GetComputeBoardKey() const;
	Key GetComputeHandKey() const;
	Key GetComputeKey() const { return GetComputeBoardKey() + GetComputeHandKey(); }

	void PrintHand(const Color c) const;

	static Key GetZobrist(const PieceType pt, const Square sq, const Color c) { return m_zobrist_[pt][sq][c]; }
	static Key GetZobTurn() { return m_zobTurn_; }
	static Key GetZobHand(const HandPiece hp, const Color c) { return m_zobHand_[hp][c]; }


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
