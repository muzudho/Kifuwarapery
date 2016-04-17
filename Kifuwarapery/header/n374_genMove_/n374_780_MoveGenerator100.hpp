#pragma once


class MoveGenerator100 {
public:

	// 指し手生成 functor
	// テンプレート引数が複数あり、部分特殊化したかったので、関数ではなく、struct にした。
	// ALL == true のとき、歩、飛、角の不成、香の2段目の不成、香の3段目の駒を取らない不成も生成する。
	template <MoveType MT, bool ALL = false>
	struct GenerateMoves {
		MoveStack* operator () (
			Color us,
			MoveStack* moveStackList, const Position& pos
		) {
			static_assert(MT == Capture || MT == NonCapture || MT == CapturePlusPro || MT == NonCaptureMinusPro, "");
			// Txxx は先手、後手の情報を吸収した変数。数字は先手に合わせている。
			const Rank TRank6 = (us == Black ? Rank6 : Rank4);
			const Rank TRank7 = (us == Black ? Rank7 : Rank3);
			const Rank TRank8 = (us == Black ? Rank8 : Rank2);
			const Bitboard TRank789BB = g_inFrontMaskBb.GetInFrontMask(us, TRank6);
			const Bitboard TRank1_6BB = g_inFrontMaskBb.GetInFrontMask(UtilColor::OppositeColor(us), TRank7);
			const Bitboard TRank1_7BB = g_inFrontMaskBb.GetInFrontMask(UtilColor::OppositeColor(us), TRank8);

			const Bitboard targetPawn =
				(MT == Capture) ? pos.GetBbOf(UtilColor::OppositeColor(us)) :
				(MT == NonCapture) ? pos.GetEmptyBB() :
				(MT == CapturePlusPro) ? pos.GetBbOf(UtilColor::OppositeColor(us)) | (pos.GetOccupiedBB().NotThisAnd(TRank789BB)) :
				(MT == NonCaptureMinusPro) ? pos.GetOccupiedBB().NotThisAnd(TRank1_6BB) :
				Bitboard::CreateAllOneBB(); // error
			const Bitboard targetOther =
				(MT == Capture) ? pos.GetBbOf(UtilColor::OppositeColor(us)) :
				(MT == NonCapture) ? pos.GetEmptyBB() :
				(MT == CapturePlusPro) ? pos.GetBbOf(UtilColor::OppositeColor(us)) :
				(MT == NonCaptureMinusPro) ? pos.GetEmptyBB() :
				Bitboard::CreateAllOneBB(); // error
			const Square ksq = pos.GetKingSquare(UtilColor::OppositeColor(us));

			moveStackList = GeneratePieceMoves_N01_Pawn()(MT, us, ALL, moveStackList, pos, targetPawn, ksq);
			moveStackList = GeneratePieceMoves_N02_Lance()(MT, us, ALL, moveStackList, pos, targetOther, ksq);
			moveStackList = GeneratePieceMoves_N03_Knight()(MT, us, ALL, moveStackList, pos, targetOther, ksq);
			moveStackList = GeneratePieceMoves_N04_Silver()(MT, us, ALL, moveStackList, pos, targetOther, ksq);
			moveStackList = GeneratePieceMoves_N05_Bishop()(MT, us, ALL, moveStackList, pos, targetOther, ksq);
			moveStackList = GeneratePieceMoves_N06_Rook()(MT, us, ALL, moveStackList, pos, targetOther, ksq);
			moveStackList = GeneratePieceMoves_pt()(N16_GoldHorseDragon, MT, us, ALL, moveStackList, pos, targetOther, ksq);
			moveStackList = GeneratePieceMoves_N08_King()(MT, us, ALL, moveStackList, pos, targetOther, ksq);

			return moveStackList;
		}
	};

	// 部分特殊化
	// 駒打ち生成
	template <>
	struct GenerateMoves<Drop> {
		FORCE_INLINE MoveStack* operator () (
			Color us,
			MoveStack* pMovestack, const Position& pos
		) {
			const Bitboard target = pos.GetEmptyBB();
			pMovestack = DropMoveGenerator::GenerateDropMoves(us, pMovestack, pos, target);//<US>
			return pMovestack;
		}
	};


	// 部分特殊化
	// 王手回避生成
	// 王手をしている駒による王手は避けるが、
	// 玉の移動先に敵の利きがある場合と、pinされている味方の駒を動かした場合、非合法手を生成する。
	// そのため、pseudo legal である。
	template <bool ALL>
	struct GenerateMoves<Evasion, ALL> {
		/*FORCE_INLINE*/ MoveStack* operator () (
			Color us,
			MoveStack* pMovestack,
			const Position& pos
		) {
			assert(pos.IsOK());
			assert(pos.InCheck());

			const Square ksq = pos.GetKingSquare(us);

			// 相手の色☆
			const Color Them = UtilColor::OppositeColor(us);

			const Bitboard checkers = pos.GetCheckersBB();
			Bitboard bb = checkers;
			Bitboard bannedKingToBB = Bitboard::CreateAllZeroBB();
			int checkersNum = 0;
			Square checkSq;

			// 玉が逃げられない位置の bitboard を生成する。
			// 絶対に王手が掛かっているので、while ではなく、do while
			do {

				// 最初にビットが立っていたマス☆
				checkSq = bb.PopFirstOneFromI9();

				assert(UtilPiece::ToColor(pos.GetPiece(checkSq)) == Them);

				// カウントアップ☆
				++checkersNum;


				// checkSq にある駒で王手されたとき、玉はその駒の利きの位置には移動できないので、
				// 移動できない位置を bannnedKingToBB に格納する。
				// 両王手のときには二度連続で呼ばれるため、= ではなく |= を使用している。
				// 最初に呼ばれたときは、bannedKingToBB == allZeroBB() である。
				// todo: FOECE_INLINE と template 省いてNPS比較
				//PieceArray::m_pieceAbstractArray[pos.GetPiece(checkSq)].MakeBanned2KingTo(bannedKingToBB, pos, checkSq, ksq);
				//*
				// 白か黒かによって分けている☆
				(Them == Black ?
					BannedKingToMaker::DoMakeBannedKingTo_black(bannedKingToBB, pos, checkSq, ksq) :
					BannedKingToMaker::DoMakeBannedKingTo_white(bannedKingToBB, pos, checkSq, ksq)
					);
				//*/

			} while (bb.Exists1Bit());


			// 玉が移動出来る移動先を格納。
			bb = bannedKingToBB.NotThisAnd(pos.GetBbOf(us).NotThisAnd(g_kingAttackBb.GetControllBb(ksq)));
			while (bb.Exists1Bit()) {
				const Square to = bb.PopFirstOneFromI9();
				// 移動先に相手駒の利きがあるか調べずに指し手を生成する。
				// attackersTo() が重いので、movePicker か search で合法手か調べる。
				(*pMovestack++).m_move = g_makePromoteMove.MakeNonPromoteMove<Capture>(N08_King, ksq, to, pos);
			}

			// 両王手なら、玉を移動するしか回避方法は無い。
			// 玉の移動は生成したので、ここで終了
			if (1 < checkersNum) {
				return pMovestack;
			}

			// 王手している駒を玉以外で取る手の生成。
			// pin されているかどうかは movePicker か search で調べる。
			const Bitboard target1 = g_betweenBb.GetBetweenBB(checkSq, ksq);
			const Bitboard target2 = target1 | checkers;
			pMovestack = GeneratePieceMoves_N01_Pawn()(Evasion, us, ALL, pMovestack, pos, target2, ksq);
			pMovestack = GeneratePieceMoves_N02_Lance()(Evasion, us, ALL, pMovestack, pos, target2, ksq);
			pMovestack = GeneratePieceMoves_N03_Knight()(Evasion, us, ALL, pMovestack, pos, target2, ksq);
			pMovestack = GeneratePieceMoves_N04_Silver()(Evasion, us, ALL, pMovestack, pos, target2, ksq);
			pMovestack = GeneratePieceMoves_N05_Bishop()(Evasion, us, ALL, pMovestack, pos, target2, ksq);
			pMovestack = GeneratePieceMoves_N06_Rook()(Evasion, us, ALL, pMovestack, pos, target2, ksq);
			pMovestack = GeneratePieceMoves_pt()(N16_GoldHorseDragon, Evasion, us, ALL, pMovestack, pos, target2, ksq);

			if (target1.Exists1Bit()) {
				pMovestack = DropMoveGenerator::GenerateDropMoves(us, pMovestack, pos, target1);//<US>
			}

			return pMovestack;
		}
	};

	// 部分特殊化
	// 王手が掛かっていないときの指し手生成
	// これには、玉が相手駒の利きのある地点に移動する自殺手と、pin されている駒を動かす自殺手を含む。
	// ここで生成した手は pseudo legal
	template <>
	struct GenerateMoves<NonEvasion> {
		/*FORCE_INLINE*/ MoveStack* operator () (
			Color us,
			MoveStack* pMovestack, const Position& pos
		) {
			Bitboard target = pos.GetEmptyBB();

			pMovestack = DropMoveGenerator::GenerateDropMoves(us, pMovestack, pos, target);//<US>
			target |= pos.GetBbOf(UtilColor::OppositeColor(us));
			const Square ksq = pos.GetKingSquare(UtilColor::OppositeColor(us));

			pMovestack = GeneratePieceMoves_N01_Pawn()(NonEvasion, us, false, pMovestack, pos, target, ksq);
			pMovestack = GeneratePieceMoves_N02_Lance()(NonEvasion, us, false, pMovestack, pos, target, ksq);
			pMovestack = GeneratePieceMoves_N03_Knight()(NonEvasion, us, false, pMovestack, pos, target, ksq);
			pMovestack = GeneratePieceMoves_N04_Silver()(NonEvasion, us, false, pMovestack, pos, target, ksq);
			pMovestack = GeneratePieceMoves_N05_Bishop()(NonEvasion, us, false, pMovestack, pos, target, ksq);
			pMovestack = GeneratePieceMoves_N06_Rook()(NonEvasion, us, false, pMovestack, pos, target, ksq);
			pMovestack = GeneratePieceMoves_pt()(N16_GoldHorseDragon, NonEvasion, us, false, pMovestack, pos, target, ksq);
			pMovestack = GeneratePieceMoves_N08_King()(NonEvasion, us, false, pMovestack, pos, target, ksq);

			return pMovestack;
		}
	};

	// 部分特殊化
	// 連続王手の千日手以外の反則手を排除した合法手生成
	// そんなに速度が要求されるところでは呼ばない。
	template <>
	struct GenerateMoves<Legal> {
		FORCE_INLINE MoveStack* operator () (
			Color us,
			MoveStack* moveStackList, const Position& pos
		) {
			MoveStack* curr = moveStackList;
			const Bitboard pinned = pos.GetPinnedBB();

			moveStackList = pos.InCheck() ?
				GenerateMoves<Evasion>()(us, moveStackList, pos) :
				GenerateMoves<NonEvasion>()(us, moveStackList, pos);

			// 玉の移動による自殺手と、pinされている駒の移動による自殺手を削除
			while (curr != moveStackList) {
				if (!pos.IsPseudoLegalMoveIsLegal<false, false>(curr->m_move, pinned)) {
					curr->m_move = (--moveStackList)->m_move;
				}
				else {
					++curr;
				}
			}

			return moveStackList;
		}
	};

	// 部分特殊化
	// Evasion のときに歩、飛、角と、香の2段目の不成も生成する。
	static MoveStack* GenerateMoves_LegalAll(
		Color us,
		MoveStack* moveStackList, const Position& pos
	) {
		MoveStack* curr = moveStackList;
		const Bitboard pinned = pos.GetPinnedBB();

		moveStackList = pos.InCheck() ?
			GenerateMoves<Evasion, true>()(us, moveStackList, pos) :
			GenerateMoves<NonEvasion>()(us,moveStackList, pos);

		// 玉の移動による自殺手と、pinされている駒の移動による自殺手を削除
		while (curr != moveStackList) {
			if (!pos.IsPseudoLegalMoveIsLegal<false, false>(curr->m_move, pinned)) {
				curr->m_move = (--moveStackList)->m_move;
			}
			else {
				++curr;
			}
		}

		return moveStackList;
	}


};

