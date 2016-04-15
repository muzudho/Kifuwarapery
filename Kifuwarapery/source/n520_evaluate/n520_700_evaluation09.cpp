#include <array>
#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n119_score___/n119_090_score.hpp"
#include "../../header/n220_position/n220_400_evalList.hpp"
#include "../../header/n220_position/n220_650_position.hpp"
#include "../../header/n223_move____/n223_500_searchStack.hpp"
#include "../../header/n520_evaluate/n520_500_kkKkpKppStorage1.hpp"
#include "../../header/n520_evaluate/n520_700_evaluation09.hpp"



EvalSum Evaluation09::doapc(const Position& pos, const int index[2]) {
	const Square sq_bk = pos.GetKingSquare(Black);
	const Square sq_wk = pos.GetKingSquare(White);
	const int* list0 = pos.GetCplist0();
	const int* list1 = pos.GetCplist1();

	EvalSum sum;
	sum.m_p[2][0] = KkKkpKppStorage1::KKP[sq_bk][sq_wk][index[0]][0];
	sum.m_p[2][1] = KkKkpKppStorage1::KKP[sq_bk][sq_wk][index[0]][1];
	const auto* pkppb = KkKkpKppStorage1::KPP[sq_bk][index[0]];
	const auto* pkppw = KkKkpKppStorage1::KPP[UtilSquare::Inverse(sq_wk)][index[1]];
#if defined USE_AVX2_EVAL || defined USE_SSE_EVAL
	GetSum.m[0] = _mm_set_epi32(0, 0, *reinterpret_cast<const s32*>(&pkppw[GetList1[0]][0]), *reinterpret_cast<const s32*>(&pkppb[GetList0[0]][0]));
	GetSum.m[0] = _mm_cvtepi16_epi32(GetSum.m[0]);
	for (int i = 1; i < GetPos.GetNlist(); ++i) {
		__m128i tmp;
		tmp = _mm_set_epi32(0, 0, *reinterpret_cast<const s32*>(&pkppw[GetList1[i]][0]), *reinterpret_cast<const s32*>(&pkppb[GetList0[i]][0]));
		tmp = _mm_cvtepi16_epi32(tmp);
		GetSum.m[0] = _mm_add_epi32(GetSum.m[0], tmp);
	}
#else
	sum.m_p[0][0] = pkppb[list0[0]][0];
	sum.m_p[0][1] = pkppb[list0[0]][1];
	sum.m_p[1][0] = pkppw[list1[0]][0];
	sum.m_p[1][1] = pkppw[list1[0]][1];
	for (int i = 1; i < pos.GetNlist(); ++i) {
		sum.m_p[0] += pkppb[list0[i]];
		sum.m_p[1] += pkppw[list1[i]];
	}
#endif

	return sum;
}




std::array<s32, 2> Evaluation09::doablack(const Position& pos, const int index[2]) {
	const Square sq_bk = pos.GetKingSquare(Black);
	const int* list0 = pos.GetCplist0();

	const auto* pkppb = KkKkpKppStorage1::KPP[sq_bk][index[0]];
	std::array<s32, 2> sum = { { pkppb[list0[0]][0], pkppb[list0[0]][1] } };
	for (int i = 1; i < pos.GetNlist(); ++i) {
		sum[0] += pkppb[list0[i]][0];
		sum[1] += pkppb[list0[i]][1];
	}
	return sum;
}



std::array<s32, 2> Evaluation09::doawhite(const Position& pos, const int index[2]) {
	const Square sq_wk = pos.GetKingSquare(White);
	const int* list1 = pos.GetCplist1();

	const auto* pkppw = KkKkpKppStorage1::KPP[UtilSquare::Inverse(sq_wk)][index[1]];
	std::array<s32, 2> sum = { { pkppw[list1[0]][0], pkppw[list1[0]][1] } };
	for (int i = 1; i < pos.GetNlist(); ++i) {
		sum[0] += pkppw[list1[i]][0];
		sum[1] += pkppw[list1[i]][1];
	}
	return sum;
}



#if defined INANIWA_SHIFT
Score Evaluation09::inaniwaScoreBody(const Position& GetPos) {
	Score GetScore = ScoreZero;
	if (GetPos.GetCsearcher()->inaniwaFlag == InaniwaIsBlack) {
		if (GetPos.GetPiece(B9) == WKnight) { GetScore += 700 * g_FVScale; }
		if (GetPos.GetPiece(H9) == WKnight) { GetScore += 700 * g_FVScale; }
		if (GetPos.GetPiece(A7) == WKnight) { GetScore += 700 * g_FVScale; }
		if (GetPos.GetPiece(I7) == WKnight) { GetScore += 700 * g_FVScale; }
		if (GetPos.GetPiece(C7) == WKnight) { GetScore += 400 * g_FVScale; }
		if (GetPos.GetPiece(G7) == WKnight) { GetScore += 400 * g_FVScale; }
		if (GetPos.GetPiece(B5) == WKnight) { GetScore += 700 * g_FVScale; }
		if (GetPos.GetPiece(H5) == WKnight) { GetScore += 700 * g_FVScale; }
		if (GetPos.GetPiece(D5) == WKnight) { GetScore += 100 * g_FVScale; }
		if (GetPos.GetPiece(F5) == WKnight) { GetScore += 100 * g_FVScale; }
		if (GetPos.GetPiece(E3) == BPawn) { GetScore += 200 * g_FVScale; }
		if (GetPos.GetPiece(E4) == BPawn) { GetScore += 200 * g_FVScale; }
		if (GetPos.GetPiece(E5) == BPawn) { GetScore += 200 * g_FVScale; }
}
	else {
		assert(GetPos.GetCsearcher()->inaniwaFlag == InaniwaIsWhite);
		if (GetPos.GetPiece(B1) == BKnight) { GetScore -= 700 * g_FVScale; }
		if (GetPos.GetPiece(H1) == BKnight) { GetScore -= 700 * g_FVScale; }
		if (GetPos.GetPiece(A3) == BKnight) { GetScore -= 700 * g_FVScale; }
		if (GetPos.GetPiece(I3) == BKnight) { GetScore -= 700 * g_FVScale; }
		if (GetPos.GetPiece(C3) == BKnight) { GetScore -= 400 * g_FVScale; }
		if (GetPos.GetPiece(G3) == BKnight) { GetScore -= 400 * g_FVScale; }
		if (GetPos.GetPiece(B5) == BKnight) { GetScore -= 700 * g_FVScale; }
		if (GetPos.GetPiece(H5) == BKnight) { GetScore -= 700 * g_FVScale; }
		if (GetPos.GetPiece(D5) == BKnight) { GetScore -= 100 * g_FVScale; }
		if (GetPos.GetPiece(F5) == BKnight) { GetScore -= 100 * g_FVScale; }
		if (GetPos.GetPiece(E7) == WPawn) { GetScore -= 200 * g_FVScale; }
		if (GetPos.GetPiece(E6) == WPawn) { GetScore -= 200 * g_FVScale; }
		if (GetPos.GetPiece(E5) == WPawn) { GetScore -= 200 * g_FVScale; }
	}
	return GetScore;
}
inline Score Evaluation09::inaniwaScore(const Position& GetPos) {
	if (GetPos.GetCsearcher()->inaniwaFlag == NotInaniwa) return ScoreZero;
	return inaniwaScoreBody(GetPos);
}
#endif




bool Evaluation09::calcDifference(Position& pos, SearchStack* ss) {
#if defined INANIWA_SHIFT
	if (GetPos.GetCsearcher()->inaniwaFlag != NotInaniwa) return false;
#endif
	if ((ss - 1)->m_staticEvalRaw.m_p[0][0] == ScoreNotEvaluated)
		return false;

	const Move lastMove = (ss - 1)->m_currentMove;
	assert(lastMove != Move::GetMoveNull());

	if (lastMove.GetPieceTypeFrom() == N08_King) {
		EvalSum diff = (ss - 1)->m_staticEvalRaw; // 本当は diff ではないので名前が良くない。
		const Square sq_bk = pos.GetKingSquare(Black);
		const Square sq_wk = pos.GetKingSquare(White);
		diff.m_p[2] = KkKkpKppStorage1::KK[sq_bk][sq_wk];
		diff.m_p[2][0] += pos.GetMaterial() * g_FVScale;
		if (pos.GetTurn() == Black) {
			const auto* ppkppw = KkKkpKppStorage1::KPP[UtilSquare::Inverse(sq_wk)];
			const int* list1 = pos.GetPlist1();
			diff.m_p[1][0] = 0;
			diff.m_p[1][1] = 0;
			for (int i = 0; i < pos.GetNlist(); ++i) {
				const int k1 = list1[i];
				const auto* pkppw = ppkppw[k1];
				for (int j = 0; j < i; ++j) {
					const int l1 = list1[j];
					diff.m_p[1] += pkppw[l1];
				}
				diff.m_p[2][0] -= KkKkpKppStorage1::KKP[UtilSquare::Inverse(sq_wk)][UtilSquare::Inverse(sq_bk)][k1][0];
				diff.m_p[2][1] += KkKkpKppStorage1::KKP[UtilSquare::Inverse(sq_wk)][UtilSquare::Inverse(sq_bk)][k1][1];
			}

			if (pos.GetCl().m_size == 2) {
				const int listIndex_cap = pos.GetCl().m_listindex[1];
				diff.m_p[0] += this->doablack(pos, pos.GetCl().m_clistpair[1].m_newlist);
				pos.GetPlist0()[listIndex_cap] = pos.GetCl().m_clistpair[1].m_oldlist[0];
				diff.m_p[0] -= this->doablack(pos, pos.GetCl().m_clistpair[1].m_oldlist);
				pos.GetPlist0()[listIndex_cap] = pos.GetCl().m_clistpair[1].m_newlist[0];
			}
		}
		else {
			const auto* ppkppb = KkKkpKppStorage1::KPP[sq_bk];
			const int* list0 = pos.GetPlist0();
			diff.m_p[0][0] = 0;
			diff.m_p[0][1] = 0;
			for (int i = 0; i < pos.GetNlist(); ++i) {
				const int k0 = list0[i];
				const auto* pkppb = ppkppb[k0];
				for (int j = 0; j < i; ++j) {
					const int l0 = list0[j];
					diff.m_p[0] += pkppb[l0];
				}
				diff.m_p[2] += KkKkpKppStorage1::KKP[sq_bk][sq_wk][k0];
			}

			if (pos.GetCl().m_size == 2) {
				const int listIndex_cap = pos.GetCl().m_listindex[1];
				diff.m_p[1] += this->doawhite(pos, pos.GetCl().m_clistpair[1].m_newlist);
				pos.GetPlist1()[listIndex_cap] = pos.GetCl().m_clistpair[1].m_oldlist[1];
				diff.m_p[1] -= this->doawhite(pos, pos.GetCl().m_clistpair[1].m_oldlist);
				pos.GetPlist1()[listIndex_cap] = pos.GetCl().m_clistpair[1].m_newlist[1];
			}
		}
		ss->m_staticEvalRaw = diff;
	}
	else {
		const int listIndex = pos.GetCl().m_listindex[0];
		auto diff = this->doapc(pos, pos.GetCl().m_clistpair[0].m_newlist);
		if (pos.GetCl().m_size == 1) {
			pos.GetPlist0()[listIndex] = pos.GetCl().m_clistpair[0].m_oldlist[0];
			pos.GetPlist1()[listIndex] = pos.GetCl().m_clistpair[0].m_oldlist[1];
			diff -= this->doapc(pos, pos.GetCl().m_clistpair[0].m_oldlist);
		}
		else {
			assert(pos.GetCl().m_size == 2);
			diff += this->doapc(pos, pos.GetCl().m_clistpair[1].m_newlist);
			diff.m_p[0] -= KkKkpKppStorage1::KPP[pos.GetKingSquare(Black)][pos.GetCl().m_clistpair[0].m_newlist[0]][pos.GetCl().m_clistpair[1].m_newlist[0]];
			diff.m_p[1] -= KkKkpKppStorage1::KPP[UtilSquare::Inverse(pos.GetKingSquare(White))][pos.GetCl().m_clistpair[0].m_newlist[1]][pos.GetCl().m_clistpair[1].m_newlist[1]];
			const int listIndex_cap = pos.GetCl().m_listindex[1];
			pos.GetPlist0()[listIndex_cap] = pos.GetCl().m_clistpair[1].m_oldlist[0];
			pos.GetPlist1()[listIndex_cap] = pos.GetCl().m_clistpair[1].m_oldlist[1];

			pos.GetPlist0()[listIndex] = pos.GetCl().m_clistpair[0].m_oldlist[0];
			pos.GetPlist1()[listIndex] = pos.GetCl().m_clistpair[0].m_oldlist[1];
			diff -= this->doapc(pos, pos.GetCl().m_clistpair[0].m_oldlist);

			diff -= this->doapc(pos, pos.GetCl().m_clistpair[1].m_oldlist);
			diff.m_p[0] += KkKkpKppStorage1::KPP[pos.GetKingSquare(Black)][pos.GetCl().m_clistpair[0].m_oldlist[0]][pos.GetCl().m_clistpair[1].m_oldlist[0]];
			diff.m_p[1] += KkKkpKppStorage1::KPP[UtilSquare::Inverse(pos.GetKingSquare(White))][pos.GetCl().m_clistpair[0].m_oldlist[1]][pos.GetCl().m_clistpair[1].m_oldlist[1]];
			pos.GetPlist0()[listIndex_cap] = pos.GetCl().m_clistpair[1].m_newlist[0];
			pos.GetPlist1()[listIndex_cap] = pos.GetCl().m_clistpair[1].m_newlist[1];
		}
		pos.GetPlist0()[listIndex] = pos.GetCl().m_clistpair[0].m_newlist[0];
		pos.GetPlist1()[listIndex] = pos.GetCl().m_clistpair[0].m_newlist[1];

		diff.m_p[2][0] += pos.GetMaterialDiff() * g_FVScale;

		ss->m_staticEvalRaw = diff + (ss - 1)->m_staticEvalRaw;
	}

	return true;
}




int Evaluation09::make_list_unUseDiff(const Position& pos, int list0[EvalList::m_ListSize], int list1[EvalList::m_ListSize], int nlist) {
	auto func = [&](const Bitboard& posBB, const int f_pt, const int e_pt) {
		Square sq;
		Bitboard bb;
		bb = (posBB)& pos.GetBbOf(Black);
		FOREACH_BB(bb, sq, {
			list0[nlist] = (f_pt)+sq;
		list1[nlist] = (e_pt)+UtilSquare::Inverse(sq);
		nlist += 1;
		});
		bb = (posBB)& pos.GetBbOf(White);
		FOREACH_BB(bb, sq, {
			list0[nlist] = (e_pt)+sq;
		list1[nlist] = (f_pt)+UtilSquare::Inverse(sq);
		nlist += 1;
		});
	};
	func(pos.GetBbOf(N01_Pawn), f_pawn, e_pawn);
	func(pos.GetBbOf(N02_Lance), f_lance, e_lance);
	func(pos.GetBbOf(N03_Knight), f_knight, e_knight);
	func(pos.GetBbOf(N04_Silver), f_silver, e_silver);
	const Bitboard goldsBB = pos.GetGoldsBB();
	func(goldsBB, f_gold, e_gold);
	func(pos.GetBbOf(N05_Bishop), f_bishop, e_bishop);
	func(pos.GetBbOf(N13_Horse), f_horse, e_horse);
	func(pos.GetBbOf(N06_Rook), f_rook, e_rook);
	func(pos.GetBbOf(N14_Dragon), f_dragon, e_dragon);

	return nlist;
}



void Evaluation09::evaluateBody(Position& pos, SearchStack* ss) {
	if (this->calcDifference(pos, ss)) {
		assert([&] {
			const auto score = ss->m_staticEvalRaw.GetSum(pos.GetTurn());
			return (this->evaluateUnUseDiff(pos) == score);
		}());
		return;
	}

	const Square sq_bk = pos.GetKingSquare(Black);
	const Square sq_wk = pos.GetKingSquare(White);
	const int* list0 = pos.GetPlist0();
	const int* list1 = pos.GetPlist1();

	const auto* ppkppb = KkKkpKppStorage1::KPP[sq_bk];
	const auto* ppkppw = KkKkpKppStorage1::KPP[UtilSquare::Inverse(sq_wk)];

	EvalSum sum;
	sum.m_p[2] = KkKkpKppStorage1::KK[sq_bk][sq_wk];
#if defined USE_AVX2_EVAL || defined USE_SSE_EVAL
	GetSum.m[0] = _mm_setzero_si128();
	for (int i = 0; i < GetPos.GetNlist(); ++i) {
		const int k0 = GetList0[i];
		const int k1 = GetList1[i];
		const auto* pkppb = ppkppb[k0];
		const auto* pkppw = ppkppw[k1];
		for (int j = 0; j < i; ++j) {
			const int l0 = GetList0[j];
			const int l1 = GetList1[j];
			__m128i tmp;
			tmp = _mm_set_epi32(0, 0, *reinterpret_cast<const s32*>(&pkppw[l1][0]), *reinterpret_cast<const s32*>(&pkppb[l0][0]));
			tmp = _mm_cvtepi16_epi32(tmp);
			GetSum.m[0] = _mm_add_epi32(GetSum.m[0], tmp);
		}
		GetSum.GetP[2] += KkKkpKppStorage1::KKP[sq_bk][sq_wk][k0];
	}
#else
	// loop 開始を i = 1 からにして、i = 0 の分のKKPを先に足す。
	sum.m_p[2] += KkKkpKppStorage1::KKP[sq_bk][sq_wk][list0[0]];
	sum.m_p[0][0] = 0;
	sum.m_p[0][1] = 0;
	sum.m_p[1][0] = 0;
	sum.m_p[1][1] = 0;
	for (int i = 1; i < pos.GetNlist(); ++i) {
		const int k0 = list0[i];
		const int k1 = list1[i];
		const auto* pkppb = ppkppb[k0];
		const auto* pkppw = ppkppw[k1];
		for (int j = 0; j < i; ++j) {
			const int l0 = list0[j];
			const int l1 = list1[j];
			sum.m_p[0] += pkppb[l0];
			sum.m_p[1] += pkppw[l1];
		}
		sum.m_p[2] += KkKkpKppStorage1::KKP[sq_bk][sq_wk][k0];
	}
#endif

	sum.m_p[2][0] += pos.GetMaterial() * g_FVScale;
#if defined INANIWA_SHIFT
	GetSum.GetP[2][0] += inaniwaScore(GetPos);
#endif
	ss->m_staticEvalRaw = sum;

	Evaluation09 evaluation;
	assert(this->evaluateUnUseDiff(pos) == sum.GetSum(pos.GetTurn()));
}



// todo: 無名名前空間に入れる。
Score Evaluation09::evaluateUnUseDiff(const Position& pos) {
	int list0[EvalList::m_ListSize];
	int list1[EvalList::m_ListSize];

	const Hand handB = pos.GetHand(Black);
	const Hand handW = pos.GetHand(White);

	const Square sq_bk = pos.GetKingSquare(Black);
	const Square sq_wk = pos.GetKingSquare(White);
	int nlist = 0;

	auto func = [&](const Hand hand, const HandPiece hp, const int list0_index, const int list1_index) {
		for (u32 i = 1; i <= hand.NumOf(hp); ++i) {
			list0[nlist] = list0_index + i;
			list1[nlist] = list1_index + i;
			++nlist;
		}
	};
	func(handB, HPawn, f_hand_pawn, e_hand_pawn);
	func(handW, HPawn, e_hand_pawn, f_hand_pawn);
	func(handB, HLance, f_hand_lance, e_hand_lance);
	func(handW, HLance, e_hand_lance, f_hand_lance);
	func(handB, HKnight, f_hand_knight, e_hand_knight);
	func(handW, HKnight, e_hand_knight, f_hand_knight);
	func(handB, HSilver, f_hand_silver, e_hand_silver);
	func(handW, HSilver, e_hand_silver, f_hand_silver);
	func(handB, HGold, f_hand_gold, e_hand_gold);
	func(handW, HGold, e_hand_gold, f_hand_gold);
	func(handB, HBishop, f_hand_bishop, e_hand_bishop);
	func(handW, HBishop, e_hand_bishop, f_hand_bishop);
	func(handB, HRook, f_hand_rook, e_hand_rook);
	func(handW, HRook, e_hand_rook, f_hand_rook);

	nlist = this->make_list_unUseDiff(pos, list0, list1, nlist);

	const auto* ppkppb = KkKkpKppStorage1::KPP[sq_bk];
	const auto* ppkppw = KkKkpKppStorage1::KPP[UtilSquare::Inverse(sq_wk)];

	EvalSum score;
	score.m_p[2] = KkKkpKppStorage1::KK[sq_bk][sq_wk];

	score.m_p[0][0] = 0;
	score.m_p[0][1] = 0;
	score.m_p[1][0] = 0;
	score.m_p[1][1] = 0;
	for (int i = 0; i < nlist; ++i) {
		const int k0 = list0[i];
		const int k1 = list1[i];
		const auto* pkppb = ppkppb[k0];
		const auto* pkppw = ppkppw[k1];
		for (int j = 0; j < i; ++j) {
			const int l0 = list0[j];
			const int l1 = list1[j];
			score.m_p[0] += pkppb[l0];
			score.m_p[1] += pkppw[l1];
		}
		score.m_p[2] += KkKkpKppStorage1::KKP[sq_bk][sq_wk][k0];
	}

	score.m_p[2][0] += pos.GetMaterial() * g_FVScale;

#if defined INANIWA_SHIFT
	GetScore.GetP[2][0] += inaniwaScore(GetPos);
#endif

	return static_cast<Score>(score.GetSum(pos.GetTurn()));
}


Score Evaluation09::evaluate(Position& pos, SearchStack* ss) {
	if (ss->m_staticEvalRaw.m_p[0][0] != ScoreNotEvaluated) {
		const Score score = static_cast<Score>(ss->m_staticEvalRaw.GetSum(pos.GetTurn()));
		assert(score == evaluateUnUseDiff(pos));
		return score / g_FVScale;
	}

	const Key keyExcludeTurn = pos.GetKeyExcludeTurn();
	EvaluateHashEntry entry = *g_evalTable[keyExcludeTurn]; // atomic にデータを取得する必要がある。
	entry.Decode();
	if (entry.m_key == keyExcludeTurn) {
		ss->m_staticEvalRaw = entry;
		assert(static_cast<Score>(ss->m_staticEvalRaw.GetSum(pos.GetTurn())) == evaluateUnUseDiff(pos));
		return static_cast<Score>(entry.GetSum(pos.GetTurn())) / g_FVScale;
	}

	this->evaluateBody(pos, ss);

	ss->m_staticEvalRaw.m_key = keyExcludeTurn;
	ss->m_staticEvalRaw.Encode();
	*g_evalTable[keyExcludeTurn] = ss->m_staticEvalRaw;
	return static_cast<Score>(ss->m_staticEvalRaw.GetSum(pos.GetTurn())) / g_FVScale;
}
