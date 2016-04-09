#pragma once

#include <stack>
#include "../n116_hand____/n116_500_hand.hpp"
#include "../n160_board___/n160_400_printBb.hpp"
#include "../n160_board___/n160_102_FileMaskBb.hpp"
#include "../n160_board___/n160_106_inFrontMaskBb.hpp"
#include "../n119_score___/n119_200_pieceScore.hpp"
#include "../n220_position/n220_350_stateInfo.hpp"
#include "../n220_position/n220_400_evalList.hpp"


using StateStackPtr = std::unique_ptr<std::stack<StateInfo> >;


class Move;
struct Thread;
struct Searcher;
//class EvalList;


class Position {
public:
	Position();

	explicit Position(Searcher* s);

	Position(const Position& pos);

	Position(const Position& pos, Thread* th);

	Position(const std::string& sfen, Thread* th, Searcher* s);

	Position& operator = (const Position& pos);

	void Set(const std::string& sfen, Thread* th);

	Bitboard GetBbOf(const PieceType pt) const;
	Bitboard GetBbOf(const Color c) const;
	Bitboard GetBbOf(const PieceType pt, const Color c) const;
	Bitboard GetBbOf(const PieceType pt1, const PieceType pt2) const;
	Bitboard GetBbOf(const PieceType pt1, const PieceType pt2, const Color c) const;
	Bitboard GetBbOf(const PieceType pt1, const PieceType pt2, const PieceType pt3) const;
	Bitboard GetBbOf(const PieceType pt1, const PieceType pt2, const PieceType pt3, const PieceType pt4) const;
	Bitboard GetBbOf(const PieceType pt1, const PieceType pt2, const PieceType pt3,
		const PieceType pt4, const PieceType pt5) const;

	Bitboard GetOccupiedBB() const;
	// emptyBB() よりもわずかに速いはず。
	// emptyBB() とは異なり、全く使用しない位置(0 から数えて、right の 63bit目、left の 18 ~ 63bit目)
	// の bit が 1 になっても構わないとき、こちらを使う。
	// todo: SSEにビット反転が無いので実はそんなに速くないはず。不要。
	Bitboard GetNOccupiedBB() const;
	Bitboard GetEmptyBB() const;
	// 金、成り金 の Bitboard
	Bitboard GetGoldsBB() const;
	Bitboard GetGoldsBB(const Color c) const;

	Piece GetPiece(const Square sq) const;

	// hand
	Hand GetHand(const Color c) const;

	// turn() 側が pin されている Bitboard を返す。
	// checkersBB が更新されている必要がある。
	Bitboard GetPinnedBB() const;
	// 間の駒を動かすことで、turn() 側が空き王手が出来る駒のBitboardを返す。
	// checkersBB が更新されている必要はない。
	// BetweenIsUs == true  : 間の駒が自駒。
	// BetweenIsUs == false : 間の駒が敵駒。
	template <bool BetweenIsUs = true>
	Bitboard DiscoveredCheckBB() const { return GetHiddenCheckers<false, BetweenIsUs>(); }

	// toFile と同じ筋に us の歩がないなら true
	bool NoPawns(const Color us, const File toFile) const;
	bool IsPawnDropCheckMate(const Color us, const Square sq) const;

	// Pinされているfromの駒がtoに移動出来なければtrueを返す。
	template <bool IsKnight = false>
	bool IsPinnedIllegal(const Square from, const Square to, const Square ksq, const Bitboard& pinned) const {
		// 桂馬ならどこに動いても駄目。
		return g_setMaskBb.IsSet(&pinned,from) && (IsKnight || !g_squareRelation.IsAligned<true>(from, to, ksq));
	}
	// 空き王手かどうか。
	template <bool IsKnight = false>
	bool IsDiscoveredCheck(const Square from, const Square to, const Square ksq, const Bitboard& dcBB) const {
		// 桂馬ならどこに動いても空き王手になる。
		return g_setMaskBb.IsSet(&dcBB,from) && (IsKnight || !g_squareRelation.IsAligned<true>(from, to, ksq));
	}

	Bitboard GetCheckersBB() const;
	Bitboard GetPrevCheckersBB() const;
	// 王手が掛かっているか。
	bool InCheck() const;

	Score GetMaterial() const;
	Score GetMaterialDiff() const;

	FORCE_INLINE Square GetKingSquare(const Color c) const {
		assert(m_kingSquare_[c] == this->GetBbOf(King, c).GetFirstOneFromI9());
		return m_kingSquare_[c];
	}

	bool IsMoveGivesCheck(const Move m) const;
	bool IsMoveGivesCheck(const Move move, const CheckInfo& ci) const;

	// attacks
	Bitboard GetAttackersTo(const Square sq, const Bitboard& occupied) const;
	Bitboard GetAttackersTo(const Color c, const Square sq) const;
	Bitboard GetAttackersTo(const Color c, const Square sq, const Bitboard& occupied) const;
	Bitboard GetAttackersToExceptKing(const Color c, const Square sq) const;
	// todo: 利きをデータとして持ったとき、attackersToIsNot0() を高速化すること。
	bool IsAttackersToIsNot0(const Color c, const Square sq) const;
	bool IsAttackersToIsNot0(const Color c, const Square sq, const Bitboard& occupied) const;
	// 移動王手が味方の利きに支えられているか。false なら相手玉で取れば詰まない。
	bool IsUnDropCheckIsSupported(const Color c, const Square sq) const;
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
		return g_goldAttackBb.GetControllBb(c, sq);
	}
	template <PieceType PT>
	Bitboard GetAttacksFrom(const Square sq) const {
		static_assert(PT == Bishop || PT == Rook || PT == King || PT == Horse || PT == Dragon, "");
		// Color は何でも良い。
		return GetAttacksFrom<PT>(ColorNum, sq);
	}
	Bitboard GetAttacksFrom(const PieceType pt, const Color c, const Square sq) const;
	static Bitboard GetAttacksFrom(const PieceType pt, const Color c, const Square sq, const Bitboard& occupied);

	// 次の手番
	Color GetTurn() const;

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

	Ply GetGamePly() const;

	Key GetBoardKey() const;
	Key GetHandKey() const;
	Key GetKey() const;
	Key GetExclusionKey() const;
	Key GetKeyExcludeTurn() const;
	void Print() const;

	u64 GetNodesSearched() const;
	void SetNodesSearched(const u64 n);
	RepetitionType IsDraw(const int checkMaxPly = std::numeric_limits<int>::max()) const;

	Thread* GetThisThread() const;

	void SetStartPosPly(const Ply ply);

	static constexpr int GetNlist() {
		return EvalList::m_ListSize;
	}


	int GetList0(const int index) const;

	int GetList1(const int index) const;

	int GetSquareHandToList(const Square sq) const;

	Square GetListToSquareHand(const int i) const;

	int* GetPlist0();

	int* GetPlist1();

	const int* GetCplist0() const;

	const int* GetCplist1() const;

	const ChangedLists& GetCl() const;

	const Searcher* GetCsearcher() const;

	Searcher* GetSearcher() const;

	void SetSearcher(Searcher* s);

#if !defined NDEBUG
	// for debug
	bool IsOK() const;
#endif

	static void InitZobrist();

	static Score GetPieceScore(const Piece pc);
	// Piece を index としても、 PieceType を index としても、
	// 同じ値が取得出来るようにしているので、PieceType => Piece への変換は必要ない。
	static Score GetPieceScore(const PieceType pt);
	static Score GetCapturePieceScore(const Piece pc);
	// Piece を index としても、 PieceType を index としても、
	// 同じ値が取得出来るようにしているので、PieceType => Piece への変換は必要ない。
	static Score GetCapturePieceScore(const PieceType pt);
	static Score GetPromotePieceScore(const PieceType pt);

private:
	void Clear();
	void SetPiece(const Piece piece, const Square sq);
	void SetHand(const HandPiece hp, const Color c, const int num);
	void SetHand(const Piece piece, const int num);

	// 手番側の玉へ check している駒を全て探して checkersBB_ にセットする。
	// 最後の手が何か覚えておけば、attackersTo() を使用しなくても良いはずで、処理が軽くなる。
	void FindCheckers();

	Score ComputeMaterial() const;

	void XorBBs(const PieceType pt, const Square sq, const Color c);

	// turn() 側が
	// pin されて(して)いる駒の Bitboard を返す。
	// BetweenIsUs == true  : 間の駒が自駒。
	// BetweenIsUs == false : 間の駒が敵駒。
	template <bool FindPinned, bool BetweenIsUs>
	Bitboard GetHiddenCheckers() const {
		Bitboard result = Bitboard::CreateAllZeroBB();
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
						g_rookAttackBb.GetControllBbToEdge(ksq)
					) |
					(
						this->GetBbOf(Bishop, Horse) &
						g_bishopAttackBb.GetControllBbToEdge(ksq)
					);

		while (pinners.Exists1Bit()) {
			const Square sq = pinners.PopFirstOneFromI9();
			// pin する遠隔駒と玉の間にある駒の位置の Bitboard
			const Bitboard between = g_betweenBb.GetBetweenBB(sq, ksq) & GetOccupiedBB();

			// pin する遠隔駒と玉の間にある駒が1つで、かつ、引数の色のとき、その駒は(を) pin されて(して)いる。
			if (between.Exists1Bit()
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
	void SetEvalList();

	Key GetComputeBoardKey() const;
	Key GetComputeHandKey() const;
	Key GetComputeKey() const;

	void PrintHand(const Color c) const;

	static Key GetZobrist(const PieceType pt, const Square sq, const Color c);
	static Key GetZobTurn();
	static Key GetZobHand(const HandPiece hp, const Color c);


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
