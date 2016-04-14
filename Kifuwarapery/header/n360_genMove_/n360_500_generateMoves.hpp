#pragma once

#include "../n105_color___/n105_500_utilColor.hpp"
#include "../n110_square__/n110_250_squareDelta.hpp"
#include "../n110_square__/n110_400_squareRelation.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_102_FileMaskBb.hpp"
#include "../n160_board___/n160_106_inFrontMaskBb.hpp"
#include "../n160_board___/n160_230_setMaskBb.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_600_bitboardAll.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n165_movStack/n165_600_utilMove.hpp"
#include "../n220_position/n220_640_utilAttack.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n220_position/n220_670_makePromoteMove.hpp"

#include "../n350_pieceTyp/n350_070_ptAbstract.hpp"
#include "../n350_pieceTyp/n350_100_ptOccupied.hpp"
#include "../n350_pieceTyp/n350_110_ptPawn.hpp"
#include "../n350_pieceTyp/n350_120_ptLance.hpp"
#include "../n350_pieceTyp/n350_130_ptKnight.hpp"
#include "../n350_pieceTyp/n350_140_ptSilver.hpp"
#include "../n350_pieceTyp/n350_150_ptBishop.hpp"
#include "../n350_pieceTyp/n350_160_ptRook.hpp"
#include "../n350_pieceTyp/n350_170_ptGold.hpp"
#include "../n350_pieceTyp/n350_180_ptKing.hpp"
#include "../n350_pieceTyp/n350_190_ptProPawn.hpp"
#include "../n350_pieceTyp/n350_200_ptProLance.hpp"
#include "../n350_pieceTyp/n350_210_ptProKnight.hpp"
#include "../n350_pieceTyp/n350_220_ptProSilver.hpp"
#include "../n350_pieceTyp/n350_230_ptHorse.hpp"
#include "../n350_pieceTyp/n350_240_ptDragon.hpp"
#include "../n350_pieceTyp/n350_500_ptArray.hpp"
#include "../n358_dropMake/n358_100_dropMakerHand0.hpp"
#include "../n358_dropMake/n358_110_dropMakerHand1.hpp"
#include "../n358_dropMake/n358_120_dropMakerHand2.hpp"
#include "../n358_dropMake/n358_130_dropMakerHand3.hpp"
#include "../n358_dropMake/n358_140_dropMakerHand4.hpp"
#include "../n358_dropMake/n358_150_dropMakerHand5.hpp"
#include "../n358_dropMake/n358_160_dropMakerHand6.hpp"
#include "../n358_dropMake/n358_500_dropMakerArray.hpp"
#include "../n360_genMove_/n360_500_generateMoves.hpp"


extern const InFrontMaskBb g_inFrontMaskBb;
extern SquareRelation g_squareRelation;


namespace {
	// �p, ��Ԃ̏ꍇ
	template <MoveType MT, PieceType PT, Color US, bool ALL>
	FORCE_INLINE MoveStack* generateBishopOrRookMoves(
		MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square /*ksq*/
		)
	{
		Bitboard fromBB = pos.GetBbOf(PT, US);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			const bool fromCanPromote = UtilSquare::CanPromote(US, UtilSquare::ToRank(from));
			Bitboard toBB = g_ptArray[PT]->GetAttacks2From(pos.GetOccupiedBB(), US, from) & target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				const bool toCanPromote = UtilSquare::CanPromote(US, UtilSquare::ToRank(to));
				if (fromCanPromote | toCanPromote) {
					(*moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2<MT>(PT, from, to, pos);
					if (MT == NonEvasion || ALL)
						(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(PT, from, to, pos);
				}
				else // �p�A��Ԃ͐����Ȃ琬��A�s���͐������Ȃ��B
					(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(PT, from, to, pos);
			}
		}
		return moveStackList;
	}

	// ��ł��̏ꍇ
	// ���ȊO�̎�����́Aloop �̑O�Ɏ�����̎�ނ̐��ɂ���� switch �œW�J���Ă���B
	// ���[�v�̓W�J�̓R�[�h���c��オ�鎖�ɂ��L���b�V���q�b�g���̒ቺ�ƁA���Z�񐔂̃o�����X������Č��߂�K�v������B
	// NPS�ɉe�����o�Ȃ��Ȃ�V���v���ɂ��������ǂ������B
	template <Color US>
	MoveStack* generateDropMoves(
		MoveStack* moveStackList, const Position& pos, const Bitboard& target
		) {
		const Hand hand = pos.GetHand(US);
		// �܂��A���ɑ΂��Ďw����𐶐�
		if (hand.Exists<HPawn>()) {
			Bitboard toBB = target;
			// ��i�ڂɂ͑łĂȂ�
			const Rank TRank9 = (US == Black ? Rank9 : Rank1);
			toBB.AndEqualNot(g_rankMaskBb.GetRankMask<TRank9>());

			// ����̉��
			Bitboard pawnsBB = pos.GetBbOf(N01_Pawn, US);
			Square pawnsSquare;
			foreachBB(pawnsBB, pawnsSquare, [&](const int part) {
				toBB.SetP(part, toBB.GetP(part) & ~g_fileMaskBb.GetSquareFileMask(pawnsSquare).GetP(part));
			});

			// �ł����l�߂̉��
			const Rank TRank1 = (US == Black ? Rank1 : Rank9);
			const SquareDelta TDeltaS = (US == Black ? DeltaS : DeltaN);

			const Square ksq = pos.GetKingSquare(UtilColor::OppositeColor(US));
			// ����ʂ���i�ڂȂ�A���ŉ���o���Ȃ��̂ŁA�ł����l�߂𒲂ׂ�K�v�͂Ȃ��B
			if (UtilSquare::ToRank(ksq) != TRank1) {
				const Square pawnDropCheckSquare = ksq + TDeltaS;
				assert(UtilSquare::ContainsOf(pawnDropCheckSquare));
				if (g_setMaskBb.IsSet(&toBB, pawnDropCheckSquare) && pos.GetPiece(pawnDropCheckSquare) == Empty) {
					if (!pos.IsPawnDropCheckMate(US, pawnDropCheckSquare)) {
						// ������ clearBit �������� MakeMove ���Ȃ����Ƃ��o����B
						// �w���肪��������鏇�Ԃ��ς��A���肪��ɐ�������邪�A��Ŗ��ɂȂ�Ȃ���?
						(*moveStackList++).m_move = UtilMove::MakeDropMove(N01_Pawn, pawnDropCheckSquare);
					}
					g_setMaskBb.XorBit(&toBB, pawnDropCheckSquare);
				}
			}

			Square to;
			FOREACH_BB(toBB, to, {
				(*moveStackList++).m_move = UtilMove::MakeDropMove(N01_Pawn, to);
			});
		}

		// �� �ȊO�̋�������Ă��邩
		if (hand.ExceptPawnExists()) {
			PieceType haveHand[6]; // ���ȊO�̎�����Bvector �g���������ǁA���x�����߂�̂Ŏg��Ȃ��B
			int haveHandNum = 0; // ������̋�̎�ނ̐�

								 // �j�n�A���ԁA����ȊO�̏��ԂŊi�[����B(���łĂ�ʒu������I�ȏ�)
			if (hand.Exists<HKnight>()) { haveHand[haveHandNum++] = N03_Knight; }
			const int noKnightIdx = haveHandNum; // �j�n��������Ń��[�v����Ƃ��̃��[�v�̏����l
			if (hand.Exists<HLance >()) { haveHand[haveHandNum++] = N02_Lance; }
			const int noKnightLanceIdx = haveHandNum; // �j�n, ���Ԃ�������Ń��[�v����Ƃ��̃��[�v�̏����l
			if (hand.Exists<HSilver>()) { haveHand[haveHandNum++] = N04_Silver; }
			if (hand.Exists<HGold  >()) { haveHand[haveHandNum++] = N07_Gold; }
			if (hand.Exists<HBishop>()) { haveHand[haveHandNum++] = N05_Bishop; }
			if (hand.Exists<HRook  >()) { haveHand[haveHandNum++] = N06_Rook; }

			const Rank TRank8 = (US == Black ? Rank8 : Rank2);
			const Rank TRank9 = (US == Black ? Rank9 : Rank1);
			const Bitboard TRank8BB = g_rankMaskBb.GetRankMask<TRank8>();
			const Bitboard TRank9BB = g_rankMaskBb.GetRankMask<TRank9>();

			Bitboard toBB;
			// �j�n�A���� �ȊO�̎��������΁A
			// ��i�ڂɑ΂��āA�j�n�A���ԈȊO�̎w����𐶐��B
			// FIXME: �z��͈̔̓`�F�b�N���ĂȂ������i�O���O�j
			toBB = target & TRank9BB;
			g_dropMakerArray[haveHandNum - noKnightLanceIdx]->MakeDropMovesToRank9ExceptNL( toBB, moveStackList, haveHand, noKnightLanceIdx);

			// �j�n�ȊO�̎��������΁A
			// ��i�ڂɑ΂��āA�j�n�ȊO�̎w����𐶐��B
			toBB = target & TRank8BB;
			g_dropMakerArray[haveHandNum - noKnightIdx]->MakeDropMovesToRank8ExceptN( toBB, moveStackList, haveHand, noKnightIdx);

			// ��A��i�ڈȊO�ɑ΂��āA�S�Ă̎�����̎w����𐶐��B
			toBB = target & ~(TRank8BB | TRank9BB);
			g_dropMakerArray[haveHandNum]->MakeDropMovesToRank1234567( toBB, moveStackList, haveHand);
		}

		return moveStackList;
	}

	// ��, ������A�n�A���̎w���萶��
	template <MoveType MT, PieceType PT, Color US, bool ALL> struct GeneratePieceMoves {
		FORCE_INLINE MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square /*ksq*/
			) {
			static_assert(PT == N16_GoldHorseDragon, "");
			// ���A�����A�n�A����bitboard���܂Ƃ߂Ĉ����B
			Bitboard fromBB = (pos.GetGoldsBB() | pos.GetBbOf(N13_Horse, N14_Dragon)) & pos.GetBbOf(US);
			while (fromBB.Exists1Bit()) {
				const Square from = fromBB.PopFirstOneFromI9();
				// from �ɂ����̎�ނ𔻕�
				const PieceType pt = UtilPiece::ToPieceType(pos.GetPiece(from));
				Bitboard toBB = UtilAttack::GetAttacksFrom(pt, US, from, pos.GetOccupiedBB()) & target;
				while (toBB.Exists1Bit()) {
					const Square to = toBB.PopFirstOneFromI9();
					(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(pt, from, to, pos);
				}
			}
			return moveStackList;
		}
	};
	// ���̏ꍇ
	template <MoveType MT, Color US, bool ALL>
	struct GeneratePieceMoves<MT, N01_Pawn, US, ALL> {

		FORCE_INLINE MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square /*ksq*/
			) {
			// Txxx �͐��A���̏����z�������ϐ��B�����͐��ɍ��킹�Ă���B
			const Rank TRank6 = (US == Black ? Rank6 : Rank4);
			const Bitboard TRank789BB = g_inFrontMaskBb.GetInFrontMask<US, TRank6>();
			const SquareDelta TDeltaS = (US == Black ? DeltaS : DeltaN);

			Bitboard toBB = pos.GetBbOf(N01_Pawn, US).PawnAttack<US>() & target;

			// ����
			if (MT != NonCaptureMinusPro) {
				Bitboard toOn789BB = toBB & TRank789BB;
				if (toOn789BB.Exists1Bit()) {

					toBB.AndEqualNot(TRank789BB);

					Square to;

					FOREACH_BB(toOn789BB, to, {
						const Square from = to + TDeltaS;
					(*moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2<MT>(N01_Pawn, from, to, pos);
					if (MT == NonEvasion || ALL) {
						const Rank TRank9 = (US == Black ? Rank9 : Rank1);
						if (UtilSquare::ToRank(to) != TRank9) {
							(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(N01_Pawn, from, to, pos);
						}
					}
					});
				}
			}
			else {
				assert(!(target & TRank789BB).Exists1Bit());
			}

			// �c��(�s��)
			// toBB �� 8~4 �i�ڂ܂ŁB
			Square to;
			FOREACH_BB(toBB, to, {
				const Square from = to + TDeltaS;
			(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(N01_Pawn, from, to, pos);
			});

			return moveStackList;
		}//���Z�q�̃I�[�o�[���[�h�̒�`�H
	};//struct

	  // ���Ԃ̏ꍇ
	template <MoveType MT, Color US, bool ALL> struct GeneratePieceMoves<MT, N02_Lance, US, ALL> {
		FORCE_INLINE MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square /*ksq*/
			) {
			Bitboard fromBB = pos.GetBbOf(N02_Lance, US);
			while (fromBB.Exists1Bit()) {
				const Square from = fromBB.PopFirstOneFromI9();
				Bitboard toBB = g_ptLance.GetAttacks2From(pos.GetOccupiedBB(), US, from) & target;
				do {
					if (toBB.Exists1Bit()) {
						// ����Ώۂ͕K����ȉ��Ȃ̂ŁAtoBB �̃r�b�g�� 0 �ɂ���K�v���Ȃ��B
						const Square to = (MT == Capture || MT == CapturePlusPro ? toBB.GetFirstOneFromI9() : toBB.PopFirstOneFromI9());
						const bool toCanPromote = UtilSquare::CanPromote(US, UtilSquare::ToRank(to));
						if (toCanPromote) {
							(*moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2<MT>(N02_Lance, from, to, pos);
							if (MT == NonEvasion || ALL) {
								if (UtilSquare::IsBehind<US, Rank9, Rank1>(UtilSquare::ToRank(to))) // 1�i�ڂ̕s���͏Ȃ�
									(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(N02_Lance, from, to, pos);
							}
							else if (MT != NonCapture && MT != NonCaptureMinusPro) { // ������Ȃ�3�i�ڂ̕s�����Ȃ�
								if (UtilSquare::IsBehind<US, Rank8, Rank2>(UtilSquare::ToRank(to))) // 2�i�ڂ̕s�����Ȃ�
									(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(N02_Lance, from, to, pos);
							}
						}
						else
							(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(N02_Lance, from, to, pos);
					}
					// ����Ώۂ͕K����ȉ��Ȃ̂ŁAloop �͕s�v�B�œK���� do while �������Ȃ�Ɨǂ��B
				} while (!(MT == Capture || MT == CapturePlusPro) && toBB.Exists1Bit());
			}
			return moveStackList;
		}
	};
	// �j�n�̏ꍇ
	template <MoveType MT, Color US, bool ALL> struct GeneratePieceMoves<MT, N03_Knight, US, ALL> {
		FORCE_INLINE MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square /*ksq*/
			) {
			Bitboard fromBB = pos.GetBbOf(N03_Knight, US);
			while (fromBB.Exists1Bit()) {
				const Square from = fromBB.PopFirstOneFromI9();
				Bitboard toBB = g_ptKnight.GetAttacks2From(g_nullBitboard, US, from) & target;
				while (toBB.Exists1Bit()) {
					const Square to = toBB.PopFirstOneFromI9();
					const bool toCanPromote = UtilSquare::CanPromote(US, UtilSquare::ToRank(to));
					if (toCanPromote) {
						(*moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2<MT>(N03_Knight, from, to, pos);
						if (UtilSquare::IsBehind<US, Rank8, Rank2>(UtilSquare::ToRank(to))) // 1, 2�i�ڂ̕s���͏Ȃ�
							(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(N03_Knight, from, to, pos);
					}
					else
						(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(N03_Knight, from, to, pos);
				}
			}
			return moveStackList;
		}
	};
	// ��̏ꍇ
	template <MoveType MT, Color US, bool ALL> struct GeneratePieceMoves<MT, N04_Silver, US, ALL> {
		FORCE_INLINE MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square /*ksq*/
			) {
			Bitboard fromBB = pos.GetBbOf(N04_Silver, US);
			while (fromBB.Exists1Bit()) {
				const Square from = fromBB.PopFirstOneFromI9();
				const bool fromCanPromote = UtilSquare::CanPromote(US, UtilSquare::ToRank(from));
				Bitboard toBB = g_ptSilver.GetAttacks2From(g_nullBitboard, US, from) & target;
				while (toBB.Exists1Bit()) {
					const Square to = toBB.PopFirstOneFromI9();
					const bool toCanPromote = UtilSquare::CanPromote(US, UtilSquare::ToRank(to));
					if (fromCanPromote | toCanPromote)
						(*moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2<MT>(N04_Silver, from, to, pos);
					(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(N04_Silver, from, to, pos);
				}
			}
			return moveStackList;
		}
	};
	template <MoveType MT, Color US, bool ALL> struct GeneratePieceMoves<MT, N05_Bishop, US, ALL> {
		FORCE_INLINE MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square ksq
			) {
			return generateBishopOrRookMoves<MT, N05_Bishop, US, ALL>(moveStackList, pos, target, ksq);
		}
	};
	template <MoveType MT, Color US, bool ALL> struct GeneratePieceMoves<MT, N06_Rook, US, ALL> {
		FORCE_INLINE MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square ksq
			) {
			return generateBishopOrRookMoves<MT, N06_Rook, US, ALL>(moveStackList, pos, target, ksq);
		}
	};
	// �ʂ̏ꍇ
	// �K���Տ�� 1 ���������邱�Ƃ�O��ɂ��邱�ƂŁAwhile ���[�v�� 1 �������č��������Ă���B
	template <MoveType MT, Color US, bool ALL> struct GeneratePieceMoves<MT, N08_King, US, ALL> {
		FORCE_INLINE MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square /*ksq*/
			) {
			const Square from = pos.GetKingSquare(US);
			Bitboard toBB = g_ptKing.GetAttacks2From(g_nullBitboard, US, from) & target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<MT>(N08_King, from, to, pos);
			}
			return moveStackList;
		}
	};

	// �w���萶�� functor
	// �e���v���[�g��������������A�������ꉻ�����������̂ŁA�֐��ł͂Ȃ��Astruct �ɂ����B
	// ALL == true �̂Ƃ��A���A��A�p�̕s���A����2�i�ڂ̕s���A����3�i�ڂ̋�����Ȃ��s������������B
	template <MoveType MT, Color US, bool ALL = false> struct GenerateMoves {
		MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos
			) {
			static_assert(MT == Capture || MT == NonCapture || MT == CapturePlusPro || MT == NonCaptureMinusPro, "");
			// Txxx �͐��A���̏����z�������ϐ��B�����͐��ɍ��킹�Ă���B
			const Rank TRank6 = (US == Black ? Rank6 : Rank4);
			const Rank TRank7 = (US == Black ? Rank7 : Rank3);
			const Rank TRank8 = (US == Black ? Rank8 : Rank2);
			const Bitboard TRank789BB = g_inFrontMaskBb.GetInFrontMask<US, TRank6>();
			const Bitboard TRank1_6BB = g_inFrontMaskBb.GetInFrontMask<UtilColor::OppositeColor(US), TRank7>();
			const Bitboard TRank1_7BB = g_inFrontMaskBb.GetInFrontMask<UtilColor::OppositeColor(US), TRank8>();

			const Bitboard targetPawn =
				(MT == Capture) ? pos.GetBbOf(UtilColor::OppositeColor(US)) :
				(MT == NonCapture) ? pos.GetEmptyBB() :
				(MT == CapturePlusPro) ? pos.GetBbOf(UtilColor::OppositeColor(US)) | (pos.GetOccupiedBB().NotThisAnd(TRank789BB)) :
				(MT == NonCaptureMinusPro) ? pos.GetOccupiedBB().NotThisAnd(TRank1_6BB) :
				Bitboard::CreateAllOneBB(); // error
			const Bitboard targetOther =
				(MT == Capture) ? pos.GetBbOf(UtilColor::OppositeColor(US)) :
				(MT == NonCapture) ? pos.GetEmptyBB() :
				(MT == CapturePlusPro) ? pos.GetBbOf(UtilColor::OppositeColor(US)) :
				(MT == NonCaptureMinusPro) ? pos.GetEmptyBB() :
				Bitboard::CreateAllOneBB(); // error
			const Square ksq = pos.GetKingSquare(UtilColor::OppositeColor(US));

			moveStackList = GeneratePieceMoves<MT, N01_Pawn, US, ALL>()(moveStackList, pos, targetPawn, ksq);
			moveStackList = GeneratePieceMoves<MT, N02_Lance, US, ALL>()(moveStackList, pos, targetOther, ksq);
			moveStackList = GeneratePieceMoves<MT, N03_Knight, US, ALL>()(moveStackList, pos, targetOther, ksq);
			moveStackList = GeneratePieceMoves<MT, N04_Silver, US, ALL>()(moveStackList, pos, targetOther, ksq);
			moveStackList = GeneratePieceMoves<MT, N05_Bishop, US, ALL>()(moveStackList, pos, targetOther, ksq);
			moveStackList = GeneratePieceMoves<MT, N06_Rook, US, ALL>()(moveStackList, pos, targetOther, ksq);
			moveStackList = GeneratePieceMoves<MT, N16_GoldHorseDragon, US, ALL>()(moveStackList, pos, targetOther, ksq);
			moveStackList = GeneratePieceMoves<MT, N08_King, US, ALL>()(moveStackList, pos, targetOther, ksq);

			return moveStackList;
		}
	};

	// �������ꉻ
	// ��ł�����
	template <Color US> struct GenerateMoves<Drop, US> {
		FORCE_INLINE MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos
			) {
			const Bitboard target = pos.GetEmptyBB();
			moveStackList = generateDropMoves<US>(moveStackList, pos, target);
			return moveStackList;
		}
	};

	// checkSq �ɂ����ŉ��肳�ꂽ�Ƃ��A�ʂ͂��̋�̗����̈ʒu�ɂ͈ړ��ł��Ȃ��̂ŁA�ړ��ł��Ȃ��ʒu�� bannnedKingToBB �Ɋi�[����B
	// ������̂Ƃ��ɂ͓�x�A���ŌĂ΂�邽�߁A= �ł͂Ȃ� |= ���g�p���Ă���B
	// �ŏ��ɌĂ΂ꂽ�Ƃ��́AbannedKingToBB == allZeroBB() �ł���B
	// todo: FOECE_INLINE �� template �Ȃ���NPS��r
	template <Color THEM>
	FORCE_INLINE void makeBannedKingTo(
		Bitboard& bannedKingToBB, const Position& pos, const Square checkSq, const Square ksq
		) {
		switch (pos.GetPiece(checkSq)) {
			//		case Empty: assert(false); break; // �œK���ׂ̈̃_�~�[
		case (THEM == Black ? BPawn : WPawn) :
		case (THEM == Black ? BKnight : WKnight) :
			// ���A�j�n�ŉ��肵���Ƃ��́A�ǂ��֓����Ă��A���̋�Ŏ���邱�Ƃ͂Ȃ��B
			// ����āA�����ł͉������Ȃ��B
			assert(
				pos.GetPiece(checkSq) == (THEM == Black ? BPawn : WPawn) ||
				pos.GetPiece(checkSq) == (THEM == Black ? BKnight : WKnight)
				);
			break;
		case (THEM == Black ? BLance : WLance) :
			bannedKingToBB |= g_lanceAttackBb.GetControllBbToEdge(THEM, checkSq);
			break;
		case (THEM == Black ? BSilver : WSilver) :
			bannedKingToBB |= g_silverAttackBb.GetControllBb(THEM, checkSq);
			break;
		case (THEM == Black ? BGold : WGold) :
		case (THEM == Black ? BProPawn : WProPawn) :
		case (THEM == Black ? BProLance : WProLance) :
		case (THEM == Black ? BProKnight : WProKnight) :
		case (THEM == Black ? BProSilver : WProSilver) :
			bannedKingToBB |= g_goldAttackBb.GetControllBb(THEM, checkSq);
			break;
		case (THEM == Black ? BBishop : WBishop) :
			bannedKingToBB |= g_bishopAttackBb.GetControllBbToEdge(checkSq);
			break;
		case (THEM == Black ? BHorse : WHorse) :
			bannedKingToBB |= g_horseAttackBb.GetControllBbToEdge(checkSq);
			break;
		case (THEM == Black ? BRook : WRook) :
			bannedKingToBB |= g_rookAttackBb.GetControllBbToEdge(checkSq);
			break;
		case (THEM == Black ? BDragon : WDragon) :
			if (g_squareRelation.GetSquareRelation(checkSq, ksq) & DirecDiag) {
				// �΂߂��牤�肵���Ƃ��́A�ʂ̈ړ���Ɖ��肵����̊Ԃɋ���邱�Ƃ�����̂ŁA
				// dragonAttackToEdge(checkSq) �͎g���Ȃ��B
				bannedKingToBB |= g_ptDragon.GetAttacks2From(pos.GetOccupiedBB(), Color::ColorNum, checkSq);
			}
			else {
				bannedKingToBB |= g_dragonAttackBb.GetControllBbToEdge(checkSq);
			}
			break;
		default:
			UNREACHABLE;
		}
	}

	// �������ꉻ
	// �����𐶐�
	// ��������Ă����ɂ�鉤��͔����邪�A
	// �ʂ̈ړ���ɓG�̗���������ꍇ�ƁApin����Ă��閡���̋�𓮂������ꍇ�A�񍇖@��𐶐�����B
	// ���̂��߁Apseudo legal �ł���B
	template <Color US, bool ALL> struct GenerateMoves<Evasion, US, ALL> {
		/*FORCE_INLINE*/ MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos
			) {
			assert(pos.IsOK());
			assert(pos.InCheck());

			const Square ksq = pos.GetKingSquare(US);
			const Color Them = UtilColor::OppositeColor(US);
			const Bitboard checkers = pos.GetCheckersBB();
			Bitboard bb = checkers;
			Bitboard bannedKingToBB = Bitboard::CreateAllZeroBB();
			int checkersNum = 0;
			Square checkSq;

			// �ʂ��������Ȃ��ʒu�� bitboard �𐶐�����B
			// ��΂ɉ��肪�|�����Ă���̂ŁAwhile �ł͂Ȃ��Ado while
			do {
				checkSq = bb.PopFirstOneFromI9();
				assert(UtilPiece::ToColor(pos.GetPiece(checkSq)) == Them);
				++checkersNum;
				makeBannedKingTo<Them>(bannedKingToBB, pos, checkSq, ksq);
			} while (bb.Exists1Bit());

			// �ʂ��ړ��o����ړ�����i�[�B
			bb = bannedKingToBB.NotThisAnd(pos.GetBbOf(US).NotThisAnd(g_kingAttackBb.GetControllBb(ksq)));
			while (bb.Exists1Bit()) {
				const Square to = bb.PopFirstOneFromI9();
				// �ړ���ɑ����̗��������邩���ׂ��Ɏw����𐶐�����B
				// attackersTo() ���d���̂ŁAmovePicker �� search �ō��@�肩���ׂ�B
				(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove<Capture>(N08_King, ksq, to, pos);
			}

			// ������Ȃ�A�ʂ��ړ����邵�������@�͖����B
			// �ʂ̈ړ��͐��������̂ŁA�����ŏI��
			if (1 < checkersNum) {
				return moveStackList;
			}

			// ���肵�Ă������ʈȊO�Ŏ���̐����B
			// pin ����Ă��邩�ǂ����� movePicker �� search �Œ��ׂ�B
			const Bitboard target1 = g_betweenBb.GetBetweenBB(checkSq, ksq);
			const Bitboard target2 = target1 | checkers;
			moveStackList = GeneratePieceMoves<Evasion, N01_Pawn, US, ALL>()(moveStackList, pos, target2, ksq);
			moveStackList = GeneratePieceMoves<Evasion, N02_Lance, US, ALL>()(moveStackList, pos, target2, ksq);
			moveStackList = GeneratePieceMoves<Evasion, N03_Knight, US, ALL>()(moveStackList, pos, target2, ksq);
			moveStackList = GeneratePieceMoves<Evasion, N04_Silver, US, ALL>()(moveStackList, pos, target2, ksq);
			moveStackList = GeneratePieceMoves<Evasion, N05_Bishop, US, ALL>()(moveStackList, pos, target2, ksq);
			moveStackList = GeneratePieceMoves<Evasion, N06_Rook, US, ALL>()(moveStackList, pos, target2, ksq);
			moveStackList = GeneratePieceMoves<Evasion, N16_GoldHorseDragon, US, ALL>()(moveStackList, pos, target2, ksq);

			if (target1.Exists1Bit()) {
				moveStackList = generateDropMoves<US>(moveStackList, pos, target1);
			}

			return moveStackList;
		}
	};

	// �������ꉻ
	// ���肪�|�����Ă��Ȃ��Ƃ��̎w���萶��
	// ����ɂ́A�ʂ������̗����̂���n�_�Ɉړ����鎩�E��ƁApin ����Ă����𓮂������E����܂ށB
	// �����Ő���������� pseudo legal
	template <Color US> struct GenerateMoves<NonEvasion, US> {
		/*FORCE_INLINE*/ MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos
			) {
			Bitboard target = pos.GetEmptyBB();

			moveStackList = generateDropMoves<US>(moveStackList, pos, target);
			target |= pos.GetBbOf(UtilColor::OppositeColor(US));
			const Square ksq = pos.GetKingSquare(UtilColor::OppositeColor(US));

			moveStackList = GeneratePieceMoves<NonEvasion, N01_Pawn, US, false>()(moveStackList, pos, target, ksq);
			moveStackList = GeneratePieceMoves<NonEvasion, N02_Lance, US, false>()(moveStackList, pos, target, ksq);
			moveStackList = GeneratePieceMoves<NonEvasion, N03_Knight, US, false>()(moveStackList, pos, target, ksq);
			moveStackList = GeneratePieceMoves<NonEvasion, N04_Silver, US, false>()(moveStackList, pos, target, ksq);
			moveStackList = GeneratePieceMoves<NonEvasion, N05_Bishop, US, false>()(moveStackList, pos, target, ksq);
			moveStackList = GeneratePieceMoves<NonEvasion, N06_Rook, US, false>()(moveStackList, pos, target, ksq);
			moveStackList = GeneratePieceMoves<NonEvasion, N16_GoldHorseDragon, US, false>()(moveStackList, pos, target, ksq);
			moveStackList = GeneratePieceMoves<NonEvasion, N08_King, US, false>()(moveStackList, pos, target, ksq);

			return moveStackList;
		}
	};

	// �������ꉻ
	// �A������̐����ȊO�̔������r���������@�萶��
	// ����Ȃɑ��x���v�������Ƃ���ł͌Ă΂Ȃ��B
	template <Color US> struct GenerateMoves<Legal, US> {
		FORCE_INLINE MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos
			) {
			MoveStack* curr = moveStackList;
			const Bitboard pinned = pos.GetPinnedBB();

			moveStackList = pos.InCheck() ?
				GenerateMoves<Evasion, US>()(moveStackList, pos) : GenerateMoves<NonEvasion, US>()(moveStackList, pos);

			// �ʂ̈ړ��ɂ�鎩�E��ƁApin����Ă����̈ړ��ɂ�鎩�E����폜
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

	// �������ꉻ
	// Evasion �̂Ƃ��ɕ��A��A�p�ƁA����2�i�ڂ̕s������������B
	template <Color US> struct GenerateMoves<LegalAll, US> {
		FORCE_INLINE MoveStack* operator () (
			MoveStack* moveStackList, const Position& pos
			) {
			MoveStack* curr = moveStackList;
			const Bitboard pinned = pos.GetPinnedBB();

			moveStackList = pos.InCheck() ?
				GenerateMoves<Evasion, US, true>()(moveStackList, pos) : GenerateMoves<NonEvasion, US>()(moveStackList, pos);

			// �ʂ̈ړ��ɂ�鎩�E��ƁApin����Ă����̈ړ��ɂ�鎩�E����폜
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
}

template <MoveType MT>
MoveStack* generateMoves(
	MoveStack* moveStackList, const Position& pos
	) {
	return (pos.GetTurn() == Black ?
		GenerateMoves<MT, Black>()(moveStackList, pos) : GenerateMoves<MT, White>()(moveStackList, pos));
}






namespace {

	// pin �͏Ȃ��Ȃ��B
	FORCE_INLINE MoveStack* generateRecaptureMoves(
		MoveStack* moveStackList, const Position& pos, const Square to, const Color us
		) {
		Bitboard fromBB = pos.GetAttackersTo(us, to, pos.GetOccupiedBB());
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			const PieceType pieceType = UtilPiece::ToPieceType(pos.GetPiece(from));

			// TODO: �z��̃��~�b�g�`�F�b�N�����ĂȂ������i�O���O�j
			g_ptArray[pieceType]->Generate2RecaptureMoves(moveStackList, pos, from, to, us);
		}
		return moveStackList;
	}
}


template <MoveType MT>
MoveStack* generateMoves(
	MoveStack* moveStackList, const Position& pos, const Square to
	) {
	return generateRecaptureMoves(moveStackList, pos, to, pos.GetTurn());
}


// �����I�ȃC���X�^���X��
// ���ꂪ�����ƁA���̃t�@�C������Ă񂾎��ɁA
// ���̂��������߂Ƀ����N�G���[�ɂȂ�B
// ���Ȃ݂ɁA���ꉻ���ꂽ�e���v���[�g�֐��́A�����I�ȃC���X�^���X���̕K�v�͂Ȃ��B
// ������ cpp �ɒu�����ƂŁA�R���p�C�����Ԃ̒Z�k���o����B
//template MoveStack* generateMoves<Capture           >(MoveStack* moveStackList, const Position& pos);
//template MoveStack* generateMoves<NonCapture        >(MoveStack* moveStackList, const Position& pos);
template MoveStack* generateMoves<Drop              >(MoveStack* moveStackList, const Position& pos);
template MoveStack* generateMoves<CapturePlusPro    >(MoveStack* moveStackList, const Position& pos);
template MoveStack* generateMoves<NonCaptureMinusPro>(MoveStack* moveStackList, const Position& pos);
template MoveStack* generateMoves<Evasion           >(MoveStack* moveStackList, const Position& pos);
template MoveStack* generateMoves<NonEvasion        >(MoveStack* moveStackList, const Position& pos);
template MoveStack* generateMoves<Legal             >(MoveStack* moveStackList, const Position& pos);
#if !defined NDEBUG || defined LEARN
template MoveStack* generateMoves<LegalAll          >(MoveStack* moveStackList, const Position& GetPos);
#endif
template MoveStack* generateMoves<Recapture         >(MoveStack* moveStackList, const Position& pos, const Square to);