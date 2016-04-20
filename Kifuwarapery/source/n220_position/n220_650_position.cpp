#include <iostream>		// std::cout
#include <algorithm>	// std::min,std::max
#include <sstream>      // std::istringstream
#include "../../header/n105_color___/n105_100_color.hpp"
#include "../../header/n110_square__/n110_100_square.hpp"
#include "../../header/n110_square__/n110_400_squareRelation.hpp"
#include "../../header/n112_pieceTyp/n112_050_pieceType.hpp"
#include "../../header/n113_piece___/n113_150_piece.hpp"
#include "../../header/n113_piece___/n113_155_utilPiece.hpp"
#include "../../header/n160_board___/n160_100_bitboard.hpp"
#include "../../header/n160_board___/n160_170_goldAndSilverAttackBb.hpp"
#include "../../header/n160_board___/n160_230_setMaskBb.hpp"
#include "../../header/n160_board___/n160_240_betweenBb.hpp"
#include "../../header/n165_movStack/n165_300_moveType.hpp"
#include "../../header/n220_position/n220_100_repetitionType.hpp"
#include "../../header/n220_position/n220_640_utilAttack.hpp"
#include "../../header/n220_position/n220_650_position.hpp"

#include "../../header/n350_pieceTyp/n350_110_ptPawn.hpp"
#include "../../header/n350_pieceTyp/n350_120_ptLance.hpp"
#include "../../header/n350_pieceTyp/n350_130_ptKnight.hpp"
#include "../../header/n350_pieceTyp/n350_140_ptSilver.hpp"
#include "../../header/n350_pieceTyp/n350_150_ptBishop.hpp"
#include "../../header/n350_pieceTyp/n350_160_ptRook.hpp"
#include "../../header/n350_pieceTyp/n350_170_ptGold.hpp"
#include "../../header/n350_pieceTyp/n350_230_ptHorse.hpp"
#include "../../header/n350_pieceTyp/n350_240_ptDragon.hpp"
#include "../../header/n351_bonaDir_/n351_500_bonaDirArray.hpp"
#include "../../header/n407_moveGen_/n407_900_moveList.hpp"

#include "../../header/n520_evaluate/n520_500_kkKkpKppStorage1.hpp"
#include "../../header/n600_book____/n600_100_mt64bit.hpp"
#include "../../header/n885_searcher/n885_500_searcher.hpp"



Key Position::m_zobrist_[N15_PieceTypeNum][SquareNum][ColorNum];
Key Position::m_zobHand_[HandPieceNum][ColorNum];
Key Position::m_zobExclusion_;



Color Position::GetTurn() const
{
	return m_turn_;
}




// 実際に指し手が合法手かどうか判定
// 連続王手の千日手は排除しない。
// 確実に駒打ちではないときは、MUSTNOTDROP == true とする。
// 確実に玉の移動で無いときは、FROMMUSTNOTKING == true とする。英語として正しい？
// 遠隔駒で王手されているとき、その駒の利きがある場所に逃げる手を検出出来ない場合があるので、
// そのような手を指し手生成してはいけない。
template <bool MUSTNOTDROP, bool FROMMUSTNOTKING>
bool Position::IsPseudoLegalMoveIsLegal(const Move move, const Bitboard& pinned) const {
	// 駒打ちは、打ち歩詰めや二歩は指し手生成時や、killerを ＭｏｖｅＰｉｃｋｅｒ::nextMove() 内で排除しているので、常に合法手
	// (連続王手の千日手は省いていないけれど。)
	if (!MUSTNOTDROP && move.IsDrop()) {
		return true;
	}
	assert(!move.IsDrop());

	const Color us = GetTurn();
	const Square from = move.From();

	if (!FROMMUSTNOTKING && UtilPiece::ToPieceType(GetPiece(from)) == N08_King) {
		const Color them = UtilColor::OppositeColor(us);
		// 玉の移動先に相手の駒の利きがあれば、合法手でないので、false
		return !IsAttackersToIsNot0(them, move.To());
	}
	// 玉以外の駒の移動
	return !IsPinnedIllegal(from, move.To(), GetKingSquare(us), pinned);
}

template bool Position::IsPseudoLegalMoveIsLegal<false, false>(const Move move, const Bitboard& pinned) const;
template bool Position::IsPseudoLegalMoveIsLegal<false, true >(const Move move, const Bitboard& pinned) const;
template bool Position::IsPseudoLegalMoveIsLegal<true, false>(const Move move, const Bitboard& pinned) const;

bool Position::IsPseudoLegalMoveIsEvasion(const Move move, const Bitboard& pinned) const {
	assert(IsOK());

	// 玉の移動
	if (move.GetPieceTypeFrom() == N08_King) {
		// 遠隔駒で王手されたとき、王手している遠隔駒の利きには移動しないように指し手を生成している。
		// その為、移動先に他の駒の利きが無いか調べるだけで良い。
		const bool canMove = !IsAttackersToIsNot0(UtilColor::OppositeColor(GetTurn()), move.To());
		assert(canMove == (IsPseudoLegalMoveIsLegal<false, false>(move, pinned)));
		return canMove;
	}

	// 玉の移動以外
	Bitboard target = GetCheckersBB();
	const Square checkSq = target.PopFirstOneFromI9();

	if (target.Exists1Bit()) {
		// 両王手のとき、玉の移動以外の手は指せない。
		return false;
	}

	const Color us = GetTurn();
	const Square to = move.To();
	// 移動、又は打った駒が、王手をさえぎるか、王手している駒を取る必要がある。
	target = g_betweenBb.GetBetweenBB(checkSq, GetKingSquare(us)) | GetCheckersBB();
	return g_setMaskBb.IsSet(&target, to) && IsPseudoLegalMoveIsLegal<false, true>(move, pinned);
}






// checkPawnDrop : 二歩と打ち歩詰めも調べるなら true
//                 これが true のとき、駒打ちの場合のみ Legal であることが確定する。
bool Position::MoveIsPseudoLegal(const Move move, const bool checkPawnDrop) const {
	const Color us = GetTurn();
	const Color them = UtilColor::OppositeColor(us);
	const Square to = move.To();

	if (move.IsDrop()) {
		const PieceType ptFrom = move.GetPieceTypeDropped();
		if (!GetHand(us).Exists(UtilHandPiece::FromPieceType(ptFrom)) || GetPiece(to) != N00_Empty) {
			return false;
		}

		if (InCheck()) {
			// 王手されているので、合駒でなければならない。
			Bitboard target = GetCheckersBB();
			const Square checksq = target.PopFirstOneFromI9();

			if (target.Exists1Bit()) {
				// 両王手は合駒出来無い。
				return false;
			}

			target = g_betweenBb.GetBetweenBB(checksq, GetKingSquare(us));
			if (!g_setMaskBb.IsSet(&target, to)) {
				// 玉と、王手した駒との間に駒を打っていない。
				return false;
			}
		}

		if (ptFrom == N01_Pawn && checkPawnDrop) {
			if ((this->GetBbOf(N01_Pawn, us) & g_fileMaskBb.GetFileMask(UtilSquare::ToFile(to))).Exists1Bit()) {
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
		if (GetPiece(from) != UtilPiece::FromColorAndPieceType(us, ptFrom) || g_setMaskBb.IsSet(&this->GetBbOf(us), to)) {
			return false;
		}

		if (!g_setMaskBb.IsSet( &UtilAttack::GetAttacksFrom(ptFrom, us, from, this->GetOccupiedBB()), to)) {
			return false;
		}

		if (InCheck()) {
			if (ptFrom == N08_King) {
				Bitboard occ = GetOccupiedBB();
				g_setMaskBb.ClearBit(&occ, from);
				if (IsAttackersToIsNot0(them, to, occ)) {
					// 王手から逃げていない。
					return false;
				}
			}
			else {
				// 玉以外の駒を移動させたとき。
				Bitboard target = GetCheckersBB();
				const Square checksq = target.PopFirstOneFromI9();

				if (target.Exists1Bit()) {
					// 両王手なので、玉が逃げない手は駄目
					return false;
				}

				target = g_betweenBb.GetBetweenBB(checksq, GetKingSquare(us)) | GetCheckersBB();
				if (!g_setMaskBb.IsSet(&target, to)) {
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
	return MoveList<N09_LegalAll>(*this).Contains(GetMove);
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

		m_evalList_.m_list0[listIndex] = kppArray[pcTo] + to;
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
			m_st_->m_checkersBB = g_setMaskBb.GetSetMaskBb(to);
			m_st_->m_continuousCheck[us] += 2;
		}
		else {
			m_st_->m_checkersBB = Bitboard::CreateAllZeroBB();
			m_st_->m_continuousCheck[us] = 0;
		}
	}
	else {
		const Square from = move.From();
		const PieceType ptFrom = move.GetPieceTypeFrom();
		ptTo = move.GetPieceTypeTo(ptFrom);

		g_setMaskBb.XorBit(&m_byTypeBB_[ptFrom], from);
		g_setMaskBb.XorBit(&m_byTypeBB_[ptTo], to);
		g_setMaskBb.XorBit(&m_byColorBB_[us], from, to);
		m_piece_[from] = N00_Empty;
		m_piece_[to] = UtilPiece::FromColorAndPieceType(us, ptTo);
		boardKey -= GetZobrist(ptFrom, from, us);
		boardKey += GetZobrist(ptTo, to, us);

		if (ptCaptured) {
			// 駒を取ったとき
			const HandPiece hpCaptured = UtilHandPiece::FromPieceType(ptCaptured);
			const Color them = UtilColor::OppositeColor(us);

			boardKey -= GetZobrist(ptCaptured, to, them);
			handKey += GetZobHand(hpCaptured, us);

			g_setMaskBb.XorBit(&m_byTypeBB_[ptCaptured], to);
			g_setMaskBb.XorBit(&m_byColorBB_[them], to);

			m_hand_[us].PlusOne(hpCaptured);
			const int toListIndex = m_evalList_.m_squareHandToList[to];
			m_st_->m_cl.m_listindex[1] = toListIndex;
			m_st_->m_cl.m_clistpair[1].m_oldlist[0] = m_evalList_.m_list0[toListIndex];
			m_st_->m_cl.m_clistpair[1].m_oldlist[1] = m_evalList_.m_list1[toListIndex];
			m_st_->m_cl.m_size = 2;

			const int handnum = GetHand(us).NumOf(hpCaptured);
			m_evalList_.m_list0[toListIndex] = kppHandArray[us][hpCaptured] + handnum;
			m_evalList_.m_list1[toListIndex] = kppHandArray[them][hpCaptured] + handnum;
			const Square squarehand = g_HandPieceToSquareHand[us][hpCaptured] + handnum;
			m_evalList_.m_listToSquareHand[toListIndex] = squarehand;
			m_evalList_.m_squareHandToList[squarehand] = toListIndex;

			m_st_->m_cl.m_clistpair[1].m_newlist[0] = m_evalList_.m_list0[toListIndex];
			m_st_->m_cl.m_clistpair[1].m_newlist[1] = m_evalList_.m_list1[toListIndex];

			m_st_->m_material += (us == Black ? GetCapturePieceScore(ptCaptured) : -GetCapturePieceScore(ptCaptured));
		}
		prefetch(GetCsearcher()->m_tt.FirstEntry(boardKey + handKey));
		// Occupied は to, from の位置のビットを操作するよりも、
		// Black と White の or を取る方が速いはず。
		m_byTypeBB_[N00_Occupied] = this->GetBbOf(Black) | GetBbOf(White);

		if (ptTo == N08_King) {
			m_kingSquare_[us] = to;
		}
		else {
			const Piece pcTo = UtilPiece::FromColorAndPieceType(us, ptTo);
			const int fromListIndex = m_evalList_.m_squareHandToList[from];

			m_st_->m_cl.m_listindex[0] = fromListIndex;
			m_st_->m_cl.m_clistpair[0].m_oldlist[0] = m_evalList_.m_list0[fromListIndex];
			m_st_->m_cl.m_clistpair[0].m_oldlist[1] = m_evalList_.m_list1[fromListIndex];

			m_evalList_.m_list0[fromListIndex] = kppArray[pcTo] + to;
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
			m_st_->m_checkersBB = ci.m_checkBB[ptTo] & g_setMaskBb.GetSetMaskBb(to);

			// Discovery checks
			const Square ksq = GetKingSquare(UtilColor::OppositeColor(us));
			if (IsDiscoveredCheck(from, to, ksq, ci.m_dcBB)) {
				g_bonaDirArray[g_squareRelation.GetSquareRelation(from, ksq)]->Do2Move(*this, from, ksq, us);
			}
			m_st_->m_continuousCheck[us] += 2;
		}
		else {
			m_st_->m_checkersBB = Bitboard::CreateAllZeroBB();
			m_st_->m_continuousCheck[us] = 0;
		}
	}
	m_goldsBB_ = this->GetBbOf(N07_Gold, N09_ProPawn, N10_ProLance, N11_ProKnight, N12_ProSilver);

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
		g_setMaskBb.XorBit(&m_byTypeBB_[ptTo], to);
		g_setMaskBb.XorBit(&m_byColorBB_[us], to);
		m_piece_[to] = N00_Empty;

		const HandPiece hp = UtilHandPiece::FromPieceType(ptTo);
		m_hand_[us].PlusOne(hp);

		const int toListIndex = m_evalList_.m_squareHandToList[to];
		const int handnum = GetHand(us).NumOf(hp);
		m_evalList_.m_list0[toListIndex] = kppHandArray[us][hp] + handnum;
		m_evalList_.m_list1[toListIndex] = kppHandArray[them][hp] + handnum;
		const Square squarehand = g_HandPieceToSquareHand[us][hp] + handnum;
		m_evalList_.m_listToSquareHand[toListIndex] = squarehand;
		m_evalList_.m_squareHandToList[squarehand] = toListIndex;
	}
	else {
		const Square from = move.From();
		const PieceType ptFrom = move.GetPieceTypeFrom();
		const PieceType ptTo = move.GetPieceTypeTo(ptFrom);
		const PieceType ptCaptured = move.GetCap(); // todo: st_->capturedType 使えば良い。

		if (ptTo == N08_King) {
			m_kingSquare_[us] = from;
		}
		else {
			const Piece pcFrom = UtilPiece::FromColorAndPieceType(us, ptFrom);
			const int toListIndex = m_evalList_.m_squareHandToList[to];
			m_evalList_.m_list0[toListIndex] = kppArray[pcFrom] + from;
			m_evalList_.m_list1[toListIndex] = kppArray[UtilPiece::Inverse(pcFrom)] + UtilSquare::Inverse(from);
			m_evalList_.m_listToSquareHand[toListIndex] = from;
			m_evalList_.m_squareHandToList[from] = toListIndex;
		}

		if (ptCaptured) {
			// 駒を取ったとき
			g_setMaskBb.XorBit(&m_byTypeBB_[ptCaptured], to);
			g_setMaskBb.XorBit(&m_byColorBB_[them], to);
			const HandPiece hpCaptured = UtilHandPiece::FromPieceType(ptCaptured);
			const Piece pcCaptured = UtilPiece::FromColorAndPieceType(them, ptCaptured);
			m_piece_[to] = pcCaptured;

			const int handnum = GetHand(us).NumOf(hpCaptured);
			const int toListIndex = m_evalList_.m_squareHandToList[g_HandPieceToSquareHand[us][hpCaptured] + handnum];
			m_evalList_.m_list0[toListIndex] = kppArray[pcCaptured] + to;
			m_evalList_.m_list1[toListIndex] = kppArray[UtilPiece::Inverse(pcCaptured)] + UtilSquare::Inverse(to);
			m_evalList_.m_listToSquareHand[toListIndex] = to;
			m_evalList_.m_squareHandToList[to] = toListIndex;

			m_hand_[us].MinusOne(hpCaptured);
		}
		else {
			// 駒を取らないときは、UtilPiece::colorAndPieceTypeToPiece(us, ptCaptured) は 0 または 16 になる。
			// 16 になると困るので、駒を取らないときは明示的に Empty にする。
			m_piece_[to] = N00_Empty;
		}
		g_setMaskBb.XorBit(&m_byTypeBB_[ptFrom], from);
		g_setMaskBb.XorBit(&m_byTypeBB_[ptTo], to);
		g_setMaskBb.XorBit(&m_byColorBB_[us], from, to);
		m_piece_[from] = UtilPiece::FromColorAndPieceType(us, ptFrom);
	}
	// Occupied は to, from の位置のビットを操作するよりも、
	// Black と White の or を取る方が速いはず。
	m_byTypeBB_[N00_Occupied] = this->GetBbOf(Black) | GetBbOf(White);
	m_goldsBB_ = this->GetBbOf(N07_Gold, N09_ProPawn, N10_ProLance, N11_ProKnight, N12_ProSilver);

	// key などは StateInfo にまとめられているので、
	// previous のポインタを st_ に代入するだけで良い。
	m_st_ = m_st_->m_previous;

	assert(IsOK());
}




// GetSee()用
namespace {
	// SEE の順番
	template <PieceType PT> struct SEENextPieceType {}; // これはインスタンス化しない。
	template <> struct SEENextPieceType<N01_Pawn     > { static const PieceType value = N02_Lance; };
	template <> struct SEENextPieceType<N02_Lance    > { static const PieceType value = N03_Knight; };
	template <> struct SEENextPieceType<N03_Knight   > { static const PieceType value = N09_ProPawn; };
	template <> struct SEENextPieceType<N09_ProPawn  > { static const PieceType value = N10_ProLance; };
	template <> struct SEENextPieceType<N10_ProLance > { static const PieceType value = N11_ProKnight; };
	template <> struct SEENextPieceType<N11_ProKnight> { static const PieceType value = N04_Silver; };
	template <> struct SEENextPieceType<N04_Silver   > { static const PieceType value = N12_ProSilver; };
	template <> struct SEENextPieceType<N12_ProSilver> { static const PieceType value = N07_Gold; };
	template <> struct SEENextPieceType<N07_Gold     > { static const PieceType value = N05_Bishop; };
	template <> struct SEENextPieceType<N05_Bishop   > { static const PieceType value = N13_Horse; };
	template <> struct SEENextPieceType<N13_Horse    > { static const PieceType value = N06_Rook; };
	template <> struct SEENextPieceType<N06_Rook     > { static const PieceType value = N14_Dragon; };
	template <> struct SEENextPieceType<N14_Dragon   > { static const PieceType value = N08_King; };

	template <PieceType PT> FORCE_INLINE PieceType nextAttacker(const Position& pos, const Square to, const Bitboard& opponentAttackers,
		Bitboard& occupied, Bitboard& attackers, const Color turn)
	{
		if (opponentAttackers.AndIsNot0(pos.GetBbOf(PT))) {
			const Bitboard bb = opponentAttackers & pos.GetBbOf(PT);
			const Square from = bb.GetFirstOneFromI9();
			g_setMaskBb.XorBit(&occupied, from);
			// todo: 実際に移動した方向を基にattackersを更新すれば、template, inline を使用しなくても良さそう。
			//       その場合、キャッシュに乗りやすくなるので逆に速くなるかも。
			if (PT == N01_Pawn || PT == N02_Lance) {
				attackers |= (g_lanceAttackBb.GetControllBb(&occupied, UtilColor::OppositeColor(turn), to) & (pos.GetBbOf(N06_Rook, N14_Dragon) | pos.GetBbOf(N02_Lance, turn)));
			}
			if (PT == N07_Gold || PT == N09_ProPawn || PT == N10_ProLance || PT == N11_ProKnight || PT == N12_ProSilver || PT == N13_Horse || PT == N14_Dragon) {
				attackers |= (g_lanceAttackBb.GetControllBb(&occupied, UtilColor::OppositeColor(turn), to) & pos.GetBbOf(N02_Lance, turn))
					| (g_lanceAttackBb.GetControllBb(&occupied, turn, to) & pos.GetBbOf(N02_Lance, UtilColor::OppositeColor(turn)))
					| (g_rookAttackBb.GetControllBb(&occupied, to) & pos.GetBbOf(N06_Rook, N14_Dragon))
					| (g_bishopAttackBb.BishopAttack(&occupied, to) & pos.GetBbOf(N05_Bishop, N13_Horse));
			}
			if (PT == N04_Silver) {
				attackers |= (g_lanceAttackBb.GetControllBb(&occupied, UtilColor::OppositeColor(turn), to) & pos.GetBbOf(N02_Lance, turn))
					| (g_rookAttackBb.GetControllBb(&occupied, to) & pos.GetBbOf(N06_Rook, N14_Dragon))
					| (g_bishopAttackBb.BishopAttack(&occupied, to) & pos.GetBbOf(N05_Bishop, N13_Horse));
			}
			if (PT == N05_Bishop) {
				attackers |= (g_bishopAttackBb.BishopAttack(&occupied, to) & pos.GetBbOf(N05_Bishop, N13_Horse));
			}
			if (PT == N06_Rook) {
				attackers |= (g_lanceAttackBb.GetControllBb(&occupied, UtilColor::OppositeColor(turn), to) & pos.GetBbOf(N02_Lance, turn))
					| (g_lanceAttackBb.GetControllBb(&occupied, turn, to) & pos.GetBbOf(N02_Lance, UtilColor::OppositeColor(turn)))
					| (g_rookAttackBb.GetControllBb(&occupied, to) & pos.GetBbOf(N06_Rook, N14_Dragon));
			}

			if (PT == N01_Pawn || PT == N02_Lance || PT == N03_Knight) {
				if (UtilSquare::CanPromote(turn, UtilSquare::ToRank(to))) {
					return PT + PTPromote;
				}
			}
			if (PT == N04_Silver || PT == N05_Bishop || PT == N06_Rook) {
				if (UtilSquare::CanPromote(turn, UtilSquare::ToRank(to)) || UtilSquare::CanPromote(turn, UtilSquare::ToRank(from))) {
					return PT + PTPromote;
				}
			}
			return PT;
		}
		return nextAttacker<SEENextPieceType<PT>::value>(pos, to, opponentAttackers, occupied, attackers, turn);
	}
	template <> FORCE_INLINE PieceType nextAttacker<N08_King>(const Position&, const Square, const Bitboard&,
		Bitboard&, Bitboard&, const Color)
	{
		return N08_King;
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
		opponentAttackers = this->GetAttackersTo(turn, to, occ);
		if (!opponentAttackers.Exists1Bit()) {
			return ScoreZero;
		}
		attackers = opponentAttackers | this->GetAttackersTo(UtilColor::OppositeColor(turn), to, occ);
		swapList[0] = ScoreZero;
		ptCaptured = move.GetPieceTypeDropped();
	}
	else {
		from = move.From();
		g_setMaskBb.XorBit(&occ, from);
		opponentAttackers = this->GetAttackersTo(turn, to, occ);
		if (!opponentAttackers.Exists1Bit()) {
			if (move.IsPromotion()) {
				const PieceType ptFrom = move.GetPieceTypeFrom();
				return GetCapturePieceScore(move.GetCap()) + GetPromotePieceScore(ptFrom);
			}
			return GetCapturePieceScore(move.GetCap());
		}
		attackers = opponentAttackers | this->GetAttackersTo(UtilColor::OppositeColor(turn), to, occ);
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

		ptCaptured = nextAttacker<N01_Pawn>(*this, to, opponentAttackers, occ, attackers, turn);

		attackers &= occ;
		++slIndex;
		turn = UtilColor::OppositeColor(turn);
		opponentAttackers = attackers & GetBbOf(turn);

		if (ptCaptured == N08_King) {
			if (opponentAttackers.Exists1Bit()) {
				swapList[slIndex++] = g_CaptureKingScore;
			}
			break;
		}
	} while (opponentAttackers.Exists1Bit());

	if (asymmThreshold) {
		for (int i = 0; i < slIndex; i += 2) {
			if (swapList[i] < asymmThreshold) {
				swapList[i] = -g_CaptureKingScore;
			}
		}
	}

	// nega max 的に駒の取り合いの点数を求める。
	while (--slIndex) {
		swapList[slIndex - 1] = std::min(-swapList[slIndex], swapList[slIndex - 1]);
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
		Bitboard kingMoveBB = bb.NotThisAnd(pos.GetBbOf(them).NotThisAnd(g_kingAttackBb.GetControllBb(ksq)));
		g_setMaskBb.ClearBit(&kingMoveBB, sq); // sq には行けないので、クリアする。xorBit(sq)ではダメ。

		if (kingMoveBB.Exists1Bit()) {
			Bitboard tempOccupied = pos.GetOccupiedBB();
			g_setMaskBb.SetBit(&tempOccupied, sq);
			g_setMaskBb.ClearBit(&tempOccupied, ksq);
			do {
				const Square to = kingMoveBB.PopFirstOneFromI9();
				// 玉の移動先に、us 側の利きが無ければ、true
				if (!pos.IsAttackersToIsNot0(us, to, tempOccupied)) {
					return true;
				}
			} while (kingMoveBB.Exists1Bit());
		}
		// 玉の移動先が無い。
		return false;
	}
	// them(相手) 側の玉以外の駒が sq にある us 側の駒を取れるか。
	bool canPieceCapture(const Position& pos, const Color them, const Square sq, const Bitboard& dcBB) {
		// 玉以外で打った駒を取れる相手側の駒の Bitboard
		Bitboard fromBB = pos.GetAttackersToExceptKing(them, sq);

		if (fromBB.Exists1Bit()) {
			const Square ksq = pos.GetKingSquare(them);
			do {
				const Square from = fromBB.PopFirstOneFromI9();
				if (!pos.IsDiscoveredCheck(from, sq, ksq, dcBB)) {
					// them 側から見て、pin されていない駒で、打たれた駒を取れるので、true
					return true;
				}
			} while (fromBB.Exists1Bit());
		}
		// 玉以外の駒で、打った駒を取れない。
		return false;
	}

	// pos.discoveredCheckBB<false>() を遅延評価するバージョン。
	bool canPieceCapture(const Position& pos, const Color them, const Square sq) {
		Bitboard fromBB = pos.GetAttackersToExceptKing(them, sq);

		if (fromBB.Exists1Bit()) {
			const Square ksq = pos.GetKingSquare(them);
			const Bitboard dcBB = pos.DiscoveredCheckBB<false>();
			do {
				const Square from = fromBB.PopFirstOneFromI9();
				if (!pos.IsDiscoveredCheck(from, sq, ksq, dcBB)) {
					// them 側から見て、pin されていない駒で、打たれた駒を取れるので、true
					return true;
				}
			} while (fromBB.Exists1Bit());
		}
		// 玉以外の駒で、打った駒を取れない。
		return false;
	}
}

bool Position::NoPawns(const Color us, const File toFile) const
{
	return !this->GetBbOf(N01_Pawn, us).AndIsNot0(g_fileMaskBb.GetFileMask(toFile));
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
	const Bitboard tempOccupied = GetOccupiedBB() | g_setMaskBb.GetSetMaskBb(sq);
	Bitboard kingMoveBB = GetBbOf(them).NotThisAnd(g_kingAttackBb.GetControllBb(ksq));

	// 少なくとも歩を取る方向には玉が動けるはずなので、do while を使用。
	assert(kingMoveBB.Exists1Bit());
	do {
		const Square to = kingMoveBB.PopFirstOneFromI9();
		if (!IsAttackersToIsNot0(us, to, tempOccupied)) {
			// 相手玉の移動先に自駒の利きがないなら、打ち歩詰めではない。
			return false;
		}
	} while (kingMoveBB.Exists1Bit());

	return true;
}

inline void Position::XorBBs(const PieceType pt, const Square sq, const Color c) {
	g_setMaskBb.XorBit(&m_byTypeBB_[N00_Occupied], sq);
	g_setMaskBb.XorBit(&m_byTypeBB_[pt], sq);
	g_setMaskBb.XorBit(&m_byColorBB_[c], sq);
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
	if (Hand::Exists_HRook(ourHand)) {
		// 合駒されるとややこしいので、3手詰み関数の中で調べる。
		// ここでは離れた位置から王手するのは考えない。
		Bitboard toBB = dropTarget & g_rookAttackBb.RookStepAttacks(ksq);
		while (toBB.Exists1Bit()) {
			const Square to = toBB.PopFirstOneFromI9();
			// 駒を打った場所に自駒の利きがあるか。(無ければ玉で取られて詰まない)
			if (IsAttackersToIsNot0(US, to)) {
				// 玉が逃げられず、他の駒で取ることも出来ないか
				if (!canKingEscape(*this, US, to, g_rookAttackBb.GetControllBbToEdge(to))
					&& !canPieceCapture(*this, Them, to, dcBB_betweenIsThem))
				{
					return UtilMove::MakeDropMove(N06_Rook, to);
				}
			}
		}
	}
	// 香車打ち
	// 飛車で詰まなければ香車でも詰まないので、else if を使用。
	// 玉が 9(1) 段目にいれば香車で王手出来無いので、それも省く。
	else if (
		Hand::Exists_HLance(ourHand) &&
		UtilSquare::IsInFrontOf<US, Rank1, Rank9>(UtilSquare::ToRank(ksq))
	) {
		const Square to = ksq + TDeltaS;
		if (GetPiece(to) == N00_Empty && IsAttackersToIsNot0(US, to)) {
			if (!canKingEscape(*this, US, to, g_lanceAttackBb.GetControllBbToEdge(US, to))
				&& !canPieceCapture(*this, Them, to, dcBB_betweenIsThem))
			{
				return UtilMove::MakeDropMove(N02_Lance, to);
			}
		}
	}

	// 角打ち
	if (Hand::Exists_HBishop(ourHand)) {
		Bitboard toBB = dropTarget & g_bishopAttackBb.BishopStepAttacks(ksq);
		while (toBB.Exists1Bit()) {
			const Square to = toBB.PopFirstOneFromI9();
			if (IsAttackersToIsNot0(US, to)) {
				if (!canKingEscape(*this, US, to, g_bishopAttackBb.GetControllBbToEdge(to))
					&& !canPieceCapture(*this, Them, to, dcBB_betweenIsThem))
				{
					return UtilMove::MakeDropMove(N05_Bishop, to);
				}
			}
		}
	}

	// 金打ち
	if (Hand::Exists_HGold(ourHand)) {
		Bitboard toBB;
		if (Hand::Exists_HRook(ourHand)) {
			// 飛車打ちを先に調べたので、尻金だけは省く。
			toBB = dropTarget & (g_goldAttackBb.GetControllBb(Them, ksq) ^ g_pawnAttackBb.GetControllBb(US, ksq));
		}
		else {
			toBB = dropTarget & g_goldAttackBb.GetControllBb(Them, ksq);
		}
		while (toBB.Exists1Bit()) {
			const Square to = toBB.PopFirstOneFromI9();
			if (IsAttackersToIsNot0(US, to)) {
				if (!canKingEscape(*this, US, to, g_goldAttackBb.GetControllBb(US, to))
					&& !canPieceCapture(*this, Them, to, dcBB_betweenIsThem))
				{
					return UtilMove::MakeDropMove(N07_Gold, to);
				}
			}
		}
	}

	if (Hand::Exists_HSilver(ourHand)) {
		Bitboard toBB;
		if (Hand::Exists_HGold(ourHand)) {
			// 金打ちを先に調べたので、斜め後ろから打つ場合だけを調べる。

			if (Hand::Exists_HBishop(ourHand)) {
				// 角打ちを先に調べたので、斜めからの王手も除外できる。銀打ちを調べる必要がない。
				goto silver_drop_end;
			}
			// 斜め後ろから打つ場合を調べる必要がある。
			toBB = dropTarget & (g_silverAttackBb.GetControllBb(Them, ksq) & g_inFrontMaskBb.GetInFrontMask(US, UtilSquare::ToRank(ksq)));
		}
		else {
			if (Hand::Exists_HBishop(ourHand)) {
				// 斜め後ろを除外。前方から打つ場合を調べる必要がある。
				toBB = dropTarget & g_goldAndSilverAttackBb.GoldAndSilverAttacks(Them, ksq);
			}
			else {
				toBB = dropTarget & g_silverAttackBb.GetControllBb(Them, ksq);
			}
		}
		while (toBB.Exists1Bit()) {
			const Square to = toBB.PopFirstOneFromI9();
			if (IsAttackersToIsNot0(US, to)) {
				if (!canKingEscape(*this, US, to, g_silverAttackBb.GetControllBb(US, to))
					&& !canPieceCapture(*this, Them, to, dcBB_betweenIsThem))
				{
					return UtilMove::MakeDropMove(N04_Silver, to);
				}
			}
		}
	}
silver_drop_end:

	if (Hand::Exists_HKnight(ourHand)) {
		Bitboard toBB = dropTarget & g_knightAttackBb.GetControllBb(Them, ksq);
		while (toBB.Exists1Bit()) {
			const Square to = toBB.PopFirstOneFromI9();
			// 桂馬は紐が付いている必要はない。
			// よって、このcanKingEscape() 内での to の位置に逃げられないようにする処理は無駄。
			if (!canKingEscape(*this, US, to, Bitboard::CreateAllZeroBB())
				&& !canPieceCapture(*this, Them, to, dcBB_betweenIsThem))
			{
				return UtilMove::MakeDropMove(N03_Knight, to);
			}
		}
	}

	// 歩打ちで詰ますと反則なので、調べない。

	// 駒を移動する場合
	// moveTarget は桂馬以外の移動先の大まかな位置。飛角香の遠隔王手は含まない。
	const Bitboard moveTarget = GetBbOf(US).NotThisAnd(g_kingAttackBb.GetControllBb(ksq));
	const Bitboard pinned = GetPinnedBB();
	const Bitboard dcBB_betweenIsUs = DiscoveredCheckBB<true>();

	{
		// 竜による移動
		Bitboard fromBB = GetBbOf(N14_Dragon, US);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			// 遠隔王手は考えない。
			Bitboard toBB = moveTarget & PieceTypeArray::m_dragon.GetAttacks2From( this->GetOccupiedBB(), Color::ColorNum, from);
			if (toBB.Exists1Bit()) {
				XorBBs(N14_Dragon, from, US);
				// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
				const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();
				// to の位置の Bitboard は canKingEscape の中で更新する。
				do {
					const Square to = toBB.PopFirstOneFromI9();
					// 王手した駒の場所に自駒の利きがあるか。(無ければ玉で取られて詰まない)
					if (IsUnDropCheckIsSupported(US, to)) {
						// 玉が逃げられない
						// かつ、(空き王手 または 他の駒で取れない)
						// かつ、王手した駒が pin されていない
						if (!canKingEscape(*this, US, to, PieceTypeArray::m_dragon.GetAttacks2From(this->GetOccupiedBB() ^ g_setMaskBb.GetSetMaskBb(ksq), Color::ColorNum, to))
							&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
								|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
							&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
						{
							XorBBs(N14_Dragon, from, US);
							return UtilMovePos::MakeCaptureMove(N14_Dragon, from, to, *this);
						}
					}
				} while (toBB.Exists1Bit());
				XorBBs(N14_Dragon, from, US);
			}
		}
	}

	// Txxx は先手、後手の情報を吸収した変数。数字は先手に合わせている。
	const Rank TRank6 = (US == Black ? Rank6 : Rank4);
	const Bitboard TRank789BB = g_inFrontMaskBb.GetInFrontMask<US, TRank6>();
	{
		// 飛車による移動
		Bitboard fromBB = GetBbOf(N06_Rook, US);
		Bitboard fromOn789BB = fromBB & TRank789BB;
		// from が 123 段目
		if (fromOn789BB.Exists1Bit()) {
			fromBB.AndEqualNot(TRank789BB);
			do {
				const Square from = fromOn789BB.PopFirstOneFromI9();
				Bitboard toBB = moveTarget & PieceTypeArray::m_rook.GetAttacks2From(this->GetOccupiedBB(), Color::ColorNum,from);
				if (toBB.Exists1Bit()) {
					XorBBs(N06_Rook, from, US);
					// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
					const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();
					// to の位置の Bitboard は canKingEscape の中で更新する。
					do {
						const Square to = toBB.PopFirstOneFromI9();
						if (IsUnDropCheckIsSupported(US, to)) {
							if (!canKingEscape(*this, US, to, PieceTypeArray::m_dragon.GetAttacks2From(this->GetOccupiedBB() ^ g_setMaskBb.GetSetMaskBb(ksq), Color::ColorNum, to))
								&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
									|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
								&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
							{
								XorBBs(N06_Rook, from, US);
								return UtilMovePos::MakeCapturePromoteMove(N06_Rook, from, to, *this);
							}
						}
					} while (toBB.Exists1Bit());
					XorBBs(N06_Rook, from, US);
				}
			} while (fromOn789BB.Exists1Bit());
		}

		// from が 4~9 段目
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			Bitboard toBB = moveTarget & PieceTypeArray::m_rook.GetAttacks2From(this->GetOccupiedBB(), Color::ColorNum, from) & (g_rookAttackBb.RookStepAttacks(ksq) | TRank789BB);
			if (toBB.Exists1Bit()) {
				XorBBs(N06_Rook, from, US);
				// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
				const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();

				Bitboard toOn789BB = toBB & TRank789BB;
				// 成り
				if (toOn789BB.Exists1Bit()) {
					do {
						const Square to = toOn789BB.PopFirstOneFromI9();
						if (IsUnDropCheckIsSupported(US, to)) {
							if (!canKingEscape(*this, US, to, PieceTypeArray::m_dragon.GetAttacks2From(this->GetOccupiedBB() ^ g_setMaskBb.GetSetMaskBb(ksq), Color::ColorNum, to))
								&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
									|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
								&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
							{
								XorBBs(N06_Rook, from, US);
								return UtilMovePos::MakeCapturePromoteMove(N06_Rook, from, to, *this);
							}
						}
					} while (toOn789BB.Exists1Bit());

					toBB.AndEqualNot(TRank789BB);
				}
				// 不成
				while (toBB.Exists1Bit()) {
					const Square to = toBB.PopFirstOneFromI9();
					if (IsUnDropCheckIsSupported(US, to)) {
						if (!canKingEscape(*this, US, to, g_rookAttackBb.GetControllBbToEdge(to))
							&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
								|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
							&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
						{
							XorBBs(N06_Rook, from, US);
							return UtilMovePos::MakeCaptureMove(N06_Rook, from, to, *this);
						}
					}
				}
				XorBBs(N06_Rook, from, US);
			}
		}
	}

	{
		// 馬による移動
		Bitboard fromBB = GetBbOf(N13_Horse, US);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			// 遠隔王手は考えない。
			Bitboard toBB = moveTarget & PieceTypeArray::m_horse.GetAttacks2From(this->GetOccupiedBB(), Color::ColorNum, from);
			if (toBB.Exists1Bit()) {
				XorBBs(N13_Horse, from, US);
				// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
				const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();
				// to の位置の Bitboard は canKingEscape の中で更新する。
				do {
					const Square to = toBB.PopFirstOneFromI9();
					// 王手した駒の場所に自駒の利きがあるか。(無ければ玉で取られて詰まない)
					if (IsUnDropCheckIsSupported(US, to)) {
						// 玉が逃げられない
						// かつ、(空き王手 または 他の駒で取れない)
						// かつ、動かした駒が pin されていない)
						if (!canKingEscape(*this, US, to, g_horseAttackBb.GetControllBbToEdge(to)) // 竜の場合と違って、常に最大の利きを使用して良い。
							&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
								|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
							&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
						{
							XorBBs(N13_Horse, from, US);
							return UtilMovePos::MakeCaptureMove(N13_Horse, from, to, *this);
						}
					}
				} while (toBB.Exists1Bit());
				XorBBs(N13_Horse, from, US);
			}
		}
	}

	{
		// 角による移動
		Bitboard fromBB = GetBbOf(N05_Bishop, US);
		Bitboard fromOn789BB = fromBB & TRank789BB;
		// from が 123 段目
		if (fromOn789BB.Exists1Bit()) {
			fromBB.AndEqualNot(TRank789BB);
			do {
				const Square from = fromOn789BB.PopFirstOneFromI9();
				Bitboard toBB = moveTarget & PieceTypeArray::m_bishop.GetAttacks2From(this->GetOccupiedBB(), Color::ColorNum,from);
				if (toBB.Exists1Bit()) {
					XorBBs(N05_Bishop, from, US);
					// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
					const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();
					// to の位置の Bitboard は canKingEscape の中で更新する。
					do {
						const Square to = toBB.PopFirstOneFromI9();
						if (IsUnDropCheckIsSupported(US, to)) {
							if (!canKingEscape(*this, US, to, g_horseAttackBb.GetControllBbToEdge(to))
								&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
									|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
								&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
							{
								XorBBs(N05_Bishop, from, US);
								return UtilMovePos::MakeCapturePromoteMove(N05_Bishop, from, to, *this);
							}
						}
					} while (toBB.Exists1Bit());
					XorBBs(N05_Bishop, from, US);
				}
			} while (fromOn789BB.Exists1Bit());
		}

		// from が 4~9 段目
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			Bitboard toBB = moveTarget & PieceTypeArray::m_bishop.GetAttacks2From(this->GetOccupiedBB(), Color::ColorNum, from) & (g_bishopAttackBb.BishopStepAttacks(ksq) | TRank789BB);
			if (toBB.Exists1Bit()) {
				XorBBs(N05_Bishop, from, US);
				// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
				const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();

				Bitboard toOn789BB = toBB & TRank789BB;
				// 成り
				if (toOn789BB.Exists1Bit()) {
					do {
						const Square to = toOn789BB.PopFirstOneFromI9();
						if (IsUnDropCheckIsSupported(US, to)) {
							if (!canKingEscape(*this, US, to, g_horseAttackBb.GetControllBbToEdge(to))
								&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
									|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
								&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
							{
								XorBBs(N05_Bishop, from, US);
								return UtilMovePos::MakeCapturePromoteMove(N05_Bishop, from, to, *this);
							}
						}
					} while (toOn789BB.Exists1Bit());

					toBB.AndEqualNot(TRank789BB);
				}
				// 不成
				while (toBB.Exists1Bit()) {
					const Square to = toBB.PopFirstOneFromI9();
					if (IsUnDropCheckIsSupported(US, to)) {
						if (!canKingEscape(*this, US, to, g_bishopAttackBb.GetControllBbToEdge(to))
							&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
								|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
							&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
						{
							XorBBs(N05_Bishop, from, US);
							return UtilMovePos::MakeCaptureMove(N05_Bishop, from, to, *this);
						}
					}
				}
				XorBBs(N05_Bishop, from, US);
			}
		}
	}

	{
		// 金、成り金による移動
		Bitboard fromBB = GetGoldsBB(US) & g_goldAttackBb.GoldCheckTable(US, ksq);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			Bitboard toBB = moveTarget & PieceTypeArray::m_gold.GetAttacks2From(g_nullBitboard, US, from) & PieceTypeArray::m_gold.GetAttacks2From(g_nullBitboard, Them, ksq);
			if (toBB.Exists1Bit()) {
				const PieceType pt = UtilPiece::ToPieceType(GetPiece(from));
				XorBBs(pt, from, US);
				g_setMaskBb.XorBit(&m_goldsBB_, from);
				// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
				const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();
				// to の位置の Bitboard は canKingEscape の中で更新する。
				do {
					const Square to = toBB.PopFirstOneFromI9();
					// 王手した駒の場所に自駒の利きがあるか。(無ければ玉で取られて詰まない)
					if (IsUnDropCheckIsSupported(US, to)) {
						// 玉が逃げられない
						// かつ、(空き王手 または 他の駒で取れない)
						// かつ、動かした駒が pin されていない)
						if (!canKingEscape(*this, US, to, PieceTypeArray::m_gold.GetAttacks2From(g_nullBitboard, US, to))
							&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
								|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
							&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
						{
							XorBBs(pt, from, US);
							g_setMaskBb.XorBit(&m_goldsBB_, from);
							return UtilMovePos::MakeCaptureMove(pt, from, to, *this);
						}
					}
				} while (toBB.Exists1Bit());
				XorBBs(pt, from, US);
				g_setMaskBb.XorBit(&m_goldsBB_, from);
			}
		}
	}

	{
		// 銀による移動
		Bitboard fromBB = GetBbOf(N04_Silver, US) & g_silverAttackBb.SilverCheckTable(US, ksq);
		if (fromBB.Exists1Bit()) {
			// Txxx は先手、後手の情報を吸収した変数。数字は先手に合わせている。
			const Bitboard TRank1_5BB = g_inFrontMaskBb.GetInFrontMask<Them, TRank6>();
			const Bitboard chkBB = PieceTypeArray::m_silver.GetAttacks2From(g_nullBitboard, Them, ksq);
			const Bitboard chkBB_promo = PieceTypeArray::m_gold.GetAttacks2From(g_nullBitboard, Them, ksq);

			Bitboard fromOn789BB = fromBB & TRank789BB;
			// from が敵陣
			if (fromOn789BB.Exists1Bit()) {
				fromBB.AndEqualNot(TRank789BB);
				do {
					const Square from = fromOn789BB.PopFirstOneFromI9();
					Bitboard toBB = moveTarget & PieceTypeArray::m_silver.GetAttacks2From(g_nullBitboard, US, from);
					Bitboard toBB_promo = toBB & chkBB_promo;

					toBB &= chkBB;
					if ((toBB_promo | toBB).Exists1Bit()) {
						XorBBs(N04_Silver, from, US);
						// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
						const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();
						// to の位置の Bitboard は canKingEscape の中で更新する。
						while (toBB_promo.Exists1Bit()) {
							const Square to = toBB_promo.PopFirstOneFromI9();
							if (IsUnDropCheckIsSupported(US, to)) {
								// 成り
								if (!canKingEscape(*this, US, to, PieceTypeArray::m_gold.GetAttacks2From(g_nullBitboard, US, to))
									&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
										|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
									&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
								{
									XorBBs(N04_Silver, from, US);
									return UtilMovePos::MakeCapturePromoteMove(N04_Silver, from, to, *this);
								}
							}
						}

						// 玉の前方に移動する場合、成で詰まなかったら不成でも詰まないので、ここで省く。
						// sakurapyon の作者が言ってたので実装。
						toBB.AndEqualNot(g_inFrontMaskBb.GetInFrontMask(Them, UtilSquare::ToRank(ksq)));
						while (toBB.Exists1Bit()) {
							const Square to = toBB.PopFirstOneFromI9();
							if (IsUnDropCheckIsSupported(US, to)) {
								// 不成
								if (!canKingEscape(*this, US, to, PieceTypeArray::m_silver.GetAttacks2From(g_nullBitboard, US, to))
									&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
										|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
									&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
								{
									XorBBs(N04_Silver, from, US);
									return UtilMovePos::MakeCaptureMove(N04_Silver, from, to, *this);
								}
							}
						}

						XorBBs(N04_Silver, from, US);
					}
				} while (fromOn789BB.Exists1Bit());
			}

			// from が 5~9段目 (必ず不成)
			Bitboard fromOn1_5BB = fromBB & TRank1_5BB;
			if (fromOn1_5BB.Exists1Bit()) {
				fromBB.AndEqualNot(TRank1_5BB);
				do {
					const Square from = fromOn1_5BB.PopFirstOneFromI9();
					Bitboard toBB = moveTarget & PieceTypeArray::m_silver.GetAttacks2From( g_nullBitboard, US, from) & chkBB;

					if (toBB.Exists1Bit()) {
						XorBBs(N04_Silver, from, US);
						// 動いた後の dcBB, pinned: to の位置の occupied や checkers は関係ないので、ここで生成できる。
						const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();
						// to の位置の Bitboard は canKingEscape の中で更新する。
						while (toBB.Exists1Bit()) {
							const Square to = toBB.PopFirstOneFromI9();
							if (IsUnDropCheckIsSupported(US, to)) {
								// 不成
								if (!canKingEscape(*this, US, to, PieceTypeArray::m_silver.GetAttacks2From(g_nullBitboard, US, to))
									&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
										|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
									&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
								{
									XorBBs(N04_Silver, from, US);
									return UtilMovePos::MakeCaptureMove(N04_Silver, from, to, *this);
								}
							}
						}

						XorBBs(N04_Silver, from, US);
					}
				} while (fromOn1_5BB.Exists1Bit());
			}

			// 残り 4 段目のみ
			// 前進するときは成れるが、後退するときは成れない。
			while (fromBB.Exists1Bit()) {
				const Square from = fromBB.PopFirstOneFromI9();
				Bitboard toBB = moveTarget & PieceTypeArray::m_silver.GetAttacks2From(g_nullBitboard, US, from);
				Bitboard toBB_promo = toBB & TRank789BB & chkBB_promo; // 3 段目にしか成れない。

				toBB &= chkBB;
				if ((toBB_promo | toBB).Exists1Bit()) {
					XorBBs(N04_Silver, from, US);
					// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
					const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();
					// to の位置の Bitboard は canKingEscape の中で更新する。
					while (toBB_promo.Exists1Bit()) {
						const Square to = toBB_promo.PopFirstOneFromI9();
						if (IsUnDropCheckIsSupported(US, to)) {
							// 成り
							if (!canKingEscape(*this, US, to, PieceTypeArray::m_gold.GetAttacks2From(g_nullBitboard, US, to))
								&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
									|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
								&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
							{
								XorBBs(N04_Silver, from, US);
								return UtilMovePos::MakeCapturePromoteMove(N04_Silver, from, to, *this);
							}
						}
					}

					while (toBB.Exists1Bit()) {
						const Square to = toBB.PopFirstOneFromI9();
						if (IsUnDropCheckIsSupported(US, to)) {
							// 不成
							if (!canKingEscape(*this, US, to, PieceTypeArray::m_silver.GetAttacks2From(g_nullBitboard,US, to))
								&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
									|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
								&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
							{
								XorBBs(N04_Silver, from, US);
								return UtilMovePos::MakeCaptureMove(N04_Silver, from, to, *this);
							}
						}
					}

					XorBBs(N04_Silver, from, US);
				}
			}
		}
	}

	{
		// 桂による移動
		Bitboard fromBB = GetBbOf(N03_Knight, US) & g_knightAttackBb.KnightCheckTable(US, ksq);
		if (fromBB.Exists1Bit()) {
			const Bitboard chkBB_promo = PieceTypeArray::m_gold.GetAttacks2From(g_nullBitboard,Them, ksq) & TRank789BB;
			const Bitboard chkBB = PieceTypeArray::m_knight.GetAttacks2From(g_nullBitboard,Them, ksq);

			do {
				const Square from = fromBB.PopFirstOneFromI9();
				Bitboard toBB = GetBbOf(US).NotThisAnd(PieceTypeArray::m_knight.GetAttacks2From(g_nullBitboard,US, from));
				Bitboard toBB_promo = toBB & chkBB_promo;
				toBB &= chkBB;
				if ((toBB_promo | toBB).Exists1Bit()) {
					XorBBs(N03_Knight, from, US);
					// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
					const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();
					// to の位置の Bitboard は canKingEscape の中で更新する。
					while (toBB_promo.Exists1Bit()) {
						const Square to = toBB_promo.PopFirstOneFromI9();
						if (IsUnDropCheckIsSupported(US, to)) {
							// 成り
							if (!canKingEscape(*this, US, to, PieceTypeArray::m_gold.GetAttacks2From(g_nullBitboard,US, to))
								&& (IsDiscoveredCheck<true>(from, to, ksq, dcBB_betweenIsUs)
									|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
								&& !IsPinnedIllegal<true>(from, to, GetKingSquare(US), pinned))
							{
								XorBBs(N03_Knight, from, US);
								return UtilMovePos::MakeCapturePromoteMove(N03_Knight, from, to, *this);
							}
						}
					}

					while (toBB.Exists1Bit()) {
						const Square to = toBB.PopFirstOneFromI9();
						// 桂馬は紐が付いてなくて良いので、紐が付いているかは調べない。
						// 不成
						if (!canKingEscape(*this, US, to, Bitboard::CreateAllZeroBB())
							&& (IsDiscoveredCheck<true>(from, to, ksq, dcBB_betweenIsUs)
								|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
							&& !IsPinnedIllegal<true>(from, to, GetKingSquare(US), pinned))
						{
							XorBBs(N03_Knight, from, US);
							return UtilMovePos::MakeCaptureMove(N03_Knight, from, to, *this);
						}
					}
					XorBBs(N03_Knight, from, US);
				}
			} while (fromBB.Exists1Bit());
		}
	}

	{
		// 香車による移動
		Bitboard fromBB = GetBbOf(N02_Lance, US) & g_lanceAttackBb.LanceCheckTable(US, ksq);
		if (fromBB.Exists1Bit()) {
			// Txxx は先手、後手の情報を吸収した変数。数字は先手に合わせている。
			const SquareDelta TDeltaS = (US == Black ? DeltaS : DeltaN);
			const Rank TRank8 = (US == Black ? Rank8 : Rank2);
			const Bitboard chkBB_promo = PieceTypeArray::m_gold.GetAttacks2From( g_nullBitboard, Them, ksq) & TRank789BB;
			// 玉の前方1マスのみ。
			// 玉が 1 段目にいるときは、成のみで良いので省く。
			const Bitboard chkBB = PieceTypeArray::m_pawn.GetAttacks2From(g_nullBitboard, Them, ksq) & g_inFrontMaskBb.GetInFrontMask<Them, TRank8>();

			do {
				const Square from = fromBB.PopFirstOneFromI9();
				Bitboard toBB = moveTarget & PieceTypeArray::m_lance.GetAttacks2From(this->GetOccupiedBB(), US, from);
				Bitboard toBB_promo = toBB & chkBB_promo;

				toBB &= chkBB;

				if ((toBB_promo | toBB).Exists1Bit()) {
					XorBBs(N02_Lance, from, US);
					// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
					const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();
					// to の位置の Bitboard は canKingEscape の中で更新する。

					while (toBB_promo.Exists1Bit()) {
						const Square to = toBB_promo.PopFirstOneFromI9();
						if (IsUnDropCheckIsSupported(US, to)) {
							// 成り
							if (!canKingEscape(*this, US, to, PieceTypeArray::m_gold.GetAttacks2From(g_nullBitboard, US, to))
								&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
									|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
								&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
							{
								XorBBs(N02_Lance, from, US);
								return UtilMovePos::MakeCapturePromoteMove(N02_Lance, from, to, *this);
							}
						}
					}

					if (toBB.Exists1Bit()) {
						assert(toBB.IsOneBit());
						// 不成で王手出来るのは、一つの場所だけなので、ループにする必要が無い。
						const Square to = ksq + TDeltaS;
						if (IsUnDropCheckIsSupported(US, to)) {
							// 不成
							if (!canKingEscape(*this, US, to, g_lanceAttackBb.GetControllBbToEdge(US, to))
								&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
									|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
								&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
							{
								XorBBs(N02_Lance, from, US);
								return UtilMovePos::MakeCaptureMove(N02_Lance, from, to, *this);
							}
						}
					}
					XorBBs(N02_Lance, from, US);
				}
			} while (fromBB.Exists1Bit());
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

			Bitboard fromBB = GetBbOf(N01_Pawn, US);
			// 玉が敵陣にいないと成で王手になることはない。
			if (UtilSquare::IsInFrontOf<US, Rank6, Rank4>(krank)) {
				// 成った時に王手になる位置
				const Bitboard toBB_promo = moveTarget & PieceTypeArray::m_gold.GetAttacks2From(g_nullBitboard, Them, ksq) & TRank789BB;
				Bitboard fromBB_promo = fromBB & Bitboard::PawnAttack(toBB_promo, Them);
				while (fromBB_promo.Exists1Bit()) {
					const Square from = fromBB_promo.PopFirstOneFromI9();
					const Square to = from + TDeltaN;

					XorBBs(N01_Pawn, from, US);
					// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
					const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();
					// to の位置の Bitboard は canKingEscape の中で更新する。
					if (IsUnDropCheckIsSupported(US, to)) {
						// 成り
						if (!canKingEscape(*this, US, to, PieceTypeArray::m_gold.GetAttacks2From(g_nullBitboard, US, to))
							&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
								|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
							&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
						{
							XorBBs(N01_Pawn, from, US);
							return UtilMovePos::MakeCapturePromoteMove(N01_Pawn, from, to, *this);
						}
					}
					XorBBs(N01_Pawn, from, US);
				}
			}

			// 不成
			// 玉が 8,9 段目にいることは無いので、from,to が隣の筋を指すことは無い。
			const Square to = ksq + TDeltaS;
			const Square from = to + TDeltaS;
			if (g_setMaskBb.IsSet(&fromBB, from) && !g_setMaskBb.IsSet(&GetBbOf(US), to)) {
				// 玉が 1, 2 段目にいるなら、成りで王手出来るので不成は調べない。
				if (UtilSquare::IsBehind<US, Rank8, Rank2>(krank)) {
					XorBBs(N01_Pawn, from, US);
					// 動いた後の dcBB: to の位置の occupied や checkers は関係ないので、ここで生成できる。
					const Bitboard dcBB_betweenIsThem_after = DiscoveredCheckBB<false>();
					// to の位置の Bitboard は canKingEscape の中で更新する。
					if (IsUnDropCheckIsSupported(US, to)) {
						// 不成
						if (!canKingEscape(*this, US, to, Bitboard::CreateAllZeroBB())
							&& (IsDiscoveredCheck(from, to, ksq, dcBB_betweenIsUs)
								|| !canPieceCapture(*this, Them, to, dcBB_betweenIsThem_after))
							&& !IsPinnedIllegal(from, to, GetKingSquare(US), pinned))
						{
							XorBBs(N01_Pawn, from, US);
							return UtilMovePos::MakeCaptureMove(N01_Pawn, from, to, *this);
						}
					}
					XorBBs(N01_Pawn, from, US);
				}
			}
		}
	}

	return Move::GetMoveNone();
}

Move Position::GetMateMoveIn1Ply() {
	return (GetTurn() == Black ? GetMateMoveIn1Ply<Black>() : GetMateMoveIn1Ply<White>());
}

Ply Position::GetGamePly() const
{
	return m_gamePly_;
}

Key Position::GetBoardKey() const
{
	return m_st_->m_boardKey;
}

Key Position::GetHandKey() const
{
	return m_st_->m_handKey;
}

Key Position::GetKey() const
{
	return m_st_->GetKey();
}

Key Position::GetExclusionKey() const
{
	return m_st_->GetKey() ^ m_zobExclusion_;
}

Key Position::GetKeyExcludeTurn() const
{
	static_assert(m_zobTurn_ == 1, "");
	return GetKey() >> 1;
}

void Position::InitZobrist() {
	// zobTurn_ は 1 であり、その他は 1桁目を使わない。
	// zobTurn のみ xor で更新する為、他の桁に影響しないようにする為。
	// hash値の更新は普通は全て xor を使うが、持ち駒の更新の為に +, - を使用した方が都合が良い。
	for (PieceType pt = N00_Occupied; pt < N15_PieceTypeNum; ++pt) {
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

Score Position::GetPieceScore(const Piece pc)
{
	return g_PieceScore[pc];
}

Score Position::GetPieceScore(const PieceType pt)
{
	return g_PieceScore[pt];
}

Score Position::GetCapturePieceScore(const Piece pc)
{
	return g_CapturePieceScore[pc];
}

Score Position::GetCapturePieceScore(const PieceType pt)
{
	return g_CapturePieceScore[pt];
}

Score Position::GetPromotePieceScore(const PieceType pt)
{
	assert(pt < N07_Gold);
	return g_PromotePieceScore[pt];
}









// Print() で使う。
namespace {
	const char* PieceToCharCSATable[N31_PieceNone] = {
		" * ", "+FU", "+KY", "+KE", "+GI", "+KA", "+HI", "+KI", "+OU", "+TO", "+NY", "+NK", "+NG", "+UM", "+RY", "", "",
		"-FU", "-KY", "-KE", "-GI", "-KA", "-HI", "-KI", "-OU", "-TO", "-NY", "-NK", "-NG", "-UM", "-RY"
	};
	inline const char* pieceToCharCSA(const Piece pc) {
		return PieceToCharCSATable[pc];
	}
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

u64 Position::GetNodesSearched() const
{
	return m_nodes_;
}

void Position::SetNodesSearched(const u64 n)
{
	m_nodes_ = n;
}

#if !defined NDEBUG
bool Position::IsOK() const {
	static Key prevKey;
	const bool debugAll = true;

	const bool debugBitboards = debugAll || false;
	const bool debugKingCount = debugAll || false;
	const bool debugKingCapture = debugAll || false;
	const bool debugCheckerCount = debugAll || false;
	const bool debugKey = debugAll || false;
	const bool debugStateHand = debugAll || false;
	const bool debugPiece = debugAll || false;
	const bool debugMaterial = debugAll || false;

	int failedStep = 0;
	if (debugBitboards) {
		if ((GetBbOf(Black) & GetBbOf(White)).Exists1Bit()) {
			goto incorrect_position;
		}
		if ((GetBbOf(Black) | GetBbOf(White)) != GetOccupiedBB()) {
			goto incorrect_position;
		}
		if ((GetBbOf(N01_Pawn) ^ GetBbOf(N02_Lance) ^ GetBbOf(N03_Knight) ^ GetBbOf(N04_Silver) ^ GetBbOf(N05_Bishop) ^
			GetBbOf(N06_Rook) ^ GetBbOf(N07_Gold) ^ GetBbOf(N08_King) ^ GetBbOf(N09_ProPawn) ^ GetBbOf(N10_ProLance) ^
			GetBbOf(N11_ProKnight) ^ GetBbOf(N12_ProSilver) ^ GetBbOf(N13_Horse) ^ GetBbOf(N14_Dragon)) != GetOccupiedBB())
		{
			goto incorrect_position;
		}
		for (PieceType pt1 = N01_Pawn; pt1 < N15_PieceTypeNum; ++pt1) {
			for (PieceType pt2 = pt1 + 1; pt2 < N15_PieceTypeNum; ++pt2) {
				if ((GetBbOf(pt1) & GetBbOf(pt2)).Exists1Bit()) {
					goto incorrect_position;
				}
			}
		}
	}

	++failedStep;
	if (debugKingCount) {
		int kingCount[ColorNum] = { 0, 0 };
		if (GetBbOf(N08_King).PopCount() != 2) {
			goto incorrect_position;
		}
		if (!GetBbOf(N08_King, Black).IsOneBit()) {
			goto incorrect_position;
		}
		if (!GetBbOf(N08_King, White).IsOneBit()) {
			goto incorrect_position;
		}
		for (Square sq = I9; sq < SquareNum; ++sq) {
			if (GetPiece(sq) == N08_BKing) {
				++kingCount[Black];
			}
			if (GetPiece(sq) == N24_WKing) {
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
		if (this->GetAttackersTo(us, ksq, g_nullBitboard).Exists1Bit()) {
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
		if (m_st_->m_hand != this->GetHand(GetTurn())) {
			goto incorrect_position;
		}
	}

	++failedStep;
	if (debugPiece) {
		for (Square sq = I9; sq < SquareNum; ++sq) {
			const Piece pc = GetPiece(sq);
			if (pc == N00_Empty) {
				if (!g_setMaskBb.IsSet( &GetEmptyBB(), sq) ) {
					goto incorrect_position;
				}
			}
			else {
				if (!g_setMaskBb.IsSet( &GetBbOf(UtilPiece::ToPieceType(pc), UtilPiece::ToColor(pc)), sq) ) {
					goto incorrect_position;
				}
			}
		}
	}

	++failedStep;
	if (debugMaterial) {
		if (this->GetMaterial() != ComputeMaterial()) {
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
void Position::SetEvalList()
{
	m_evalList_.Set(*this);
}

Key Position::GetComputeBoardKey() const {
	Key result = 0;
	for (Square sq = I9; sq < SquareNum; ++sq) {
		if (GetPiece(sq) != N00_Empty) {
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

Key Position::GetComputeKey() const
{
	return GetComputeBoardKey() + GetComputeHandKey();
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
					return N03_RepetitionLose;
				}
				else if (i <= m_st_->m_continuousCheck[UtilColor::OppositeColor(GetTurn())]) {
					return N02_RepetitionWin;
				}
#if defined BAN_BLACK_REPETITION
				return (GetTurn() == Black ? N03_RepetitionLose : N02_RepetitionWin);
#elif defined BAN_WHITE_REPETITION
				return (GetTurn() == White ? N03_RepetitionLose : N02_RepetitionWin);
#else
				return N01_RepetitionDraw;
#endif
			}
			else if (stp->m_boardKey == m_st_->m_boardKey) {
				if (m_st_->m_hand.IsEqualOrSuperior(stp->m_hand)) { return N04_RepetitionSuperior; }
				if (stp->m_hand.IsEqualOrSuperior(m_st_->m_hand)) { return N05_RepetitionInferior; }
			}
			i += 2;
		} while (i <= e);
	}
	return N00_NotRepetition;
}

Thread * Position::GetThisThread() const
{
	return m_thisThread_;
}

void Position::SetStartPosPly(const Ply ply)
{
	m_gamePly_ = ply;
}

int Position::GetList0(const int index) const
{
	return m_evalList_.m_list0[index];
}

int Position::GetList1(const int index) const
{
	return m_evalList_.m_list1[index];
}

int Position::GetSquareHandToList(const Square sq) const
{
	return m_evalList_.m_squareHandToList[sq];
}

Square Position::GetListToSquareHand(const int i) const
{
	return m_evalList_.m_listToSquareHand[i];
}

int * Position::GetPlist0()
{
	return &m_evalList_.m_list0[0];
}

int * Position::GetPlist1()
{
	return &m_evalList_.m_list1[0];
}

const int * Position::GetCplist0() const
{
	return &m_evalList_.m_list0[0];
}

const int * Position::GetCplist1() const
{
	return &m_evalList_.m_list1[0];
}

const ChangedLists & Position::GetCl() const
{
	return m_st_->m_cl;
}

const Searcher * Position::GetCsearcher() const
{
	return m_searcher_;
}

Searcher * Position::GetSearcher() const
{
	return m_searcher_;
}

void Position::SetSearcher(Searcher * s)
{
	m_searcher_ = s;
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
	printHandPiece(*this, HPawn, c, "FU");
	printHandPiece(*this, HLance, c, "KY");
	printHandPiece(*this, HKnight, c, "KE");
	printHandPiece(*this, HSilver, c, "GI");
	printHandPiece(*this, HGold, c, "KI");
	printHandPiece(*this, HBishop, c, "KA");
	printHandPiece(*this, HRook, c, "HI");
}

Key Position::GetZobrist(const PieceType pt, const Square sq, const Color c)
{
	return m_zobrist_[pt][sq][c];
}

Key Position::GetZobTurn()
{
	return m_zobTurn_;
}

Key Position::GetZobHand(const HandPiece hp, const Color c)
{
	return m_zobHand_[hp][c];
}

Position::Position()
{
	// デフォルト・コンストラクタは空っぽ☆（＾ｑ＾）
}

Position::Position(Searcher * s) : m_searcher_(s)
{
}

Position::Position(const Position & pos)
{
	*this = pos;
}

Position::Position(const Position & pos, Thread * th)
{
	*this = pos;
	m_thisThread_ = th;
}

Position::Position(const std::string & sfen, Thread * th, Searcher * s)
{
	Set(sfen, th);
	SetSearcher(s);
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
		else if (g_charToPieceUSI.IsLegalChar(token)) {
			if (UtilSquare::ContainsOf(sq)) {
				SetPiece(g_charToPieceUSI.GetValue(token) + promoteFlag, sq);
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
	m_kingSquare_[Black] = GetBbOf(N08_King, Black).GetFirstOneFromI9();
	m_kingSquare_[White] = GetBbOf(N08_King, White).GetFirstOneFromI9();
	m_goldsBB_ = GetBbOf(N07_Gold, N09_ProPawn, N10_ProLance, N11_ProKnight, N12_ProSilver);

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
		else if (g_charToPieceUSI.IsLegalChar(token)) {
			// 持ち駒を32bit に pack する
			const Piece piece = g_charToPieceUSI.GetValue(token);
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

Bitboard Position::GetBbOf(const PieceType pt) const
{
	return this->m_byTypeBB_[pt];
}

Bitboard Position::GetBbOf(const Color c) const
{
	return this->m_byColorBB_[c];
}

Bitboard Position::GetBbOf(const PieceType pt, const Color c) const
{
	return this->GetBbOf(pt) & this->GetBbOf(c);
}

Bitboard Position::GetBbOf(const PieceType pt1, const PieceType pt2) const
{
	return this->GetBbOf(pt1) | this->GetBbOf(pt2);
}

Bitboard Position::GetBbOf(const PieceType pt1, const PieceType pt2, const Color c) const
{
	return this->GetBbOf(pt1, pt2) & this->GetBbOf(c);
}

Bitboard Position::GetBbOf(const PieceType pt1, const PieceType pt2, const PieceType pt3) const
{
	return this->GetBbOf(pt1, pt2) | this->GetBbOf(pt3);
}

Bitboard Position::GetBbOf(const PieceType pt1, const PieceType pt2, const PieceType pt3, const PieceType pt4) const
{
	return this->GetBbOf(pt1, pt2, pt3) | this->GetBbOf(pt4);
}

Bitboard Position::GetBbOf(const PieceType pt1, const PieceType pt2, const PieceType pt3, const PieceType pt4, const PieceType pt5) const
{
	return this->GetBbOf(pt1, pt2, pt3, pt4) | this->GetBbOf(pt5);
}

Bitboard Position::GetOccupiedBB() const
{
	return this->GetBbOf(N00_Occupied);
}

Bitboard Position::GetNOccupiedBB() const
{
	return ~GetOccupiedBB();
}

Bitboard Position::GetEmptyBB() const
{
	return GetOccupiedBB() ^ Bitboard::CreateAllOneBB();
}

Bitboard Position::GetGoldsBB() const
{
	return m_goldsBB_;
}

Bitboard Position::GetGoldsBB(const Color c) const
{
	return GetGoldsBB() & this->GetBbOf(c);
}

Piece Position::GetPiece(const Square sq) const
{
	return m_piece_[sq];
}

Hand Position::GetHand(const Color c) const
{
	return m_hand_[c];
}

Bitboard Position::GetPinnedBB() const
{
	return GetHiddenCheckers<true, true>();
}

Bitboard Position::GetCheckersBB() const
{
	return m_st_->m_checkersBB;
}

Bitboard Position::GetPrevCheckersBB() const
{
	return m_st_->m_previous->m_checkersBB;
}

bool Position::InCheck() const
{
	return GetCheckersBB().Exists1Bit();
}

Score Position::GetMaterial() const
{
	return m_st_->m_material;
}

Score Position::GetMaterialDiff() const
{
	return m_st_->m_material - m_st_->m_previous->m_material;
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
		if (g_setMaskBb.IsSet(&ci.m_checkBB[ptTo], to)) {
			return true;
		}
	}
	else {
		const Square from = move.From();
		const PieceType ptFrom = move.GetPieceTypeFrom();
		const PieceType ptTo = move.GetPieceTypeTo(ptFrom);
		assert(ptFrom == UtilPiece::ToPieceType(GetPiece(from)));
		// Direct Check ?
		if (g_setMaskBb.IsSet(&ci.m_checkBB[ptTo], to)) {
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

void Position::SetPiece(const Piece piece, const Square sq)
{
	const Color c = UtilPiece::ToColor(piece);
	const PieceType pt = UtilPiece::ToPieceType(piece);

	m_piece_[sq] = piece;

	g_setMaskBb.SetBit(&m_byTypeBB_[pt], sq);
	g_setMaskBb.SetBit(&m_byColorBB_[c], sq);
	g_setMaskBb.SetBit(&m_byTypeBB_[N00_Occupied], sq);
}

void Position::SetHand(const HandPiece hp, const Color c, const int num)
{
	m_hand_[c].OrEqual(num, hp);
}

void Position::SetHand(const Piece piece, const int num)
{
	const Color c = UtilPiece::ToColor(piece);
	const PieceType pt = UtilPiece::ToPieceType(piece);
	const HandPiece hp = UtilHandPiece::FromPieceType(pt);
	SetHand(hp, c, num);
}

// 先手、後手に関わらず、sq へ移動可能な Bitboard を返す。
Bitboard Position::GetAttackersTo(const Square sq, const Bitboard& occupied) const {

	const Bitboard golds = GetGoldsBB();

	return (((PieceTypeArray::m_pawn.GetAttacks2From(g_nullBitboard, Black, sq) & this->GetBbOf(N01_Pawn))
		| (PieceTypeArray::m_lance.GetAttacks2From(occupied, Black, sq) & this->GetBbOf(N02_Lance))
		| (PieceTypeArray::m_knight.GetAttacks2From(g_nullBitboard, Black, sq) & this->GetBbOf(N03_Knight))
		| (PieceTypeArray::m_silver.GetAttacks2From(g_nullBitboard, Black, sq) & this->GetBbOf(N04_Silver))
		| (PieceTypeArray::m_gold.GetAttacks2From(g_nullBitboard, Black, sq) & golds))
		& this->GetBbOf(White))
		| (((PieceTypeArray::m_pawn.GetAttacks2From(g_nullBitboard, White, sq) & this->GetBbOf(N01_Pawn))
			| (PieceTypeArray::m_lance.GetAttacks2From(occupied, White, sq) & this->GetBbOf(N02_Lance))
			| (PieceTypeArray::m_knight.GetAttacks2From(g_nullBitboard, White, sq) & this->GetBbOf(N03_Knight))
			| (PieceTypeArray::m_silver.GetAttacks2From(g_nullBitboard, White, sq) & this->GetBbOf(N04_Silver))
			| (PieceTypeArray::m_gold.GetAttacks2From(g_nullBitboard, White, sq) & golds))
			& this->GetBbOf(Black))
		| (PieceTypeArray::m_bishop.GetAttacks2From(occupied, Color::ColorNum, sq) & this->GetBbOf(N05_Bishop, N13_Horse))
		| (PieceTypeArray::m_rook.GetAttacks2From(occupied, Color::ColorNum, sq) & this->GetBbOf(N06_Rook, N14_Dragon))
		| (PieceTypeArray::m_king.GetAttacks2From(g_nullBitboard, Color::ColorNum, sq) & this->GetBbOf(N08_King, N13_Horse, N14_Dragon));
}

// occupied を Position::occupiedBB() 以外のものを使用する場合に使用する。
Bitboard Position::GetAttackersTo(const Color c, const Square sq, const Bitboard& occupied) const {
	const Color opposite = UtilColor::OppositeColor(c);
	return ((PieceTypeArray::m_pawn.GetAttacks2From( g_nullBitboard, opposite, sq) & this->GetBbOf(N01_Pawn))
		| (PieceTypeArray::m_lance.GetAttacks2From(occupied, opposite, sq) & this->GetBbOf(N02_Lance))
		| (PieceTypeArray::m_knight.GetAttacks2From(g_nullBitboard, opposite, sq) & this->GetBbOf(N03_Knight))
		| (PieceTypeArray::m_silver.GetAttacks2From(g_nullBitboard, opposite, sq) & this->GetBbOf(N04_Silver))
		| (PieceTypeArray::m_gold.GetAttacks2From(g_nullBitboard, opposite, sq) & GetGoldsBB())
		| (PieceTypeArray::m_bishop.GetAttacks2From(occupied, Color::ColorNum, sq) & this->GetBbOf(N05_Bishop, N13_Horse))
		| (PieceTypeArray::m_rook.GetAttacks2From(occupied, Color::ColorNum, sq) & this->GetBbOf(N06_Rook, N14_Dragon))
		| (PieceTypeArray::m_king.GetAttacks2From(g_nullBitboard, Color::ColorNum, sq) & this->GetBbOf(N08_King, N13_Horse, N14_Dragon)))
		& this->GetBbOf(c);
}

// 玉以外で sq へ移動可能な c 側の駒の Bitboard を返す。
Bitboard Position::GetAttackersToExceptKing(const Color c, const Square sq) const {
	const Color opposite = UtilColor::OppositeColor(c);
	return ((PieceTypeArray::m_pawn.GetAttacks2From(g_nullBitboard, opposite, sq) & this->GetBbOf(N01_Pawn))
		| (PieceTypeArray::m_lance.GetAttacks2From( this->GetOccupiedBB(), opposite, sq) & this->GetBbOf(N02_Lance))
		| (PieceTypeArray::m_knight.GetAttacks2From( g_nullBitboard, opposite, sq) & this->GetBbOf(N03_Knight))
		| (PieceTypeArray::m_silver.GetAttacks2From(g_nullBitboard, opposite, sq) & this->GetBbOf(N04_Silver, N14_Dragon))
		| (PieceTypeArray::m_gold.GetAttacks2From(g_nullBitboard, opposite, sq) & (GetGoldsBB() | this->GetBbOf(N13_Horse)))
		| (PieceTypeArray::m_bishop.GetAttacks2From(this->GetOccupiedBB(), Color::ColorNum, sq) & this->GetBbOf(N05_Bishop, N13_Horse))
		| (PieceTypeArray::m_rook.GetAttacks2From(this->GetOccupiedBB(), Color::ColorNum, sq) & this->GetBbOf(N06_Rook, N14_Dragon)))
		& this->GetBbOf(c);
}

bool Position::IsAttackersToIsNot0(const Color c, const Square sq) const
{
	return this->GetAttackersTo(c, sq, this->GetOccupiedBB()).Exists1Bit();
}

bool Position::IsAttackersToIsNot0(const Color c, const Square sq, const Bitboard & occupied) const
{
	return this->GetAttackersTo(c, sq, occupied).Exists1Bit();
}

bool Position::IsUnDropCheckIsSupported(const Color c, const Square sq) const
{
	return this->GetAttackersTo(c, sq, this->GetOccupiedBB()).Exists1Bit();
}

void Position::FindCheckers()
{
	m_st_->m_checkersBB = GetAttackersToExceptKing(UtilColor::OppositeColor(GetTurn()), GetKingSquare(GetTurn()));
}

Score Position::ComputeMaterial() const {
	Score s = ScoreZero;
	for (PieceType pt = N01_Pawn; pt < N15_PieceTypeNum; ++pt) {
		const int num = this->GetBbOf(pt, Black).PopCount() - this->GetBbOf(pt, White).PopCount();
		s += num * GetPieceScore(pt);
	}
	for (PieceType pt = N01_Pawn; pt < N08_King; ++pt) {
		const int num = GetHand(Black).NumOf(UtilHandPiece::FromPieceType(pt)) - GetHand(White).NumOf(UtilHandPiece::FromPieceType(pt));
		s += num * GetPieceScore(pt);
	}
	return s;
}
