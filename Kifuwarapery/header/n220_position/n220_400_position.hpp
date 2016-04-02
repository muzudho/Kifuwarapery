#pragma once

#include "../../header/n220_position/n220_350_stateInfo.hpp"
#include "../../header/n240_position/n240_200_evalList.hpp"

using StateStackPtr = std::unique_ptr<std::stack<StateInfo> >;

class Move;
struct Thread;
struct Searcher;

class Position {
public:
	Position() {}
	explicit Position(Searcher* s) : searcher_(s) {}
	Position(const Position& pos) { *this = pos; }
	Position(const Position& pos, Thread* th) {
		*this = pos;
		thisThread_ = th;
	}
	Position(const std::string& sfen, Thread* th, Searcher* s) {
		set(sfen, th);
		setSearcher(s);
	}

	Position& operator = (const Position& pos);
	void set(const std::string& sfen, Thread* th);

	Bitboard bbOf(const PieceType pt) const { return byTypeBB_[pt]; }
	Bitboard bbOf(const Color c) const { return byColorBB_[c]; }
	Bitboard bbOf(const PieceType pt, const Color c) const { return bbOf(pt) & bbOf(c); }
	Bitboard bbOf(const PieceType pt1, const PieceType pt2) const { return bbOf(pt1) | bbOf(pt2); }
	Bitboard bbOf(const PieceType pt1, const PieceType pt2, const Color c) const { return bbOf(pt1, pt2) & bbOf(c); }
	Bitboard bbOf(const PieceType pt1, const PieceType pt2, const PieceType pt3) const { return bbOf(pt1, pt2) | bbOf(pt3); }
	Bitboard bbOf(const PieceType pt1, const PieceType pt2, const PieceType pt3, const PieceType pt4) const {
		return bbOf(pt1, pt2, pt3) | bbOf(pt4);
	}
	Bitboard bbOf(const PieceType pt1, const PieceType pt2, const PieceType pt3,
		const PieceType pt4, const PieceType pt5) const
	{
		return bbOf(pt1, pt2, pt3, pt4) | bbOf(pt5);
	}
	Bitboard occupiedBB() const { return bbOf(Occupied); }
	// emptyBB() �����킸���ɑ����͂��B
	// emptyBB() �Ƃ͈قȂ�A�S���g�p���Ȃ��ʒu(0 ���琔���āAright �� 63bit�ځAleft �� 18 ~ 63bit��)
	// �� bit �� 1 �ɂȂ��Ă��\��Ȃ��Ƃ��A��������g���B
	// todo: SSE�Ƀr�b�g���]�������̂Ŏ��͂���Ȃɑ����Ȃ��͂��B�s�v�B
	Bitboard nOccupiedBB() const { return ~occupiedBB(); }
	Bitboard emptyBB() const { return occupiedBB() ^ Bitboard::allOneBB(); }
	// ���A����� �� Bitboard
	Bitboard goldsBB() const { return goldsBB_; }
	Bitboard goldsBB(const Color c) const { return goldsBB() & bbOf(c); }

	Piece piece(const Square sq) const { return piece_[sq]; }

	// hand
	Hand hand(const Color c) const { return hand_[c]; }

	// turn() ���� pin ����Ă��� Bitboard ��Ԃ��B
	// checkersBB ���X�V����Ă���K�v������B
	Bitboard pinnedBB() const { return hiddenCheckers<true, true>(); }
	// �Ԃ̋�𓮂������ƂŁAturn() �����󂫉��肪�o������Bitboard��Ԃ��B
	// checkersBB ���X�V����Ă���K�v�͂Ȃ��B
	// BetweenIsUs == true  : �Ԃ̋����B
	// BetweenIsUs == false : �Ԃ̋�G��B
	template <bool BetweenIsUs = true> Bitboard discoveredCheckBB() const { return hiddenCheckers<false, BetweenIsUs>(); }

	// toFile �Ɠ����؂� us �̕����Ȃ��Ȃ� true
	bool noPawns(const Color us, const File toFile) const { return !bbOf(Pawn, us).andIsNot0(fileMask(toFile)); }
	bool isPawnDropCheckMate(const Color us, const Square sq) const;
	// Pin����Ă���from�̋to�Ɉړ��o���Ȃ����true��Ԃ��B
	template <bool IsKnight = false>
	bool isPinnedIllegal(const Square from, const Square to, const Square ksq, const Bitboard& pinned) const {
		// �j�n�Ȃ�ǂ��ɓ����Ă��ʖځB
		return pinned.isSet(from) && (IsKnight || !UtilSquare::IsAligned<true>(from, to, ksq));
	}
	// �󂫉��肩�ǂ����B
	template <bool IsKnight = false>
	bool isDiscoveredCheck(const Square from, const Square to, const Square ksq, const Bitboard& dcBB) const {
		// �j�n�Ȃ�ǂ��ɓ����Ă��󂫉���ɂȂ�B
		return dcBB.isSet(from) && (IsKnight || !UtilSquare::IsAligned<true>(from, to, ksq));
	}

	Bitboard checkersBB() const { return st_->checkersBB; }
	Bitboard prevCheckersBB() const { return st_->previous->checkersBB; }
	// ���肪�|�����Ă��邩�B
	bool inCheck() const { return checkersBB().isNot0(); }

	Score material() const { return st_->material; }
	Score materialDiff() const { return st_->material - st_->previous->material; }

	FORCE_INLINE Square kingSquare(const Color c) const {
		assert(kingSquare_[c] == bbOf(King, c).constFirstOneFromI9());
		return kingSquare_[c];
	}

	bool moveGivesCheck(const Move m) const;
	bool moveGivesCheck(const Move move, const CheckInfo& ci) const;

	// attacks
	Bitboard attackersTo(const Square sq, const Bitboard& occupied) const;
	Bitboard attackersTo(const Color c, const Square sq) const { return attackersTo(c, sq, occupiedBB()); }
	Bitboard attackersTo(const Color c, const Square sq, const Bitboard& occupied) const;
	Bitboard attackersToExceptKing(const Color c, const Square sq) const;
	// todo: �������f�[�^�Ƃ��Ď������Ƃ��AattackersToIsNot0() �����������邱�ƁB
	bool attackersToIsNot0(const Color c, const Square sq) const { return attackersTo(c, sq).isNot0(); }
	bool attackersToIsNot0(const Color c, const Square sq, const Bitboard& occupied) const {
		return attackersTo(c, sq, occupied).isNot0();
	}
	// �ړ����肪�����̗����Ɏx�����Ă��邩�Bfalse �Ȃ瑊��ʂŎ��΋l�܂Ȃ��B
	bool unDropCheckIsSupported(const Color c, const Square sq) const { return attackersTo(c, sq).isNot0(); }
	// �����̐���

	// �C�ӂ� occupied �ɑ΂��闘���𐶐�����B
	template <PieceType PT> static Bitboard attacksFrom(const Color c, const Square sq, const Bitboard& occupied);
	// �C�ӂ� occupied �ɑ΂��闘���𐶐�����B
	template <PieceType PT> Bitboard attacksFrom(const Square sq, const Bitboard& occupied) const {
		static_assert(PT == Bishop || PT == Rook || PT == Horse || PT == Dragon, "");
		// Color �͉��ł��ǂ��B
		return attacksFrom<PT>(ColorNum, sq, occupied);
	}

	template <PieceType PT> Bitboard attacksFrom(const Color c, const Square sq) const {
		static_assert(PT == Gold, ""); // Gold �ȊO�� template ���ꉻ����B
		return goldAttack(c, sq);
	}
	template <PieceType PT> Bitboard attacksFrom(const Square sq) const {
		static_assert(PT == Bishop || PT == Rook || PT == King || PT == Horse || PT == Dragon, "");
		// Color �͉��ł��ǂ��B
		return attacksFrom<PT>(ColorNum, sq);
	}
	Bitboard attacksFrom(const PieceType pt, const Color c, const Square sq) const { return attacksFrom(pt, c, sq, occupiedBB()); }
	static Bitboard attacksFrom(const PieceType pt, const Color c, const Square sq, const Bitboard& occupied);

	// ���̎��
	Color turn() const { return turn_; }

	// pseudoLegal �Ƃ�
	// �E�ʂ������̗���������ꏊ�Ɉړ�����
	// �Epin �̋���ړ�������
	// �E�A������̐����̎���w��
	// �����̔�������܂߂���̎��ƒ�`����B
	// ����āA�ł����l�߂����̎�� pseudoLegal �ł͖����B
	template <bool MUSTNOTDROP, bool FROMMUSTNOTBEKING>
	bool pseudoLegalMoveIsLegal(const Move move, const Bitboard& pinned) const;
	bool pseudoLegalMoveIsEvasion(const Move move, const Bitboard& pinned) const;
	// checkPawnDrop : ����Ƒł����l�߂����ׂ�Ȃ� true
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

	Ply gamePly() const { return gamePly_; }

	Key getBoardKey() const { return st_->boardKey; }
	Key getHandKey() const { return st_->handKey; }
	Key getKey() const { return st_->key(); }
	Key getExclusionKey() const { return st_->key() ^ zobExclusion_; }
	Key getKeyExcludeTurn() const {
		static_assert(zobTurn_ == 1, "");
		return getKey() >> 1;
	}
	void print() const;

	u64 nodesSearched() const { return nodes_; }
	void setNodesSearched(const u64 n) { nodes_ = n; }
	RepetitionType isDraw(const int checkMaxPly = std::numeric_limits<int>::max()) const;

	Thread* thisThread() const { return thisThread_; }

	void setStartPosPly(const Ply ply) { gamePly_ = ply; }

	static constexpr int nlist() { return EvalList::ListSize; }
	int list0(const int index) const { return evalList_.list0[index]; }
	int list1(const int index) const { return evalList_.list1[index]; }
	int squareHandToList(const Square sq) const { return evalList_.squareHandToList[sq]; }
	Square listToSquareHand(const int i) const { return evalList_.listToSquareHand[i]; }
	int* plist0() { return &evalList_.list0[0]; }
	int* plist1() { return &evalList_.list1[0]; }
	const int* cplist0() const { return &evalList_.list0[0]; }
	const int* cplist1() const { return &evalList_.list1[0]; }
	const ChangedLists& cl() const { return st_->cl; }

	const Searcher* csearcher() const { return searcher_; }
	Searcher* searcher() const { return searcher_; }
	void setSearcher(Searcher* s) { searcher_ = s; }

#if !defined NDEBUG
	// for debug
	bool isOK() const;
#endif

	static void initZobrist();

	static Score pieceScore(const Piece pc) { return PieceScore[pc]; }
	// Piece �� index �Ƃ��Ă��A PieceType �� index �Ƃ��Ă��A
	// �����l���擾�o����悤�ɂ��Ă���̂ŁAPieceType => Piece �ւ̕ϊ��͕K�v�Ȃ��B
	static Score pieceScore(const PieceType pt) { return PieceScore[pt]; }
	static Score capturePieceScore(const Piece pc) { return CapturePieceScore[pc]; }
	// Piece �� index �Ƃ��Ă��A PieceType �� index �Ƃ��Ă��A
	// �����l���擾�o����悤�ɂ��Ă���̂ŁAPieceType => Piece �ւ̕ϊ��͕K�v�Ȃ��B
	static Score capturePieceScore(const PieceType pt) { return CapturePieceScore[pt]; }
	static Score promotePieceScore(const PieceType pt) {
		assert(pt < Gold);
		return PromotePieceScore[pt];
	}

private:
	void clear();
	void setPiece(const Piece piece, const Square sq) {
		const Color c = pieceToColor(piece);
		const PieceType pt = pieceToPieceType(piece);

		piece_[sq] = piece;

		byTypeBB_[pt].setBit(sq);
		byColorBB_[c].setBit(sq);
		byTypeBB_[Occupied].setBit(sq);
	}
	void setHand(const HandPiece hp, const Color c, const int num) { hand_[c].orEqual(num, hp); }
	void setHand(const Piece piece, const int num) {
		const Color c = pieceToColor(piece);
		const PieceType pt = pieceToPieceType(piece);
		const HandPiece hp = pieceTypeToHandPiece(pt);
		setHand(hp, c, num);
	}

	// ��ԑ��̋ʂ� check ���Ă�����S�ĒT���� checkersBB_ �ɃZ�b�g����B
	// �Ō�̎肪�����o���Ă����΁AattackersTo() ���g�p���Ȃ��Ă��ǂ��͂��ŁA�������y���Ȃ�B
	void findCheckers() { st_->checkersBB = attackersToExceptKing(UtilColor::OppositeColor(turn()), kingSquare(turn())); }

	Score computeMaterial() const;

	void xorBBs(const PieceType pt, const Square sq, const Color c);
	// turn() ����
	// pin �����(����)������ Bitboard ��Ԃ��B
	// BetweenIsUs == true  : �Ԃ̋����B
	// BetweenIsUs == false : �Ԃ̋�G��B
	template <bool FindPinned, bool BetweenIsUs> Bitboard hiddenCheckers() const {
		Bitboard result = Bitboard::allZeroBB();
		const Color us = turn();
		const Color them = UtilColor::OppositeColor(us);
		// pin ���鉓�u��
		// �܂��͎���G��ő�G�c�ɔ���
		Bitboard pinners = bbOf(FindPinned ? them : us);

		const Square ksq = kingSquare(FindPinned ? us : them);

		// ��Q����������΋ʂɓ��B�o������Bitboard�����c���B
		pinners &= (bbOf(Lance) & lanceAttackToEdge((FindPinned ? us : them), ksq)) |
			(bbOf(Rook, Dragon) & rookAttackToEdge(ksq)) | (bbOf(Bishop, Horse) & bishopAttackToEdge(ksq));

		while (pinners.isNot0()) {
			const Square sq = pinners.firstOneFromI9();
			// pin ���鉓�u��Ƌʂ̊Ԃɂ����̈ʒu�� Bitboard
			const Bitboard between = betweenBB(sq, ksq) & occupiedBB();

			// pin ���鉓�u��Ƌʂ̊Ԃɂ���1�ŁA���A�����̐F�̂Ƃ��A���̋��(��) pin �����(����)����B
			if (between.isNot0()
				&& between.isOneBit<false>()
				&& between.andIsNot0(bbOf(BetweenIsUs ? us : them)))
			{
				result |= between;
			}
		}

		return result;
	}

#if !defined NDEBUG
	int debugSetEvalList() const;
#endif
	void setEvalList() { evalList_.set(*this); }

	Key computeBoardKey() const;
	Key computeHandKey() const;
	Key computeKey() const { return computeBoardKey() + computeHandKey(); }

	void printHand(const Color c) const;

	static Key zobrist(const PieceType pt, const Square sq, const Color c) { return zobrist_[pt][sq][c]; }
	static Key zobTurn() { return zobTurn_; }
	static Key zobHand(const HandPiece hp, const Color c) { return zobHand_[hp][c]; }

	// byTypeBB �͓G�A�����̋����ʂ��Ȃ��B
	// byColorBB �͋�̎�ނ���ʂ��Ȃ��B
	Bitboard byTypeBB_[PieceTypeNum];
	Bitboard byColorBB_[ColorNum];
	Bitboard goldsBB_;

	// �e�}�X�̏��
	Piece piece_[SquareNum];
	Square kingSquare_[ColorNum];

	// ���
	Hand hand_[ColorNum];
	Color turn_;

	EvalList evalList_;

	StateInfo startState_;
	StateInfo* st_;
	// ���ԊǗ��Ɏg�p����B
	Ply gamePly_;
	Thread* thisThread_;
	u64 nodes_;

	Searcher* searcher_;

	static Key zobrist_[PieceTypeNum][SquareNum][ColorNum];
	static const Key zobTurn_ = 1;
	static Key zobHand_[HandPieceNum][ColorNum];
	static Key zobExclusion_; // todo: ���ꂪ�K�v���A�v����
};
