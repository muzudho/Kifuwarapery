#include <sstream>      // std::istringstream
#include "../../header/n160_board___/n160_200_utilBitboard.hpp"
#include "../../header/n220_position/n220_500_charToPieceUSI.hpp"
#include "../../header/n223_move____/n223_105_utilMove.hpp"
#include "../../header/n240_tt______/n240_300_tt.hpp"
#include "../../header/n276_genMove_/n276_250_makePromoteMove.hpp"
#include "../../header/n280_move____/n280_200_mt64bit.hpp"
#include "../../header/n320_searcher/n320_150_search.hpp"


Key Position::m_zobrist_[PieceTypeNum][SquareNum][ColorNum];
Key Position::m_zobHand_[HandPieceNum][ColorNum];
Key Position::m_zobExclusion_;

const CharToPieceUSI g_charToPieceUSI;

namespace {
	const char* PieceToCharCSATable[PieceNone] = {
		" * ", "+FU", "+KY", "+KE", "+GI", "+KA", "+HI", "+KI", "+OU", "+TO", "+NY", "+NK", "+NG", "+UM", "+RY", "", "",
		"-FU", "-KY", "-KE", "-GI", "-KA", "-HI", "-KI", "-OU", "-TO", "-NY", "-NK", "-NG", "-UM", "-RY"
	};
	inline const char* pieceToCharCSA(const Piece pc) {
		return PieceToCharCSATable[pc];
	}
}

CheckInfo::CheckInfo(const Position& pos) {
	const Color them = UtilColor::OppositeColor(pos.GetTurn());
	const Square ksq = pos.GetKingSquare(them);

	m_pinned = pos.GetPinnedBB();
	m_dcBB = pos.DiscoveredCheckBB();

	m_checkBB[Pawn     ] = pos.GetAttacksFrom<Pawn  >(them, ksq);
	m_checkBB[Lance    ] = pos.GetAttacksFrom<Lance >(them, ksq);
	m_checkBB[Knight   ] = pos.GetAttacksFrom<Knight>(them, ksq);
	m_checkBB[Silver   ] = pos.GetAttacksFrom<Silver>(them, ksq);
	m_checkBB[Bishop   ] = pos.GetAttacksFrom<Bishop>(ksq);
	m_checkBB[Rook     ] = pos.GetAttacksFrom<Rook  >(ksq);
	m_checkBB[Gold     ] = pos.GetAttacksFrom<Gold  >(them, ksq);
	m_checkBB[King     ] = Bitboard::AllZeroBB();
	// todo: ここで AVX2 使えそう。
	//       checkBB のreadアクセスは switch (pt) で場合分けして、余計なコピー減らした方が良いかも。
	m_checkBB[ProPawn  ] = m_checkBB[Gold];
	m_checkBB[ProLance ] = m_checkBB[Gold];
	m_checkBB[ProKnight] = m_checkBB[Gold];
	m_checkBB[ProSilver] = m_checkBB[Gold];
	m_checkBB[Horse    ] = m_checkBB[Bishop] | pos.GetAttacksFrom<King>(ksq);
	m_checkBB[Dragon   ] = m_checkBB[Rook  ] | pos.GetAttacksFrom<King>(ksq);
}

Bitboard Position::GetAttacksFrom(const PieceType pt, const Color c, const Square sq, const Bitboard& occupied) {
	switch (pt) {
	case Occupied:  return Bitboard::AllZeroBB();
	case Pawn:      return UtilBitboard::PawnAttack(c, sq);
	case Lance:     return UtilBitboard::LanceAttack(&occupied, c, sq);
	case Knight:    return UtilBitboard::KnightAttack(c, sq);
	case Silver:    return UtilBitboard::SilverAttack(c, sq);
	case Bishop:    return UtilBitboard::BishopAttack(&occupied, sq);
	case Rook:      return UtilBitboard::RookAttack(&occupied, sq);
	case Gold:
	case ProPawn:
	case ProLance:
	case ProKnight:
	case ProSilver: return UtilBitboard::GoldAttack(c, sq);
	case King:      return g_kingAttackBb.KingAttack(sq);
	case Horse:     return UtilBitboard::HorseAttack(&occupied,sq);
	case Dragon:    return UtilBitboard::DragonAttack(&occupied,sq);
	default:        UNREACHABLE;
	}
	UNREACHABLE;
	return Bitboard::AllOneBB();
}

// 実際に指し手が合法手かどうか判定
// 連続王手の千日手は排除しない。
// 確実に駒打ちではないときは、MUSTNOTDROP == true とする。
// 確実に玉の移動で無いときは、FROMMUSTNOTKING == true とする。英語として正しい？
// 遠隔駒で王手されているとき、その駒の利きがある場所に逃げる手を検出出来ない場合があるので、
// そのような手を指し手生成してはいけない。
template <bool MUSTNOTDROP, bool FROMMUSTNOTKING>
bool Position::IsPseudoLegalMoveIsLegal(const Move move, const Bitboard& pinned) const {
	// 駒打ちは、打ち歩詰めや二歩は指し手生成時や、killerをMovePicker::nextMove() 内で排除しているので、常に合法手
	// (連続王手の千日手は省いていないけれど。)
	if (!MUSTNOTDROP && move.IsDrop()) {
		return true;
	}
	assert(!move.IsDrop());

	const Color us = GetTurn();
	const Square from = move.From();

	if (!FROMMUSTNOTKING && UtilPiece::ToPieceType(GetPiece(from)) == King) {
		const Color them = UtilColor::OppositeColor(us);
		// 玉の移動先に相手の駒の利きがあれば、合法手でないので、false
		return !IsAttackersToIsNot0(them, move.To());
	}
	// 玉以外の駒の移動
	return !IsPinnedIllegal(from, move.To(), GetKingSquare(us), pinned);
}

template bool Position::IsPseudoLegalMoveIsLegal<false, false>(const Move move, const Bitboard& pinned) const;
template bool Position::IsPseudoLegalMoveIsLegal<false, true >(const Move move, const Bitboard& pinned) const;
template bool Position::IsPseudoLegalMoveIsLegal<true,  false>(const Move move, const Bitboard& pinned) const;

bool Position::IsPseudoLegalMoveIsEvasion(const Move move, const Bitboard& pinned) const {
	assert(IsOK());

	// 玉の移動
	if (move.GetPieceTypeFrom() == King) {
		// 遠隔駒で王手されたとき、王手している遠隔駒の利きには移動しないように指し手を生成している。
		// その為、移動先に他の駒の利きが無いか調べるだけで良い。
		const bool canMove = !IsAttackersToIsNot0(UtilColor::OppositeColor(GetTurn()), move.To());
		assert(canMove == (IsPseudoLegalMoveIsLegal<false, false>(move, pinned)));
		return canMove;
	}

	// 玉の移動以外
	Bitboard target = GetCheckersBB();
	const Square checkSq = target.FirstOneFromI9();

	if (target.IsNot0()) {
		// 両王手のとき、玉の移動以外の手は指せない。
		return false;
	}

	const Color us = GetTurn();
	const Square to = move.To();
	// 移動、又は打った駒が、王手をさえぎるか、王手している駒を取る必要がある。
	target = UtilBitboard::BetweenBB(checkSq, GetKingSquare(us)) | GetCheckersBB();
	return UtilBitboard::IsSet(&target,to) && IsPseudoLegalMoveIsLegal<false, true>(move, pinned);
}

// checkPawnDrop : 二歩と打ち歩詰めも調べるなら true
//                 これが true のとき、駒打ちの場合のみ Legal であることが確定する。
bool Position::MoveIsPseudoLegal(const Move move, const bool checkPawnDrop) const {
	const Color us = GetTurn();
	const Color them = UtilColor::OppositeColor(us);
	const Square to = move.To();

	if (move.IsDrop()) {
		const PieceType ptFrom = move.GetPieceTypeDropped();
		if (!GetHand(us).Exists(UtilHandPiece::FromPieceType(ptFrom)) || GetPiece(to) != Empty) {
			return false;
		}

		if (InCheck()) {
			// 王手されているので、合駒でなければならない。
			Bitboard target = GetCheckersBB();
			const Square checksq = target.FirstOneFromI9();

			if (target.IsNot0()) {
				// 両王手は合駒出来無い。
				return false;
			}

			target = UtilBitboard::BetweenBB(checksq, GetKingSquare(us));
			if (!UtilBitboard::IsSet(&target, to)) {
				// 玉と、王手した駒との間に駒を打っていない。
				return false;
			}
		}

		if (ptFrom == Pawn && checkPawnDrop) {
			if ((this->GetBbOf(Pawn, us) & BitboardMask::GetFileMask(UtilSquare::ToFile(to))).IsNot0()) {
				// 二歩
				return false;
			}
			const SquareDelta TDeltaN = (us == Black ? DeltaN : DeltaS);
			if (to + TDeltaN == GetKingSquare(them) && IsPawnDropCheckMate(us, to)) {
				// 王手かつ打ち歩詰め
				return false;
			}
		}
	}
	else {
		const Square from = move.From();
		const PieceType ptFrom = move.GetPieceTypeFrom();
		if (GetPiece(from) != UtilPiece::FromColorAndPieceType(us, ptFrom) || UtilBitboard::IsSet(&this->GetBbOf(us),to)) {
			return false;
		}

		if (!UtilBitboard::IsSet(&GetAttacksFrom(ptFrom, us, from),to)) {
			return false;
		}

		if (InCheck()) {
			if (ptFrom == King) {
				Bitboard occ = GetOccupiedBB();
				UtilBitboard::ClearBit(&occ, from);
				if (IsAttackersToIsNot0(them, to, occ)) {
					// 王手から逃げていない。
					return false;
				}
			}
			else {
				// 玉以外の駒を移動させたとき。
				Bitboard target = GetCheckersBB();
				const Square checksq = target.FirstOneFromI9();

				if (target.IsNot0()) {
					// 両王手なので、玉が逃げない手は駄目
					return false;
				}

				target = UtilBitboard::BetweenBB(checksq, GetKingSquare(us)) | GetCheckersBB();
				if (!UtilBitboard::IsSet(&target,to)) {
					// 玉と、王手した駒との間に移動するか、王手した駒を取る以外は駄目。
					return false;
				}
			}
		}
	}

	return true;
}

#if !defined NDEBUG
// 過去(又は現在)に生成した指し手が現在の局面でも有効か判定。
// あまり速度が要求される場面で使ってはいけない。
bool Position::MoveIsLegal(const Move GetMove) const {
	return MoveList<LegalAll>(*this).Contains(GetMove);
}
#endif

// 局面の更新
void Position::DoMove(const Move move, StateInfo& newSt) {
	const CheckInfo ci(*this);
	DoMove(move, newSt, ci, IsMoveGivesCheck(move, ci));
}

// 局面の更新
void Position::DoMove(const Move move, StateInfo& newSt, const CheckInfo& ci, const bool moveIsCheck) {
	assert(IsOK());
	assert(!move.IsNone());
	assert(&newSt != m_st_);

	Key boardKey = GetBoardKey();
	Key handKey = GetHandKey();
	boardKey ^= GetZobTurn();

	memcpy(&newSt, m_st_, sizeof(StateInfoMin));
	newSt.m_previous = m_st_;
	m_st_ = &newSt;

	m_st_->m_cl.m_size = 1;

	const Color us = GetTurn();
	const Square to = move.To();
	const PieceType ptCaptured = move.GetCap();
	PieceType ptTo;
	if (move.IsDrop()) {
		ptTo = move.GetPieceTypeDropped();
		const HandPiece hpTo = UtilHandPiece::FromPieceType(ptTo);

		handKey -= GetZobHand(hpTo, us);
		boardKey += GetZobrist(ptTo, to, us);

		prefetch(GetCsearcher()->m_tt.FirstEntry(boardKey + handKey));

		const int handnum = GetHand(us).NumOf(hpTo);
		const int listIndex = m_evalList_.m_squareHandToList[g_HandPieceToSquareHand[us][hpTo] + handnum];
		const Piece pcTo = UtilPiece::FromColorAndPieceType(us, ptTo);
		m_st_->m_cl.m_listindex[0] = listIndex;
		m_st_->m_cl.m_clistpair[0].m_oldlist[0] = m_evalList_.m_list0[listIndex];
		m_st_->m_cl.m_clistpair[0].m_oldlist[1] = m_evalList_.m_list1[listIndex];

		m_evalList_.m_list0[listIndex] = kppArray[pcTo         ] + to;
		m_evalList_.m_list1[listIndex] = kppArray[UtilPiece::Inverse(pcTo)] + UtilSquare::Inverse(to);
		m_evalList_.m_listToSquareHand[listIndex] = to;
		m_evalList_.m_squareHandToList[to] = listIndex;

		m_st_->m_cl.m_clistpair[0].m_newlist[0] = m_evalList_.m_list0[listIndex];
		m_st_->m_cl.m_clistpair[0].m_newlist[1] = m_evalList_.m_list1[listIndex];

		m_hand_[us].MinusOne(hpTo);
		XorBBs(ptTo, to, us);
		m_piece_[to] = UtilPiece::FromColorAndPieceType(us, ptTo);

		if (moveIsCheck) {
			// Direct checks
			m_st_->m_checkersBB = UtilBitboard::SetMaskBB(to);
			m_st_->m_continuousCheck[us] += 2;
		}
		else {
			m_st_->m_checkersBB = Bitboard::AllZeroBB();
			m_st_->m_continuousCheck[us] = 0;
		}
	}
	else {
		const Square from = move.From();
		const PieceType ptFrom = move.GetPieceTypeFrom();
		ptTo = move.GetPieceTypeTo(ptFrom);

		UtilBitboard::XorBit(&m_byTypeBB_[ptFrom],from);
		UtilBitboard::XorBit(&m_byTypeBB_[ptTo],to);
		UtilBitboard::XorBit(&m_byColorBB_[us],from, to);
		m_piece_[from] = Empty;
		m_piece_[to] = UtilPiece::FromColorAndPieceType(us, ptTo);
		boardKey -= GetZobrist(ptFrom, from, us);
		boardKey += GetZobrist(ptTo, to, us);

		if (ptCaptured) {
			// 駒を取ったとき
			const HandPiece hpCaptured = UtilHandPiece::FromPieceType(ptCaptured);
			const Color them = UtilColor::OppositeColor(us);

			boardKey -= GetZobrist(ptCaptured, to, them);
			handKey += GetZobHand(hpCaptured, us);

			UtilBitboard::XorBit(&m_byTypeBB_[ptCaptured],to);
			UtilBitboard::XorBit(&m_byColorBB_[them],to);

			m_hand_[us].PlusOne(hpCaptured);
			const int toListIndex = m_evalList_.m_squareHandToList[to];
			m_st_->m_cl.m_listindex[1] = toListIndex;
			m_st_->m_cl.m_clistpair[1].m_oldlist[0] = m_evalList_.m_list0[toListIndex];
			m_st_->m_cl.m_clistpair[1].m_oldlist[1] = m_evalList_.m_list1[toListIndex];
			m_st_->m_cl.m_size = 2;

			const int handnum = GetHand(us).NumOf(hpCaptured);
			m_evalList_.m_list0[toListIndex] = kppHandArray[us  ][hpCaptured] + handnum;
			m_evalList_.m_list1[toListIndex] = kppHandArray[them][hpCaptured] + handnum;
			const Square squarehand = g_HandPieceToSquareHand[us][hpCaptured] + handnum;
			m_evalList_.m_listToSquareHand[toListIndex] = squarehand;
			m_evalList_.m_squareHandToList[squarehand]  = toListIndex;

			m_st_->m_cl.m_clistpair[1].m_newlist[0] = m_evalList_.m_list0[toListIndex];
			m_st_->m_cl.m_clistpair[1].m_newlist[1] = m_evalList_.m_list1[toListIndex];

			m_st_->m_material += (us == Black ? GetCapturePieceScore(ptCaptured) : -GetCapturePieceScore(ptCaptured));
		}
		prefetch(GetCsearcher()->m_tt.FirstEntry(boardKey + handKey));
		// Occupied は to, from の位置のビットを操作するよりも、
		// Black と White の or を取る方が速いはず。
		m_byTypeBB_[Occupied] = this->GetBbOf(Black) | GetBbOf(White);

		if (ptTo == King) {
			m_kingSquare_[us] = to;
		}
		else {
			const Piece pcTo = UtilPiece::FromColorAndPieceType(us, ptTo);
			const int fromListIndex = m_evalList_.m_squareHandToList[from];

			m_st_->m_cl.m_listindex[0] = fromListIndex;
			m_st_->m_cl.m_clistpair[0].m_oldlist[0] = m_evalList_.m_list0[fromListIndex];
			m_st_->m_cl.m_clistpair[0].m_oldlist[1] = m_evalList_.m_list1[fromListIndex];

			m_evalList_.m_list0[fromListIndex] = kppArray[pcTo         ] + to;
			m_evalList_.m_list1[fromListIndex] = kppArray[UtilPiece::Inverse(pcTo)] + UtilSquare::Inverse(to);
			m_evalList_.m_listToSquareHand[fromListIndex] = to;
			m_evalList_.m_squareHandToList[to] = fromListIndex;

			m_st_->m_cl.m_clistpair[0].m_newlist[0] = m_evalList_.m_list0[fromListIndex];
			m_st_->m_cl.m_clistpair[0].m_newlist[1] = m_evalList_.m_list1[fromListIndex];
		}

		if (move.IsPromotion()) {
			m_st_->m_material += (us == Black ?
							  (GetPieceScore(ptTo) - GetPieceScore(ptFrom))
							  : -(GetPieceScore(ptTo) - GetPieceScore(ptFrom)));
		}

		if (moveIsCheck) {
			// Direct checks
			m_st_->m_checkersBB = ci.m_checkBB[ptTo] & UtilBitboard::SetMaskBB(to);

			// Discovery checks
			const Square ksq = GetKingSquare(UtilColor::OppositeColor(us));
			if (IsDiscoveredCheck(from, to, ksq, ci.m_dcBB)) {
				switch (UtilSquare::GetSquareRelation(from, ksq)) {
				case DirecMisc: assert(false); break; // 最適化の為のダミー
				case DirecFile:
					// from の位置から縦に利きを調べると相手玉と、空き王手している駒に当たっているはず。味方の駒が空き王手している駒。
					m_st_->m_checkersBB |= UtilBitboard::RookAttackFile(&GetOccupiedBB(), from) & this->GetBbOf(us);
					break;
				case DirecRank:
					m_st_->m_checkersBB |= GetAttacksFrom<Rook>(ksq) & this->GetBbOf(Rook, Dragon, us);
					break;
				case DirecDiagNESW: case DirecDiagNWSE:
					m_st_->m_checkersBB |= GetAttacksFrom<Bishop>(ksq) & this->GetBbOf(Bishop, Horse, us);
					break;
				default: UNREACHABLE;
				}
			}
			m_st_->m_continuousCheck[us] += 2;
		}
		else {
			m_st_->m_checkersBB = Bitboard::AllZeroBB();
			m_st_->m_continuousCheck[us] = 0;
		}
	}
	m_goldsBB_ = this->GetBbOf(Gold, ProPawn, ProLance, ProKnight, ProSilver);

	m_st_->m_boardKey = boardKey;
	m_st_->m_handKey = handKey;
	++m_st_->m_pliesFromNull;

	m_turn_ = UtilColor::OppositeColor(us);
	m_st_->m_hand = GetHand(GetTurn());

	assert(IsOK());
}

void Position::UndoMove(const Move move) {
	assert(IsOK());
	assert(!move.IsNone());

	const Color them = GetTurn();
	const Color us = UtilColor::OppositeColor(them);
	const Square to = move.To();
	m_turn_ = us;
	// ここで先に turn_ を戻したので、以下、move は us の指し手とする。
	if (move.IsDrop()) {
		const PieceType ptTo = move.GetPieceTypeDropped();
		UtilBitboard::XorBit(&m_byTypeBB_[ptTo],to);
		UtilBitboard::XorBit(&m_byColorBB_[us],to);
		m_piece_[to] = Empty;

		const HandPiece hp = UtilHandPiece::FromPieceType(ptTo);
		m_hand_[us].PlusOne(hp);

		const int toListIndex  = m_evalList_.m_squareHandToList[to];
		const int handnum = GetHand(us).NumOf(hp);
		m_evalList_.m_list0[toListIndex] = kppHandArray[us  ][hp] + handnum;
		m_evalList_.m_list1[toListIndex] = kppHandArray[them][hp] + handnum;
		const Square squarehand = g_HandPieceToSquareHand[us][hp] + handnum;
		m_evalList_.m_listToSquareHand[toListIndex] = squarehand;
		m_evalList_.m_squareHandToList[squarehand]  = toListIndex;
	}
	else {
		const Square from = move.From();
		const PieceType ptFrom = move.GetPieceTypeFrom();
		const PieceType ptTo = move.GetPieceTypeTo(ptFrom);
		const PieceType ptCaptured = move.GetCap(); // todo: st_->capturedType 使えば良い。

		if (ptTo == King) {
			m_kingSquare_[us] = from;
		}
		else {
			const Piece pcFrom = UtilPiece::FromColorAndPieceType(us, ptFrom);
			const int toListIndex = m_evalList_.m_squareHandToList[to];
			m_evalList_.m_list0[toListIndex] = kppArray[pcFrom         ] + from;
			m_evalList_.m_list1[toListIndex] = kppArray[UtilPiece::Inverse(pcFrom)] + UtilSquare::Inverse(from);
			m_evalList_.m_listToSquareHand[toListIndex] = from;
			m_evalList_.m_squareHandToList[from] = toListIndex;
		}

		if (ptCaptured) {
			// 駒を取ったとき
		    UtilBitboard::XorBit(&m_byTypeBB_[ptCaptured],to);
			UtilBitboard::XorBit(&m_byColorBB_[them],to);
			const HandPiece hpCaptured = UtilHandPiece::FromPieceType(ptCaptured);
			const Piece pcCaptured = UtilPiece::FromColorAndPieceType(them, ptCaptured);
			m_piece_[to] = pcCaptured;

			const int handnum = GetHand(us).NumOf(hpCaptured);
			const int toListIndex = m_evalList_.m_squareHandToList[g_HandPieceToSquareHand[us][hpCaptured] + handnum];
			m_evalList_.m_list0[toListIndex] = kppArray[pcCaptured         ] + to;
			m_evalList_.m_list1[toListIndex] = kppArray[UtilPiece::Inverse(pcCaptured)] + UtilSquare::Inverse(to);
			m_evalList_.m_listToSquareHand[toListIndex] = to;
			m_evalList_.m_squareHandToList[to] = toListIndex;

			m_hand_[us].MinusOne(hpCaptured);
		}
		else {
			// 駒を取らないときは、UtilPiece::colorAndPieceTypeToPiece(us, ptCaptured) は 0 または 16 になる。
			// 16 になると困るので、駒を取らないときは明示的に Empty にする。
			m_piece_[to] = Empty;
		}
		UtilBitboard::XorBit(&m_byTypeBB_[ptFrom],from);
	    UtilBitboard::XorBit(&m_byTypeBB_[ptTo],to);
	    UtilBitboard::XorBit(&m_byColorBB_[us],from, to);
		m_piece_[from] = UtilPiece::FromColorAndPieceType(us, ptFrom);
	}
	// Occupied は to, from の位置のビットを操作するよりも、
	// Black と White の or を取る方が速いはず。
	m_byTypeBB_[Occupied] = this->GetBbOf(Black) | GetBbOf(White);
	m_goldsBB_ = this->GetBbOf(Gold, ProPawn, ProLance, ProKnight, ProSilver);

	// key などは StateInfo にまとめられているので、
	// previous のポインタを st_ に代入するだけで良い。
	m_st_ = m_st_->m_previous;

	assert(IsOK());
}

namespace {
	// SEE の順番
	template <PieceType PT> struct SEENextPieceType {}; // これはインスタンス化しない。
	template <> struct SEENextPieceType<Pawn     > { static const PieceType value = Lance;     };
	template <> struct SEENextPieceType<Lance    > { static const PieceType value = Knight;    };
	template <> struct SEENextPieceType<Knight   > { static const PieceType value = ProPawn;   };
	template <> struct SEENextPieceType<ProPawn  > { static const PieceType value = ProLance;  };
	template <> struct SEENextPieceType<ProLance > { static const PieceType value = ProKnight; };
	template <> struct SEENextPieceType<ProKnight> { static const PieceType value = Silver;    };
	template <> struct SEENextPieceType<Silver   > { static const PieceType value = ProSilver; };
	template <> struct SEENextPieceType<ProSilver> { static const PieceType value = Gold;      };
	template <> struct SEENextPieceType<Gold     > { static const PieceType value = Bishop;    };
	template <> struct SEENextPieceType<Bishop   > { static const PieceType value = Horse;     };
	template <> struct SEENextPieceType<Horse    > { static const PieceType value = Rook;      };
	template <> struct SEENextPieceType<Rook     > { static const PieceType value = Dragon;    };
	template <> struct SEENextPieceType<Dragon   > { static const PieceType value = King;      };

	template <PieceType PT> FORCE_INLINE PieceType nextAttacker(const Position& pos, const Square to, const Bitboard& opponentAttackers,
																Bitboard& occupied, Bitboard& attackers, const Color turn)
	{
		if (opponentAttackers.AndIsNot0(pos.GetBbOf(PT))) {
			const Bitboard bb = opponentAttackers & pos.GetBbOf(PT);
			const Square from = bb.ConstFirstOneFromI9();
			UtilBitboard::XorBit(&occupied,from);
			// todo: 実際に移動した方向を基にattackersを更新すれば、template, inline を使用しなくても良さそう。
			//       その場合、キャッシュに乗りやすくなるので逆に速くなるかも。
			if (PT == Pawn || PT == Lance) {
				attackers |= (UtilBitboard::LanceAttack(&occupied,UtilColor::OppositeColor(turn), to) & (pos.GetBbOf(Rook, Dragon) | pos.GetBbOf(Lance, turn)));
			}
			if (PT == Gold || PT == ProPawn || PT == ProLance || PT == ProKnight || PT == ProSilver || PT == Horse || PT == Dragon) {
				attackers |= (UtilBitboard::LanceAttack(&occupied,UtilColor::OppositeColor(turn), to) & pos.GetBbOf(Lance, turn))
					| (UtilBitboard::LanceAttack(&occupied,turn, to) & pos.GetBbOf(Lance, UtilColor::OppositeColor(turn)))
					| (UtilBitboard::RookAttack(&occupied,to) & pos.GetBbOf(Rook, Dragon))
					| (UtilBitboard::BishopAttack(&occupied,to) & pos.GetBbOf(Bishop, Horse));
			}
			if (PT == Silver) {
				attackers |= (UtilBitboard::LanceAttack(&occupied,UtilColor::OppositeColor(turn), to) & pos.GetBbOf(Lance, turn))
					| (UtilBitboard::RookAttack(&occupied,to) & pos.GetBbOf(Rook, Dragon))
					| (UtilBitboard::BishopAttack(&occupied,to) & pos.GetBbOf(Bishop, Horse));
			}
			if (PT == Bishop) {
				attackers |= (UtilBitboard::BishopAttack(&occupied,to) & pos.GetBbOf(Bishop, Horse));
			}
			if (PT == Rook) {
				attackers |= (UtilBitboard::LanceAttack(&occupied,UtilColor::OppositeColor(turn), to) & pos.GetBbOf(Lance, turn))
					| (UtilBitboard::LanceAttack(&occupied,turn, to) & pos.GetBbOf(Lance, UtilColor::OppositeColor(turn)))
					| (UtilBitboard::RookAttack(&occupied,to) & pos.GetBbOf(Rook, Dragon));
			}

			if (PT == Pawn || PT == Lance || PT == Knight) {
				if (UtilSquare::CanPromote(turn, UtilSquare::ToRank(to))) {
					return PT + PTPromote;
				}
			}
			if (PT == Silver || PT == Bishop || PT == Rook) {
				if (UtilSquare::CanPromote(turn, UtilSquare::ToRank(to)) || UtilSquare::CanPromote(turn, UtilSquare::ToRank(from))) {
					return PT + PTPromote;
				}
			}
			return PT;
		}
		return nextAttacker<SEENextPieceType<PT>::value>(pos, to, opponentAttackers, occupied, attackers, turn);
	}
	template <> FORCE_INLINE PieceType nextAttacker<King>(const Position&, const Square, const Bitboard&,
														  Bitboard&, Bitboard&, const Color)
	{
		return King;
	}
}

Score Position::GetSee(const Move move, const int asymmThreshold) const {
	const Square to = move.To();
	Square from;
	PieceType ptCaptured;
	Bitboard occ = GetOccupiedBB();
	Bitboard attackers;
	Bitboard opponentAttackers;
	Color turn = UtilColor::OppositeColor(this->GetTurn());
	Score swapList[32];
	if (move.IsDrop()) {
		opponentAttackers = GetAttackersTo(turn, to, occ);
		if (!opponentAttackers.IsNot0()) {
			return ScoreZero;
		}
		attackers = opponentAttackers | GetAttackersTo(UtilColor::OppositeColor(turn), to, occ);
		swapList[0] = ScoreZero;
		ptCaptured = move.GetPieceTypeDropped();
	}
	else {
		from = move.From();
		UtilBitboard::XorBit(&occ,from);
		opponentAttackers = GetAttackersTo(turn, to, occ);
		if (!opponentAttackers.IsNot0()) {
			if (move.IsPromotion()) {
				const PieceType ptFrom = move.GetPieceTypeFrom();
				return GetCapturePieceScore(move.GetCap()) + GetPromotePieceScore(ptFrom);
			}
			return GetCapturePieceScore(move.GetCap());
		}
		attackers = opponentAttackers | GetAttackersTo(UtilColor::OppositeColor(turn), to, occ);
		swapList[0] = GetCapturePieceScore(move.GetCap());
		ptCaptured = move.GetPieceTypeFrom();
		if (move.IsPromotion()) {
			const PieceType ptFrom = move.GetPieceTypeFrom();
			swapList[0] += GetPromotePieceScore(ptFrom);
			ptCaptured += PTPromote;
		}
	}

	int slIndex = 1;
	do {
		swapList[slIndex] = -swapList[slIndex - 1] + GetCapturePieceScore(ptCaptured);

		ptCaptured = nextAttacker<Pawn>(*this, to, opponentAttackers, occ, attackers, turn);

		attackers &= occ;
		++slIndex;
		turn = UtilColor::OppositeColor(turn);
		opponentAttackers = attackers & GetBbOf(turn);

		if (ptCaptured == King) {
			if (opponentAttackers.IsNot0()) {
				swapList[slIndex++] = g_CaptureKingScore;
			}
			break;
		}
	} while (opponentAttackers.IsNot0());

	if (asymmThreshold) {
		for (int i = 0; i < slIndex; i += 2) {
			if (swapList[i] < asymmThreshold) {
				swapList[i] = -g_CaptureKingScore;
			}
		}
	}

	// nega max 的に駒の取り合いの点数を求める。
	while (--slIndex) {
		swapList[slIndex-1] = std::min(-swapList[slIndex], swapList[slIndex-1]);
	}
	return swapList[0];
}

Score Position::GetSeeSign(const Move move) const {
	if (move.IsCapture()) {
		const PieceType ptFrom = move.GetPieceTypeFrom();
		const Square to = move.To();
		if (GetCapturePieceScore(ptFrom) <= GetCapturePieceScore(GetPiece(to))) {
			return static_cast<Score>(1);
		}
	}
	return GetSee(move);
}

namespace {
	// them(相手) 側の玉が逃げられるか。
	// sq : 王手した相手の駒の位置。紐付きか、桂馬の位置とする。よって、玉は sq には行けない。
	// bb : sq の利きのある場所のBitboard。よって、玉は bb のビットが立っている場所には行けない。
	// sq と ksq の位置の Occupied Bitboard のみは、ここで更新して評価し、元に戻す。
	// (実際にはテンポラリのOccupied Bitboard を使うので、元には戻さない。)
	bool canKingEscape(const Position& pos, const Color us, const Square sq, const Bitboard& bb) {
		const Color them = UtilColor::OppositeColor(us);
		const Square ksq = pos.GetKingSquare(them);
		Bitboard kingMoveBB = bb.NotThisAnd(pos.GetBbOf(them).NotThisAnd(g_kingAttackBb.KingAttack(ksq)));
		UtilBitboard::ClearBit(&kingMoveBB,sq); // sq には行けないので、クリアする。xorBit(sq)ではダメ。

		if (kingMoveBB.IsNot0()) {
			Bitboard tempOccupied = pos.GetOccupiedBB();
			UtilBitboard::SetBit(&tempOccupied,sq);
		    UtilBitboard::ClearBit(&tempOccupied,ksq);
			do {
				const Square to = kingMoveBB.FirstOneFromI9();
				// 玉の移動先に、us 側の利きが無ければ、true
				if (!pos.IsAttackersToIsNot0(us, to, tempOccupied)) {
					return true;
				}
			} while (kingMoveBB.IsNot0());
		}
		// 玉の移動先が無い。
		return false;
	}
	// them(相手) 側の玉以外の駒が sq にある us 側の駒を取れるか。
	bool canPieceCapture(const Position& pos, const Color them, const Square sq, const Bitboard& dcBB) {
		// 玉以外で打った駒を取れる相手側の駒の Bitboard
		Bitboard fromBB = pos.GetAttackersToExceptKing(them, sq);

		if (fromBB.IsNot0()) {
			const Square ksq = pos.GetKingSquare(them);
			do {
				const Square from = fromBB.FirstOneFromI9();
				if (!pos.IsDiscoveredCheck(from, sq, ksq, dcBB)) {
					// them 側から見て、pin されていない駒で、打たれた駒を取れるので、true
					return true;
				}
			} while (fromBB.IsNot0());
		}
		// 玉以外の駒で、打った駒を取れない。
		return false;
	}

	// pos.discoveredCheckBB<false>() を遅延評価するバージョン。
	bool canPieceCapture(const Position& pos, const Color them, const Square sq) {
		Bitboard fromBB = pos.GetAttackersToExceptKing(them, sq);

		if (fromBB.IsNot0()) {
			const Square ksq = pos.GetKingSquare(them);
			const Bitboard dcBB = pos.DiscoveredCheckBB<false>();
			do {
				const Square from = fromBB.FirstOneFromI9();
				if (!pos.IsDiscoveredCheck(from, sq, ksq, dcBB)) {
					// them 側から見て、pin されていない駒で、打たれた駒を取れるので、true
					return true;
				}
			} while (fromBB.IsNot0());
		}
		// 玉以外の駒で、打った駒を取れない。
		return false;
	}
}

// us が sq へ歩を打ったとき、them の玉が詰むか。
// us が sq へ歩を打つのは王手であると仮定する。
// 打ち歩詰めのとき、true を返す。
bool Position::IsPawnDropCheckMate(const Color us, const Square sq) const {
	const Color them = UtilColor::OppositeColor(us);
	// 玉以外の駒で、打たれた歩が取れるなら、打ち歩詰めではない。
	if (canPieceCapture(*this, them, sq)) {
		return false;
	}
	// todo: ここで玉の位置を求めるのは、上位で求めたものと2重になるので無駄。後で整理すること。
	const Square ksq = GetKingSquare(them);

	// 玉以外で打った歩を取れないとき、玉が歩を取るか、玉が逃げるか。

	// 利きを求める際に、occupied の歩を打った位置の bit を立てた Bitboard を使用する。
	// ここでは歩の Bitboard は更新する必要がない。
	// color の Bitboard も更新する必要がない。(相手玉が動くとき、こちらの打った歩で玉を取ることは無い為。)
	const Bitboard tempOccupied = GetOccupiedBB() | UtilBitboard::SetMaskBB(sq);
	Bitboard kingMoveBB = GetBbOf(them).NotThisAnd(g_kingAttackBb.KingAttack(ksq));

	// 少なくとも歩を取る方向には玉が動けるはずなので、do while を使用。
	assert(kingMoveBB.IsNot0());
	do {
		const Square to = kingMoveBB.FirstOneFromI9();
		if (!IsAttackersToIsNot0(us, to, tempOccupied)) {
			// 相手玉の移動先に自駒の利きがないなら、打ち歩詰めではない。
			return false;
		}
	} while (kingMoveBB.IsNot0());

	return true;
}

inline void Position::XorBBs(const PieceType pt, const Square sq, const Color c) {
	UtilBitboard::XorBit(&m_byTypeBB_[Occupied],sq);
	UtilBitboard::XorBit(&m_byTypeBB_[pt],sq);
	UtilBitboard::XorBit(&m_byColorBB_[c],sq);
}

// 相手玉が1手詰みかどうかを判定。
// 1手詰みなら、詰みに至る指し手の一部の情報(from, to のみとか)を返す。
// 1手詰みでないなら、Move::moveNone() を返す。
// Bitboard の状態を途中で更新する為、const 関数ではない。(更新後、元に戻すが。)
template <Color US> Move Position::GetMateMoveIn1Ply() {
	const Color Them = UtilColor::OppositeColor(US);
	const Square ksq = GetKingSquare(Them);
	const SquareDelta TDeltaS = (US == Black ? DeltaS : DeltaN);

	assert(!IsAttackersToIsNot0(Them, GetKingSquare(US)));

	// 駒打ちを調べる。
	const Bitboard dropTarget = GetNOccupiedBB(); // emptyBB() ではないので注意して使うこと。
	const Hand ourHand = GetHand(US);
	// 王手する前の状態の dcBB。
	// 間にある駒は相手側の駒。
	// 駒打ちのときは、打った後も、打たれる前の状態の dcBB を使用する。
	const Bitboard dcBB_betweenIsThem = DiscoveredCheckBB<false>();

	// 飛車打ち
	if (ourHand.Exists<HRook>()) {
		// 合駒されるとややこしいので、3手詰み関数の中で調べる。
		// ここでは離れた位置から王手するのは考えない。
		Bitboard toBB = dropTarget & UtilBitboard::RookStepAttacks(ksq);
		while (toBB.IsNot0()) {
			const Square to = toBB.FirstOneFromI9();
			// 駒を打った場所に自駒の利きがあるか。(無ければ玉で取られて詰まない)
			if (IsAttackersToIsNot0(US, to)) {
				// 玉が逃げられず、他の駒で取ることも出来ないか
				if (!canKingEscape(*this, US, to, UtilBitboard::RookAttackToEdge(to))
					&& !canPieceCapture(*this, Them, to, dcBB_betweenIsThem))
				{
					return UtilMove::MakeDropMove(Rook, to);
				}
			}
		}
	}
	// 香車打ち
	// 飛車で詰まなければ香車でも詰まないので、else if を使用。
	// 玉が 9(1) 段目にいれば香車で王手出来無いので、それも省く。
	else if (ourHand.Exists<HLance>() && UtilSquare::IsInFrontOf<US, Rank1, Rank9>(UtilSquare::ToRank(ksq))) {
		const Square to = ksq + TDeltaS;
		if (GetPiece(to) == Empty && IsAttackersToIsNot0(US, to)) {
			if (!canKingEscape(*this, US, to, UtilBitboard::LanceAttackToEdge(US, to))
				&& !canPieceCapture(*this, Them, to, dcBB_betweenIsThem))
			{
				return UtilMove::MakeDropMove(Lance, to);
			}
		}
	}

	// 角打ち
	if (ourHand.Exists<HBishop>()) {
		Bitboard toBB = dropTarget & UtilBitboard::BishopStepAttacks(ksq);
		while (toBB.IsNot0()) {
			const Square to = toBB.FirstOneFromI9();
			if (IsAttackersToIsNot0(US, to)) {
				if (!canKingEscape(*this, US, to, UtilBitboard::BishopAttackToEdge(to))
					&& !canPieceCapture(*this, Them, to, dcBB_betweenIsThem))
				{
					return UtilMove::MakeDropMove(Bishop, to);
				}
			}
		}
	}

	// 金打ち
	if (ourHand.Exists<HGold>()) {
		Bitboard toBB;
		if (ourHand.Exists<HRook>()) {
			// 飛車打ちを先に調べたので、尻金だけは省く。
			toBB = dropTarget & (UtilBitboard::GoldAttack(Them, ksq) ^ UtilBitboard::PawnAttack(US, ksq));
		}
		else {
			toBB = dropTarget & UtilBitboard::GoldAttack(Them, ksq);
		}
		while (toBB.IsNot0()) {
			const Square to = toBB.FirstOneFromI9();
			if (IsAttackersToIsNot0(US, to)) {
				if (!canKingEscape(*this, US, to, UtilBitboard::GoldAttack(US, to))
					&& !canPieceCapture(*this, Them, to, dcBB_betweenIsThem))
				{
					return UtilMove::MakeDropMove(Gold, to);
				}
			}
		}
	}

	if (ourHand.Exists<HSilver>()) {
		Bitboard toBB;
		if (ourHand.Exists<HGold>()) {
			// 金打ちを先に調べたので、斜め後ろから打つ場合だけを調べる。

			if (ourHand.Exists<HBishop>()) {
				// 角打ちを先に調べたので、斜めからの王手も除外できる。銀打ちを調べる必要がない。
				goto silver_drop_end;
			}
			// 斜め後ろから打つ場合を調べる必要がある。
			toBB = dropTarget & (UtilBitboard::SilverAttack(Them, ksq) & BitboardMask::GetInFrontMask(US, UtilSquare::ToRank(ksq)));
		}
		else {
			if (ourHand.Exists<HBishop>()) {
				// 斜め後ろを除外。前方から打つ場合を調べる必要がある。
				toBB = dropTarget & UtilBitboard::GoldAndSilverAttacks(Them, ksq);
			}
			else {
				toBB = dropTarget & UtilBitboard::SilverAttack(Them, ksq);
			}
		}
		while (toBB.IsNot0()) {
			const Square to = toBB.FirstOneFromI9();
			if (IsAttackersToIsNot0(US, to)) {
				if (!canKingEscape(*this, US, to, UtilBitboard::SilverAttack(US, to))
					&& !canPieceCapture(*this, Them, to, dcBB_betweenIsThem))
				{
					return UtilMove::MakeDropMove(Silver, to);
				}
			}
		}
	}
silver_drop_end:

	if (ourHand.Exists<HKnight>()) {
		Bitboard toBB = dropTarget & UtilBitboard::KnightAttack(Them, ksq);
		while (toBB.IsNot0()) {
			const Square to = toBB.FirstOneFromI9();
			// 桂馬は紐が付いている必要はない。
			// よって、このcanKingEscape() 内での to の位置に逃げられないようにする処理は無駄。
			if (!canKingEscape(*this, US, to, Bitboard::AllZeroBB())
				&& !canPieceCapture(*this, Them, to, dcBB_betweenIsThem))
			{
				return UtilMove::MakeDropMove(Knight, to);
			}
		}
	}

	// 歩打ちで詰ますと反則なので、調べない。

	// 駒を移動する場合
	// moveTarget は桂馬以外の移動先の大まかな位置。飛角香の遠隔王手は含まない。
	const Bitboard moveTarget = GetBbOf(US).NotThisAnd(g_kingAttackBb.KingAttack(ksq));
	const Bitboard pinned = GetPinnedBB();
	const Bitboard dcBB_betweenIsUs = DiscoveredCheckBB<true>();

	{
		// 竜による移動
		Bitboard fromBB = GetBbOf(Dragon, US);
		while (fromBB.IsNot0()) {
			const Square from = fromBB.FirstOneFromI9();
			// 遠隔王手は考えない。
			Bitboard toBB = moveTarget & GetAttacksFrom<Dragon>(from);
			if (toBB.IsNot0()) {
				XorBBs(Dragon, from, US);
				// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
				const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();
				// to の位置の Bitboard は canKingEscape の中で更新する。
				do {
					const Square to = toBB.FirstOneFromI9();
					// 王手した駒の場所に自駒の利きがあるか。(無ければ玉で取られて詰まない)
					if (IsUnDropCheckIsSupported(US, to)) {
						// 玉が逃げられない
						// かつ、(空き王手 または 他の駒で取れない)
						// かつ、王手した駒が pin されていない
						if (!canKingEscape(*this, US, to, GetAttacksFrom<Dragon>(to, GetOccupiedBB() ^ UtilBitboard::SetMaskBB(ksq)))
							&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
								|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
							&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
						{
							XorBBs(Dragon, from, US);
							return UtilMove::MakeCaptureMove(Dragon, from, to, *this);
						}
					}
				} while (toBB.IsNot0());
				XorBBs(Dragon, from, US);
			}
		}
	}

	// Txxx は先手、後手の情報を吸収した変数。数字は先手に合わせている。
	const Rank TRank6 = (US == Black ? Rank6 : Rank4);
	const Bitboard TRank789BB = BitboardMask::GetInFrontMask<US, TRank6>();
	{
		// 飛車による移動
		Bitboard fromBB = GetBbOf(Rook, US);
		Bitboard fromOn789BB = fromBB & TRank789BB;
		// from が 123 段目
		if (fromOn789BB.IsNot0()) {
			fromBB.AndEqualNot(TRank789BB);
			do {
				const Square from = fromOn789BB.FirstOneFromI9();
				Bitboard toBB = moveTarget & GetAttacksFrom<Rook>(from);
				if (toBB.IsNot0()) {
					XorBBs(Rook, from, US);
					// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
					const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();
					// to の位置の Bitboard は canKingEscape の中で更新する。
					do {
						const Square to = toBB.FirstOneFromI9();
						if (IsUnDropCheckIsSupported(US, to)) {
							if (!canKingEscape(*this, US, to, GetAttacksFrom<Dragon>(to, GetOccupiedBB() ^ UtilBitboard::SetMaskBB(ksq)))
								&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
									|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
								&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
							{
								XorBBs(Rook, from, US);
								return UtilMove::MakeCapturePromoteMove(Rook, from, to, *this);
							}
						}
					} while (toBB.IsNot0());
					XorBBs(Rook, from, US);
				}
			} while (fromOn789BB.IsNot0());
		}

		// from が 4~9 段目
		while (fromBB.IsNot0()) {
			const Square from = fromBB.FirstOneFromI9();
			Bitboard toBB = moveTarget & GetAttacksFrom<Rook>(from) & (UtilBitboard::RookStepAttacks(ksq) | TRank789BB);
			if (toBB.IsNot0()) {
				XorBBs(Rook, from, US);
				// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
				const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();

				Bitboard toOn789BB = toBB & TRank789BB;
				// 成り
				if (toOn789BB.IsNot0()) {
					do {
						const Square to = toOn789BB.FirstOneFromI9();
						if (IsUnDropCheckIsSupported(US, to)) {
							if (!canKingEscape(*this, US, to, GetAttacksFrom<Dragon>(to, GetOccupiedBB() ^ UtilBitboard::SetMaskBB(ksq)))
								&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
									|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
								&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
							{
								XorBBs(Rook, from, US);
								return UtilMove::MakeCapturePromoteMove(Rook, from, to, *this);
							}
						}
					} while (toOn789BB.IsNot0());

					toBB.AndEqualNot(TRank789BB);
				}
				// 不成
				while (toBB.IsNot0()) {
					const Square to = toBB.FirstOneFromI9();
					if (IsUnDropCheckIsSupported(US, to)) {
						if (!canKingEscape(*this, US, to, UtilBitboard::RookAttackToEdge(to))
							&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
								|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
							&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
						{
							XorBBs(Rook, from, US);
							return UtilMove::MakeCaptureMove(Rook, from, to, *this);
						}
					}
				}
				XorBBs(Rook, from, US);
			}
		}
	}

	{
		// 馬による移動
		Bitboard fromBB = GetBbOf(Horse, US);
		while (fromBB.IsNot0()) {
			const Square from = fromBB.FirstOneFromI9();
			// 遠隔王手は考えない。
			Bitboard toBB = moveTarget & GetAttacksFrom<Horse>(from);
			if (toBB.IsNot0()) {
				XorBBs(Horse, from, US);
				// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
				const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();
				// to の位置の Bitboard は canKingEscape の中で更新する。
				do {
					const Square to = toBB.FirstOneFromI9();
					// 王手した駒の場所に自駒の利きがあるか。(無ければ玉で取られて詰まない)
					if (IsUnDropCheckIsSupported(US, to)) {
						// 玉が逃げられない
						// かつ、(空き王手 または 他の駒で取れない)
						// かつ、動かした駒が pin されていない)
						if (!canKingEscape(*this, US, to, UtilBitboard::HorseAttackToEdge(to)) // 竜の場合と違って、常に最大の利きを使用して良い。
							&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
								|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
							&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
						{
							XorBBs(Horse, from, US);
							return UtilMove::MakeCaptureMove(Horse, from, to, *this);
						}
					}
				} while (toBB.IsNot0());
				XorBBs(Horse, from, US);
			}
		}
	}

	{
		// 角による移動
		Bitboard fromBB = GetBbOf(Bishop, US);
		Bitboard fromOn789BB = fromBB & TRank789BB;
		// from が 123 段目
		if (fromOn789BB.IsNot0()) {
			fromBB.AndEqualNot(TRank789BB);
			do {
				const Square from = fromOn789BB.FirstOneFromI9();
				Bitboard toBB = moveTarget & GetAttacksFrom<Bishop>(from);
				if (toBB.IsNot0()) {
					XorBBs(Bishop, from, US);
					// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
					const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();
					// to の位置の Bitboard は canKingEscape の中で更新する。
					do {
						const Square to = toBB.FirstOneFromI9();
						if (IsUnDropCheckIsSupported(US, to)) {
							if (!canKingEscape(*this, US, to, UtilBitboard::HorseAttackToEdge(to))
								&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
									|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
								&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
							{
								XorBBs(Bishop, from, US);
								return UtilMove::MakeCapturePromoteMove(Bishop, from, to, *this);
							}
						}
					} while (toBB.IsNot0());
					XorBBs(Bishop, from, US);
				}
			} while (fromOn789BB.IsNot0());
		}

		// from が 4~9 段目
		while (fromBB.IsNot0()) {
			const Square from = fromBB.FirstOneFromI9();
			Bitboard toBB = moveTarget & GetAttacksFrom<Bishop>(from) & (UtilBitboard::BishopStepAttacks(ksq) | TRank789BB);
			if (toBB.IsNot0()) {
				XorBBs(Bishop, from, US);
				// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
				const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();

				Bitboard toOn789BB = toBB & TRank789BB;
				// 成り
				if (toOn789BB.IsNot0()) {
					do {
						const Square to = toOn789BB.FirstOneFromI9();
						if (IsUnDropCheckIsSupported(US, to)) {
							if (!canKingEscape(*this, US, to, UtilBitboard::HorseAttackToEdge(to))
								&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
									|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
								&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
							{
								XorBBs(Bishop, from, US);
								return UtilMove::MakeCapturePromoteMove(Bishop, from, to, *this);
							}
						}
					} while (toOn789BB.IsNot0());

					toBB.AndEqualNot(TRank789BB);
				}
				// 不成
				while (toBB.IsNot0()) {
					const Square to = toBB.FirstOneFromI9();
					if (IsUnDropCheckIsSupported(US, to)) {
						if (!canKingEscape(*this, US, to, UtilBitboard::BishopAttackToEdge(to))
							&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
								|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
							&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
						{
							XorBBs(Bishop, from, US);
							return UtilMove::MakeCaptureMove(Bishop, from, to, *this);
						}
					}
				}
				XorBBs(Bishop, from, US);
			}
		}
	}

	{
		// 金、成り金による移動
		Bitboard fromBB = GetGoldsBB(US) & UtilBitboard::GoldCheckTable(US, ksq);
		while (fromBB.IsNot0()) {
			const Square from = fromBB.FirstOneFromI9();
			Bitboard toBB = moveTarget & GetAttacksFrom<Gold>(US, from) & GetAttacksFrom<Gold>(Them, ksq);
			if (toBB.IsNot0()) {
				const PieceType pt = UtilPiece::ToPieceType(GetPiece(from));
				XorBBs(pt, from, US);
			    UtilBitboard::XorBit(&m_goldsBB_,from);
				// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
				const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();
				// to の位置の Bitboard は canKingEscape の中で更新する。
				do {
					const Square to = toBB.FirstOneFromI9();
					// 王手した駒の場所に自駒の利きがあるか。(無ければ玉で取られて詰まない)
					if (IsUnDropCheckIsSupported(US, to)) {
						// 玉が逃げられない
						// かつ、(空き王手 または 他の駒で取れない)
						// かつ、動かした駒が pin されていない)
						if (!canKingEscape(*this, US, to, GetAttacksFrom<Gold>(US, to))
							&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
								|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
							&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
						{
							XorBBs(pt, from, US);
							UtilBitboard::XorBit(&m_goldsBB_,from);
							return UtilMove::MakeCaptureMove(pt, from, to, *this);
						}
					}
				} while (toBB.IsNot0());
				XorBBs(pt, from, US);
				UtilBitboard::XorBit(&m_goldsBB_,from);
			}
		}
	}

	{
		// 銀による移動
		Bitboard fromBB = GetBbOf(Silver, US) & UtilBitboard::SilverCheckTable(US, ksq);
		if (fromBB.IsNot0()) {
			// Txxx は先手、後手の情報を吸収した変数。数字は先手に合わせている。
			const Bitboard TRank1_5BB = BitboardMask::GetInFrontMask<Them, TRank6>();
			const Bitboard chkBB = GetAttacksFrom<Silver>(Them, ksq);
			const Bitboard chkBB_promo = GetAttacksFrom<Gold>(Them, ksq);

			Bitboard fromOn789BB = fromBB & TRank789BB;
			// from が敵陣
			if (fromOn789BB.IsNot0()) {
				fromBB.AndEqualNot(TRank789BB);
				do {
					const Square from = fromOn789BB.FirstOneFromI9();
					Bitboard toBB = moveTarget & GetAttacksFrom<Silver>(US, from);
					Bitboard toBB_promo = toBB & chkBB_promo;

					toBB &= chkBB;
					if ((toBB_promo | toBB).IsNot0()) {
						XorBBs(Silver, from, US);
						// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
						const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();
						// to の位置の Bitboard は canKingEscape の中で更新する。
						while (toBB_promo.IsNot0()) {
							const Square to = toBB_promo.FirstOneFromI9();
							if (IsUnDropCheckIsSupported(US, to)) {
								// 成り
								if (!canKingEscape(*this, US, to, GetAttacksFrom<Gold>(US, to))
									&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
										|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
									&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
								{
									XorBBs(Silver, from, US);
									return UtilMove::MakeCapturePromoteMove(Silver, from, to, *this);
								}
							}
						}

						// 玉の前方に移動する場合、成で詰まなかったら不成でも詰まないので、ここで省く。
						// sakurapyon の作者が言ってたので実装。
						toBB.AndEqualNot(BitboardMask::GetInFrontMask(Them, UtilSquare::ToRank(ksq)));
						while (toBB.IsNot0()) {
							const Square to = toBB.FirstOneFromI9();
							if (IsUnDropCheckIsSupported(US, to)) {
								// 不成
								if (!canKingEscape(*this, US, to, GetAttacksFrom<Silver>(US, to))
									&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
										|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
									&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
								{
									XorBBs(Silver, from, US);
									return UtilMove::MakeCaptureMove(Silver, from, to, *this);
								}
							}
						}

						XorBBs(Silver, from, US);
					}
				} while (fromOn789BB.IsNot0());
			}

			// from が 5~9段目 (必ず不成)
			Bitboard fromOn1_5BB = fromBB & TRank1_5BB;
			if (fromOn1_5BB.IsNot0()) {
				fromBB.AndEqualNot(TRank1_5BB);
				do {
					const Square from = fromOn1_5BB.FirstOneFromI9();
					Bitboard toBB = moveTarget & GetAttacksFrom<Silver>(US, from) & chkBB;

					if (toBB.IsNot0()) {
						XorBBs(Silver, from, US);
						// 動いた後の dcBB, pinned: to の位置の occupied や checkers は関係ないので、ここで生成できる。
						const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();
						// to の位置の Bitboard は canKingEscape の中で更新する。
						while (toBB.IsNot0()) {
							const Square to = toBB.FirstOneFromI9();
							if (IsUnDropCheckIsSupported(US, to)) {
								// 不成
								if (!canKingEscape(*this, US, to, GetAttacksFrom<Silver>(US, to))
									&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
										|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
									&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
								{
									XorBBs(Silver, from, US);
									return UtilMove::MakeCaptureMove(Silver, from, to, *this);
								}
							}
						}

						XorBBs(Silver, from, US);
					}
				} while (fromOn1_5BB.IsNot0());
			}

			// 残り 4 段目のみ
			// 前進するときは成れるが、後退するときは成れない。
			while (fromBB.IsNot0()) {
				const Square from = fromBB.FirstOneFromI9();
				Bitboard toBB = moveTarget & GetAttacksFrom<Silver>(US, from);
				Bitboard toBB_promo = toBB & TRank789BB & chkBB_promo; // 3 段目にしか成れない。

				toBB &= chkBB;
				if ((toBB_promo | toBB).IsNot0()) {
					XorBBs(Silver, from, US);
					// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
					const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();
					// to の位置の Bitboard は canKingEscape の中で更新する。
					while (toBB_promo.IsNot0()) {
						const Square to = toBB_promo.FirstOneFromI9();
						if (IsUnDropCheckIsSupported(US, to)) {
							// 成り
							if (!canKingEscape(*this, US, to, GetAttacksFrom<Gold>(US, to))
								&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
									|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
								&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
							{
								XorBBs(Silver, from, US);
								return UtilMove::MakeCapturePromoteMove(Silver, from, to, *this);
							}
						}
					}

					while (toBB.IsNot0()) {
						const Square to = toBB.FirstOneFromI9();
						if (IsUnDropCheckIsSupported(US, to)) {
							// 不成
							if (!canKingEscape(*this, US, to, GetAttacksFrom<Silver>(US, to))
								&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
									|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
								&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
							{
								XorBBs(Silver, from, US);
								return UtilMove::MakeCaptureMove(Silver, from, to, *this);
							}
						}
					}

					XorBBs(Silver, from, US);
				}
			}
		}
	}

	{
		// 桂による移動
		Bitboard fromBB = GetBbOf(Knight, US) & UtilBitboard::KnightCheckTable(US, ksq);
		if (fromBB.IsNot0()) {
			const Bitboard chkBB_promo = GetAttacksFrom<Gold>(Them, ksq) & TRank789BB;
			const Bitboard chkBB = GetAttacksFrom<Knight>(Them, ksq);

			do {
				const Square from = fromBB.FirstOneFromI9();
				Bitboard toBB = GetBbOf(US).NotThisAnd(GetAttacksFrom<Knight>(US, from));
				Bitboard toBB_promo = toBB & chkBB_promo;
				toBB &= chkBB;
				if ((toBB_promo | toBB).IsNot0()) {
					XorBBs(Knight, from, US);
					// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
					const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();
					// to の位置の Bitboard は canKingEscape の中で更新する。
					while (toBB_promo.IsNot0()) {
						const Square to = toBB_promo.FirstOneFromI9();
						if (IsUnDropCheckIsSupported(US, to)) {
							// 成り
							if (!canKingEscape(*this, US, to, GetAttacksFrom<Gold>(US, to))
								&& (IsDiscoveredCheck<true>(from, to, ksq, dcBB_betweenIsUs)
									|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
								&& !IsPinnedIllegal<true>(from, to, GetKingSquare(US), pinned))
							{
								XorBBs(Knight, from, US);
								return UtilMove::MakeCapturePromoteMove(Knight, from, to, *this);
							}
						}
					}

					while (toBB.IsNot0()) {
						const Square to = toBB.FirstOneFromI9();
						// 桂馬は紐が付いてなくて良いので、紐が付いているかは調べない。
						// 不成
						if (!canKingEscape(*this, US, to, Bitboard::AllZeroBB())
							&& (IsDiscoveredCheck<true>(from, to, ksq, dcBB_betweenIsUs)
								|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
							&& !IsPinnedIllegal<true>(from, to, GetKingSquare(US), pinned))
						{
							XorBBs(Knight, from, US);
							return UtilMove::MakeCaptureMove(Knight, from, to, *this);
						}
					}
					XorBBs(Knight, from, US);
				}
			} while (fromBB.IsNot0());
		}
	}

	{
		// 香車による移動
		Bitboard fromBB = GetBbOf(Lance, US) & UtilBitboard::LanceCheckTable(US, ksq);
		if (fromBB.IsNot0()) {
			// Txxx は先手、後手の情報を吸収した変数。数字は先手に合わせている。
			const SquareDelta TDeltaS = (US == Black ? DeltaS : DeltaN);
			const Rank TRank8 = (US == Black ? Rank8 : Rank2);
			const Bitboard chkBB_promo = GetAttacksFrom<Gold>(Them, ksq) & TRank789BB;
			// 玉の前方1マスのみ。
			// 玉が 1 段目にいるときは、成のみで良いので省く。
			const Bitboard chkBB = GetAttacksFrom<Pawn>(Them, ksq) & BitboardMask::GetInFrontMask<Them, TRank8>();

			do {
				const Square from = fromBB.FirstOneFromI9();
				Bitboard toBB = moveTarget & GetAttacksFrom<Lance>(US, from);
				Bitboard toBB_promo = toBB & chkBB_promo;

				toBB &= chkBB;

				if ((toBB_promo | toBB).IsNot0()) {
					XorBBs(Lance, from, US);
					// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
					const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();
					// to の位置の Bitboard は canKingEscape の中で更新する。

					while (toBB_promo.IsNot0()) {
						const Square to = toBB_promo.FirstOneFromI9();
						if (IsUnDropCheckIsSupported(US, to)) {
							// 成り
							if (!canKingEscape(*this, US, to, GetAttacksFrom<Gold>(US, to))
								&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
									|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
								&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
							{
								XorBBs(Lance, from, US);
								return UtilMove::MakeCapturePromoteMove(Lance, from, to, *this);
							}
						}
					}

					if (toBB.IsNot0()) {
						assert(toBB.IsOneBit());
						// 不成で王手出来るのは、一つの場所だけなので、ループにする必要が無い。
						const Square to = ksq + TDeltaS;
						if (IsUnDropCheckIsSupported(US, to)) {
							// 不成
							if (!canKingEscape(*this, US, to, UtilBitboard::LanceAttackToEdge(US, to))
								&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
									|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
								&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
							{
								XorBBs(Lance, from, US);
								return UtilMove::MakeCaptureMove(Lance, from, to, *this);
							}
						}
					}
					XorBBs(Lance, from, US);
				}
			} while (fromBB.IsNot0());
		}
	}

	{
		// 歩による移動
		// 成れる場合は必ずなる。
		// todo: PawnCheckBB 作って簡略化する。
		const Rank krank = UtilSquare::ToRank(ksq);
		// 歩が移動して王手になるのは、相手玉が1~7段目の時のみ。
		if (UtilSquare::IsInFrontOf<US, Rank2, Rank8>(krank)) {
			// Txxx は先手、後手の情報を吸収した変数。数字は先手に合わせている。
			const SquareDelta TDeltaS = (US == Black ? DeltaS : DeltaN);
			const SquareDelta TDeltaN = (US == Black ? DeltaN : DeltaS);

			Bitboard fromBB = GetBbOf(Pawn, US);
			// 玉が敵陣にいないと成で王手になることはない。
			if (UtilSquare::IsInFrontOf<US, Rank6, Rank4>(krank)) {
				// 成った時に王手になる位置
				const Bitboard toBB_promo = moveTarget & GetAttacksFrom<Gold>(Them, ksq) & TRank789BB;
				Bitboard fromBB_promo = fromBB & toBB_promo.PawnAttack<Them>();
				while (fromBB_promo.IsNot0()) {
					const Square from = fromBB_promo.FirstOneFromI9();
					const Square to = from + TDeltaN;

					XorBBs(Pawn, from, US);
					// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
					const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();
					// to の位置の Bitboard は canKingEscape の中で更新する。
					if (IsUnDropCheckIsSupported(US, to)) {
						// 成り
						if (!canKingEscape(*this, US, to, GetAttacksFrom<Gold>(US, to))
							&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
								|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
							&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
						{
							XorBBs(Pawn, from, US);
							return UtilMove::MakeCapturePromoteMove(Pawn, from, to, *this);
						}
					}
					XorBBs(Pawn, from, US);
				}
			}

			// 不成
			// 玉が 8,9 段目にいることは無いので、from,to が隣の筋を指すことは無い。
			const Square to = ksq + TDeltaS;
			const Square from = to + TDeltaS;
			if (UtilBitboard::IsSet(&fromBB,from) && !UtilBitboard::IsSet(&GetBbOf(US),to)) {
				// 玉が 1, 2 段目にいるなら、成りで王手出来るので不成は調べない。
				if (UtilSquare::IsBehind<US, Rank8, Rank2>(krank)) {
					XorBBs(Pawn, from, US);
					// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
					const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();
					// to の位置の Bitboard は canKingEscape の中で更新する。
					if (IsUnDropCheckIsSupported(US, to)) {
						// 不成
						if (!canKingEscape(*this, US, to, Bitboard::AllZeroBB())
							&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
								|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
							&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
						{
							XorBBs(Pawn, from, US);
							return UtilMove::MakeCaptureMove(Pawn, from, to, *this);
						}
					}
					XorBBs(Pawn, from, US);
				}
			}
		}
	}

	return Move::GetMoveNone();
}

Move Position::GetMateMoveIn1Ply() {
	return (GetTurn() == Black ? GetMateMoveIn1Ply<Black>() : GetMateMoveIn1Ply<White>());
}

void Position::InitZobrist() {
	// zobTurn_ は 1 であり、その他は 1桁目を使わない。
	// zobTurn のみ xor で更新する為、他の桁に影響しないようにする為。
	// hash値の更新は普通は全て xor を使うが、持ち駒の更新の為に +, - を使用した方が都合が良い。
	for (PieceType pt = Occupied; pt < PieceTypeNum; ++pt) {
		for (Square sq = I9; sq < SquareNum; ++sq) {
			for (Color c = Black; c < ColorNum; ++c) {
				m_zobrist_[pt][sq][c] = g_mt64bit.GetRandom() & ~UINT64_C(1);
			}
		}
	}
	for (HandPiece hp = HPawn; hp < HandPieceNum; ++hp) {
		m_zobHand_[hp][Black] = g_mt64bit.GetRandom() & ~UINT64_C(1);
		m_zobHand_[hp][White] = g_mt64bit.GetRandom() & ~UINT64_C(1);
	}
	m_zobExclusion_ = g_mt64bit.GetRandom() & ~UINT64_C(1);
}

void Position::Print() const {
	std::cout << "'  9  8  7  6  5  4  3  2  1" << std::endl;
	int i = 0;
	for (Rank r = Rank9; r < RankNum; ++r) {
		++i;
		std::cout << "P" << i;
		for (File f = FileA; FileI <= f; --f) {
			std::cout << pieceToCharCSA(GetPiece(UtilSquare::FromFileRank(f, r)));
		}
		std::cout << std::endl;
	}
	PrintHand(Black);
	PrintHand(White);
	std::cout << (GetTurn() == Black ? "+" : "-") << std::endl;
	std::cout << std::endl;
	std::cout << "key = " << GetKey() << std::endl;
}

#if !defined NDEBUG
bool Position::IsOK() const {
	static Key prevKey;
	const bool debugAll = true;

	const bool debugBitboards    = debugAll || false;
	const bool debugKingCount    = debugAll || false;
	const bool debugKingCapture  = debugAll || false;
	const bool debugCheckerCount = debugAll || false;
	const bool debugKey          = debugAll || false;
	const bool debugStateHand    = debugAll || false;
	const bool debugPiece        = debugAll || false;
	const bool debugMaterial     = debugAll || false;

	int failedStep = 0;
	if (debugBitboards) {
		if ((GetBbOf(Black) & GetBbOf(White)).IsNot0()) {
			goto incorrect_position;
		}
		if ((GetBbOf(Black) | GetBbOf(White)) != GetOccupiedBB()) {
			goto incorrect_position;
		}
		if ((GetBbOf(Pawn     ) ^ GetBbOf(Lance    ) ^ GetBbOf(Knight) ^ GetBbOf(Silver ) ^ GetBbOf(Bishop  ) ^
			 GetBbOf(Rook     ) ^ GetBbOf(Gold     ) ^ GetBbOf(King  ) ^ GetBbOf(ProPawn) ^ GetBbOf(ProLance) ^
			 GetBbOf(ProKnight) ^ GetBbOf(ProSilver) ^ GetBbOf(Horse ) ^ GetBbOf(Dragon )) != GetOccupiedBB())
		{
			goto incorrect_position;
		}
		for (PieceType pt1 = Pawn; pt1 < PieceTypeNum; ++pt1) {
			for (PieceType pt2 = pt1 + 1; pt2 < PieceTypeNum; ++pt2) {
				if ((GetBbOf(pt1) & GetBbOf(pt2)).IsNot0()) {
					goto incorrect_position;
				}
			}
		}
	}

	++failedStep;
	if (debugKingCount) {
		int kingCount[ColorNum] = {0, 0};
		if (GetBbOf(King).PopCount() != 2) {
			goto incorrect_position;
		}
		if (!GetBbOf(King, Black).IsOneBit()) {
			goto incorrect_position;
		}
		if (!GetBbOf(King, White).IsOneBit()) {
			goto incorrect_position;
		}
		for (Square sq = I9; sq < SquareNum; ++sq) {
			if (GetPiece(sq) == BKing) {
				++kingCount[Black];
			}
			if (GetPiece(sq) == WKing) {
				++kingCount[White];
			}
		}
		if (kingCount[Black] != 1 || kingCount[White] != 1) {
			goto incorrect_position;
		}
	}

	++failedStep;
	if (debugKingCapture) {
		// 相手玉を取れないことを確認
		const Color us = GetTurn();
		const Color them = UtilColor::OppositeColor(us);
		const Square ksq = GetKingSquare(them);
		if (GetAttackersTo(us, ksq).IsNot0()) {
			goto incorrect_position;
		}
	}

	++failedStep;
	if (debugCheckerCount) {
		if (2 < m_st_->m_checkersBB.PopCount()) {
			goto incorrect_position;
		}
	}

	++failedStep;
	if (debugKey) {
		if (GetKey() != GetComputeKey()) {
			goto incorrect_position;
		}
	}

	++failedStep;
	if (debugStateHand) {
		if (m_st_->m_hand != m_hand(GetTurn())) {
			goto incorrect_position;
		}
	}

	++failedStep;
	if (debugPiece) {
		for (Square sq = I9; sq < SquareNum; ++sq) {
			const Piece pc = GetPiece(sq);
			if (pc == Empty) {
				if (!GetEmptyBB().IsSet(sq)) {
					goto incorrect_position;
				}
			}
			else {
				if (!GetBbOf(UtilPiece::ToPieceType(pc), UtilPiece::ToColor(pc)).IsSet(sq)) {
					goto incorrect_position;
				}
			}
		}
	}

	++failedStep;
	if (debugMaterial) {
		if (m_material() != ComputeMaterial()) {
			goto incorrect_position;
		}
	}

	++failedStep;
	{
		int i;
		if ((i = GetDebugSetEvalList()) != 0) {
			std::cout << "GetDebugSetEvalList() error = " << i << std::endl;
			goto incorrect_position;
		}
	}

	prevKey = GetKey();
	return true;

incorrect_position:
	std::cout << "Error! failedStep = " << failedStep << std::endl;
	std::cout << "prevKey = " << prevKey << std::endl;
	std::cout << "currKey = " << GetKey() << std::endl;
	Print();
	return false;
}
#endif

#if !defined NDEBUG
int Position::GetDebugSetEvalList() const {
	// not implement
	return 0;
}
#endif

Key Position::GetComputeBoardKey() const {
	Key result = 0;
	for (Square sq = I9; sq < SquareNum; ++sq) {
		if (GetPiece(sq) != Empty) {
			result += GetZobrist(UtilPiece::ToPieceType(GetPiece(sq)), sq, UtilPiece::ToColor(GetPiece(sq)));
		}
	}
	if (GetTurn() == White) {
		result ^= GetZobTurn();
	}
	return result;
}

Key Position::GetComputeHandKey() const {
	Key result = 0;
	for (HandPiece hp = HPawn; hp < HandPieceNum; ++hp) {
		for (Color c = Black; c < ColorNum; ++c) {
			const int num = GetHand(c).NumOf(hp);
			for (int i = 0; i < num; ++i) {
				result += GetZobHand(hp, c);
			}
		}
	}
	return result;
}

// todo: isRepetition() に名前変えた方が良さそう。
//       同一局面4回をきちんと数えていないけど問題ないか。
RepetitionType Position::IsDraw(const int checkMaxPly) const {
	const int Start = 4;
	int i = Start;
	const int e = std::min(m_st_->m_pliesFromNull, checkMaxPly);

	// 4手掛けないと千日手には絶対にならない。
	if (i <= e) {
		// 現在の局面と、少なくとも 4 手戻らないと同じ局面にならない。
		// ここでまず 2 手戻る。
		StateInfo* stp = m_st_->m_previous->m_previous;

		do {
			// 更に 2 手戻る。
			stp = stp->m_previous->m_previous;
			if (stp->GetKey() == m_st_->GetKey()) {
				if (i <= m_st_->m_continuousCheck[GetTurn()]) {
					return RepetitionLose;
				}
				else if (i <= m_st_->m_continuousCheck[UtilColor::OppositeColor(GetTurn())]) {
					return RepetitionWin;
				}
#if defined BAN_BLACK_REPETITION
				return (GetTurn() == Black ? RepetitionLose : RepetitionWin);
#elif defined BAN_WHITE_REPETITION
				return (GetTurn() == White ? RepetitionLose : RepetitionWin);
#else
				return RepetitionDraw;
#endif
			}
			else if (stp->m_boardKey == m_st_->m_boardKey) {
				if (m_st_->m_hand.IsEqualOrSuperior(stp->m_hand)) { return RepetitionSuperior; }
				if (stp->m_hand.IsEqualOrSuperior(m_st_->m_hand)) { return RepetitionInferior; }
			}
			i += 2;
		} while (i <= e);
	}
	return NotRepetition;
}

namespace {
	void printHandPiece(const Position& pos, const HandPiece hp, const Color c, const std::string& str) {
		if (pos.GetHand(c).NumOf(hp)) {
			const char* sign = (c == Black ? "+" : "-");
			std::cout << "P" << sign;
			for (u32 i = 0; i < pos.GetHand(c).NumOf(hp); ++i) {
				std::cout << "00" << str;
			}
			std::cout << std::endl;
		}
	}
}
void Position::PrintHand(const Color c) const {
	printHandPiece(*this, HPawn  , c, "FU");
	printHandPiece(*this, HLance , c, "KY");
	printHandPiece(*this, HKnight, c, "KE");
	printHandPiece(*this, HSilver, c, "GI");
	printHandPiece(*this, HGold  , c, "KI");
	printHandPiece(*this, HBishop, c, "KA");
	printHandPiece(*this, HRook  , c, "HI");
}

Position& Position::operator = (const Position& pos) {
	memcpy(this, &pos, sizeof(Position));
	m_startState_ = *m_st_;
	m_st_ = &m_startState_;
	m_nodes_ = 0;

	assert(IsOK());

	return *this;
}

void Position::Set(const std::string& sfen, Thread* th) {
	Piece promoteFlag = UnPromoted;
	std::istringstream ss(sfen);
	char token;
	Square sq = A9;

	Searcher* s = std::move(m_searcher_);
	Clear();
	SetSearcher(s);

	// 盤上の駒
	while (ss.get(token) && token != ' ') {
		if (isdigit(token)) {
			sq += DeltaE * (token - '0');
		}
		else if (token == '/') {
			sq += (DeltaW * 9) + DeltaS;
		}
		else if (token == '+') {
			promoteFlag = Promoted;
		}
		else if (g_charToPieceUSI.isLegalChar(token)) {
			if (UtilSquare::ContainsOf(sq)) {
				SetPiece(g_charToPieceUSI.value(token) + promoteFlag, sq);
				promoteFlag = UnPromoted;
				sq += DeltaE;
			}
			else {
				goto INCORRECT;
			}
		}
		else {
			goto INCORRECT;
		}
	}
	m_kingSquare_[Black] = GetBbOf(King, Black).ConstFirstOneFromI9();
	m_kingSquare_[White] = GetBbOf(King, White).ConstFirstOneFromI9();
	m_goldsBB_ = GetBbOf(Gold, ProPawn, ProLance, ProKnight, ProSilver);

	// 手番
	while (ss.get(token) && token != ' ') {
		if (token == 'b') {
			m_turn_ = Black;
		}
		else if (token == 'w') {
			m_turn_ = White;
		}
		else {
			goto INCORRECT;
		}
	}

	// 持ち駒
	for (int digits = 0; ss.get(token) && token != ' '; ) {
		if (token == '-') {
			memset(m_hand_, 0, sizeof(m_hand_));
		}
		else if (isdigit(token)) {
			digits = digits * 10 + token - '0';
		}
		else if (g_charToPieceUSI.isLegalChar(token)) {
			// 持ち駒を32bit に pack する
			const Piece piece = g_charToPieceUSI.value(token);
			SetHand(piece, (digits == 0 ? 1 : digits));

			digits = 0;
		}
		else {
			goto INCORRECT;
		}
	}

	// 次の手が何手目か
	ss >> m_gamePly_;
	m_gamePly_ = std::max(2 * (m_gamePly_ - 1), 0) + static_cast<int>(GetTurn() == White);

	// 残り時間, hash key, (もし実装するなら)駒番号などをここで設定
	m_st_->m_boardKey = GetComputeBoardKey();
	m_st_->m_handKey = GetComputeHandKey();
	m_st_->m_hand = GetHand(GetTurn());

	SetEvalList();
	FindCheckers();
	m_st_->m_material = ComputeMaterial();
	m_thisThread_ = th;

	return;
INCORRECT:
	std::cout << "incorrect SFEN string : " << sfen << std::endl;
}

bool Position::IsMoveGivesCheck(const Move move) const {
	return IsMoveGivesCheck(move, CheckInfo(*this));
}

// move が王手なら true
bool Position::IsMoveGivesCheck(const Move move, const CheckInfo& ci) const {
	assert(IsOK());
	assert(ci.m_dcBB == DiscoveredCheckBB());

	const Square to = move.To();
	if (move.IsDrop()) {
		const PieceType ptTo = move.GetPieceTypeDropped();
		// Direct Check ?
		if (UtilBitboard::IsSet(&ci.m_checkBB[ptTo], to)) {
			return true;
		}
	}
	else {
		const Square from = move.From();
		const PieceType ptFrom = move.GetPieceTypeFrom();
		const PieceType ptTo = move.GetPieceTypeTo(ptFrom);
		assert(ptFrom == UtilPiece::ToPieceType(GetPiece(from)));
		// Direct Check ?
		if (UtilBitboard::IsSet(&ci.m_checkBB[ptTo], to)) {
			return true;
		}

		// Discovery Check ?
		if (IsDiscoveredCheck(from, to, GetKingSquare(UtilColor::OppositeColor(GetTurn())), ci.m_dcBB)) {
			return true;
		}
	}

	return false;
}

void Position::Clear() {
	memset(this, 0, sizeof(Position));
	m_st_ = &m_startState_;
}

// 先手、後手に関わらず、sq へ移動可能な Bitboard を返す。
Bitboard Position::GetAttackersTo(const Square sq, const Bitboard& occupied) const {
	const Bitboard golds = GetGoldsBB();
	return (((GetAttacksFrom<Pawn  >(Black, sq          ) & this->GetBbOf(Pawn  ))
			 | (GetAttacksFrom<Lance >(Black, sq, occupied) & this->GetBbOf(Lance ))
			 | (GetAttacksFrom<Knight>(Black, sq          ) & this->GetBbOf(Knight))
			 | (GetAttacksFrom<Silver>(Black, sq          ) & this->GetBbOf(Silver))
			 | (GetAttacksFrom<Gold  >(Black, sq          ) & golds       ))
			& this->GetBbOf(White))
		| (((GetAttacksFrom<Pawn  >(White, sq          ) & this->GetBbOf(Pawn  ))
			| (GetAttacksFrom<Lance >(White, sq, occupied) & this->GetBbOf(Lance ))
			| (GetAttacksFrom<Knight>(White, sq          ) & this->GetBbOf(Knight))
			| (GetAttacksFrom<Silver>(White, sq          ) & this->GetBbOf(Silver))
			| (GetAttacksFrom<Gold  >(White, sq          ) & golds))
		   & this->GetBbOf(Black))
		| (GetAttacksFrom<Bishop>(sq, occupied) & this->GetBbOf(Bishop, Horse        ))
		| (GetAttacksFrom<Rook  >(sq, occupied) & this->GetBbOf(Rook  , Dragon       ))
		| (GetAttacksFrom<King  >(sq          ) & this->GetBbOf(King  , Horse, Dragon));
}

// occupied を Position::occupiedBB() 以外のものを使用する場合に使用する。
Bitboard Position::GetAttackersTo(const Color c, const Square sq, const Bitboard& occupied) const {
	const Color opposite = UtilColor::OppositeColor(c);
	return ((GetAttacksFrom<Pawn  >(opposite, sq          ) & this->GetBbOf(Pawn  ))
			| (GetAttacksFrom<Lance >(opposite, sq, occupied) & this->GetBbOf(Lance ))
			| (GetAttacksFrom<Knight>(opposite, sq          ) & this->GetBbOf(Knight))
			| (GetAttacksFrom<Silver>(opposite, sq          ) & this->GetBbOf(Silver))
			| (GetAttacksFrom<Gold  >(opposite, sq          ) & GetGoldsBB())
			| (GetAttacksFrom<Bishop>(          sq, occupied) & this->GetBbOf(Bishop, Horse        ))
			| (GetAttacksFrom<Rook  >(          sq, occupied) & this->GetBbOf(Rook  , Dragon       ))
			| (GetAttacksFrom<King  >(          sq          ) & this->GetBbOf(King  , Horse, Dragon)))
		& this->GetBbOf(c);
}

// 玉以外で sq へ移動可能な c 側の駒の Bitboard を返す。
Bitboard Position::GetAttackersToExceptKing(const Color c, const Square sq) const {
	const Color opposite = UtilColor::OppositeColor(c);
	return ((GetAttacksFrom<Pawn  >(opposite, sq) & this->GetBbOf(Pawn  ))
			| (GetAttacksFrom<Lance >(opposite, sq) & this->GetBbOf(Lance ))
			| (GetAttacksFrom<Knight>(opposite, sq) & this->GetBbOf(Knight))
			| (GetAttacksFrom<Silver>(opposite, sq) & this->GetBbOf(Silver, Dragon))
			| (GetAttacksFrom<Gold  >(opposite, sq) & (GetGoldsBB() | this->GetBbOf(Horse)))
			| (GetAttacksFrom<Bishop>(          sq) & this->GetBbOf(Bishop, Horse ))
			| (GetAttacksFrom<Rook  >(          sq) & this->GetBbOf(Rook  , Dragon)))
		& this->GetBbOf(c);
}

Score Position::ComputeMaterial() const {
	Score s = ScoreZero;
	for (PieceType pt = Pawn; pt < PieceTypeNum; ++pt) {
		const int num = this->GetBbOf(pt, Black).PopCount() - this->GetBbOf(pt, White).PopCount();
		s += num * GetPieceScore(pt);
	}
	for (PieceType pt = Pawn; pt < King; ++pt) {
		const int num = GetHand(Black).NumOf(UtilHandPiece::FromPieceType(pt)) - GetHand(White).NumOf(UtilHandPiece::FromPieceType(pt));
		s += num * GetPieceScore(pt);
	}
	return s;
}
