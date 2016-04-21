#include <map>
#include "../../header/n165_movStack/n165_300_moveType.hpp"
#include "../../header/n165_movStack/n165_400_move.hpp"
#include "../../header/n165_movStack/n165_600_convMove.hpp"
#include "../../header/n220_position/n220_650_position.hpp"
#include "../../header/n220_position/n220_670_makePromoteMove.hpp"
#include "../../header/n407_moveGen_/n407_900_moveList.hpp"
#include "../../header/n720_usi_____/n720_260_usiOperation.hpp"
#include "../../header/n885_searcher/n885_500_rucksack.hpp"


class StringToPieceTypeCSA : public std::map<std::string, PieceType> {
public:
	StringToPieceTypeCSA() {
		(*this)["FU"] = N01_Pawn;
		(*this)["KY"] = N02_Lance;
		(*this)["KE"] = N03_Knight;
		(*this)["GI"] = N04_Silver;
		(*this)["KA"] = N05_Bishop;
		(*this)["HI"] = N06_Rook;
		(*this)["KI"] = N07_Gold;
		(*this)["OU"] = N08_King;
		(*this)["TO"] = N09_ProPawn;
		(*this)["NY"] = N10_ProLance;
		(*this)["NK"] = N11_ProKnight;
		(*this)["NG"] = N12_ProSilver;
		(*this)["UM"] = N13_Horse;
		(*this)["RY"] = N14_Dragon;
	}
	PieceType value(const std::string& str) const {
		return this->find(str)->second;
	}
	bool isLegalString(const std::string& str) const {
		return (this->find(str) != this->end());
	}
};
const StringToPieceTypeCSA g_stringToPieceTypeCSA;


// 考え始めるのはここ。
void UsiOperation::Go(const Position& pos, std::istringstream& ssCmd) {
	LimitsOfThinking limits;
	std::vector<Move> moves;
	std::string token;

	// go にも種類がある☆
	while (ssCmd >> token) {
		if      (token == "ponder"     ) { limits.m_ponder = true; }
		else if (token == "btime"      ) { ssCmd >> limits.m_time[Black]; }
		else if (token == "wtime"      ) { ssCmd >> limits.m_time[White]; }
		else if (token == "infinite"   ) { limits.m_infinite = true; }
		else if (token == "byoyomi" ||
				 token == "movetime"   ) {
			// btime wtime の後に byoyomi が来る前提になっているので良くない。
			ssCmd >> limits.m_moveTime;
			if (limits.m_moveTime != 0) { limits.m_moveTime -= pos.GetSearcher()->m_engineOptions["Byoyomi_Margin"]; }
		}
		else if (token == "depth"      ) { ssCmd >> limits.m_depth; }
		else if (token == "nodes"      ) { ssCmd >> limits.m_nodes; }
		else if (token == "searchmoves") {
			UsiOperation usiOperation;
			while (ssCmd >> token)
			{
				moves.push_back(usiOperation.UsiToMove(pos, token));
			}
		}
	}
	pos.GetSearcher()->m_ourMoves = moves;

	// 思考を開始☆
	pos.GetSearcher()->m_ownerHerosPub.StartThinking(pos, limits, moves);
}

#if defined LEARN
// 学習用。通常の go 呼び出しは文字列を扱って高コストなので、大量に探索の開始、終了を行う学習では別の呼び出し方にする。
void UsiOperation::Go(const Position& GetPos, const Ply GetDepth, const Move GetMove) {
	LimitsOfThinking m_limits;
	std::vector<Move> moves;
	m_limits.GetDepth = GetDepth;
	moves.push_back(GetMove);
	GetPos.GetSearcher()->m_ownerHerosPub.StartThinking(GetPos, m_limits, moves);
}
#endif



#if !defined NDEBUG
// for debug
Move UsiOperation::usiToMoveDebug(const Position& GetPos, const std::string& moveStr) {
	for (MoveList<N09_LegalAll> ml(GetPos); !ml.IsEnd(); ++ml) {
		if (moveStr == ml.GetMove().ToUSI()) {
			return ml.GetMove();
		}
	}
	return Move::GetMoveNone();
}


Move UsiOperation::csaToMoveDebug(const Position& GetPos, const std::string& moveStr) {
	for (MoveList<N09_LegalAll> ml(GetPos); !ml.IsEnd(); ++ml) {
		if (moveStr == ml.GetMove().ToCSA()) {
			return ml.GetMove();
		}
	}
	return Move::GetMoveNone();
}
#endif


Move UsiOperation::UsiToMove(const Position& pos, const std::string& moveStr) {
	const Move move = UsiToMoveBody(pos, moveStr);
	assert(move == this->usiToMoveDebug(pos, moveStr));
	return move;
}

Move UsiOperation::CsaToMove(const Position& pos, const std::string& moveStr) {
	const Move move = CsaToMoveBody(pos, moveStr);
	assert(move == this->csaToMoveDebug(pos, moveStr));
	return move;
}

void UsiOperation::SetPosition(Position& pos, std::istringstream& ssCmd) {
	std::string token;
	std::string sfen;

	ssCmd >> token;

	if (token == "startpos") {
		sfen = g_DefaultStartPositionSFEN;
		ssCmd >> token; // "moves" が入力されるはず。
	}
	else if (token == "sfen") {
		while (ssCmd >> token && token != "moves") {
			sfen += token + " ";
		}
	}
	else {
		return;
	}

	pos.Set(sfen, pos.GetSearcher()->m_ownerHerosPub.GetFirstCaptain());
	pos.GetSearcher()->m_setUpStates = StateStackPtr(new std::stack<StateInfo>());

	Ply currentPly = pos.GetGamePly();
	while (ssCmd >> token) {
		const Move move = this->UsiToMove(pos, token);
		if (move.IsNone()) break;
		pos.GetSearcher()->m_setUpStates->push(StateInfo());
		pos.DoMove(move, pos.GetSearcher()->m_setUpStates->top());
		++currentPly;
	}
	pos.SetStartPosPly(currentPly);
}

Move UsiOperation::UsiToMoveBody(const Position& pos, const std::string& moveStr) {
	Move move;
	if (g_charToPieceUSI.IsLegalChar(moveStr[0])) {
		// drop
		const PieceType ptTo = ConvPiece::TO_PIECE_TYPE10(g_charToPieceUSI.GetValue(moveStr[0]));
		if (moveStr[1] != '*') {
			return g_MOVE_NONE;
		}
		const File toFile = ConvFile::FROM_CHAR_USI10(moveStr[2]);
		const Rank toRank = ConvRank::FROM_CHAR_USI10(moveStr[3]);
		if (!ConvSquare::CONTAINS_OF20(toFile, toRank)) {
			return g_MOVE_NONE;
		}
		const Square to = ConvSquare::FROM_FILE_RANK10(toFile, toRank);
		move = ConvMove::Convert30_MakeDropMove(ptTo, to);
	}
	else {
		const File fromFile = ConvFile::FROM_CHAR_USI10(moveStr[0]);
		const Rank fromRank = ConvRank::FROM_CHAR_USI10(moveStr[1]);
		if (!ConvSquare::CONTAINS_OF20(fromFile, fromRank)) {
			return g_MOVE_NONE;
		}
		const Square from = ConvSquare::FROM_FILE_RANK10(fromFile, fromRank);
		const File toFile = ConvFile::FROM_CHAR_USI10(moveStr[2]);
		const Rank toRank = ConvRank::FROM_CHAR_USI10(moveStr[3]);
		if (!ConvSquare::CONTAINS_OF20(toFile, toRank)) {
			return g_MOVE_NONE;
		}
		const Square to = ConvSquare::FROM_FILE_RANK10(toFile, toRank);
		if (moveStr[4] == '\0') {
			move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_CaptureCategory(ConvPiece::TO_PIECE_TYPE10(pos.GetPiece(from)), from, to, pos);
		}
		else if (moveStr[4] == '+') {
			if (moveStr[5] != '\0') {
				return g_MOVE_NONE;
			}
			move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_CaptureCategory(ConvPiece::TO_PIECE_TYPE10(pos.GetPiece(from)), from, to, pos);
			MakePromoteMove::APPEND_PROMOTE_FLAG(move);//, N00_Capture, UtilPiece::ToPieceType(pos.GetPiece(from))
		}
		else {
			return g_MOVE_NONE;
		}
	}

	if (pos.MoveIsPseudoLegal(move, true)
		&& pos.IsPseudoLegalMoveIsLegal<false, false>(move, pos.GetPinnedBB()))
	{
		return move;
	}
	return g_MOVE_NONE;
}

Move UsiOperation::CsaToMoveBody(const Position& pos, const std::string& moveStr) {
	if (moveStr.size() != 6) {
		return g_MOVE_NONE;
	}
	const File toFile = ConvFile::FROM_CHAR_CSA10(moveStr[2]);
	const Rank toRank = ConvRank::FROM_CHAR_CSA10(moveStr[3]);
	if (!ConvSquare::CONTAINS_OF20(toFile, toRank)) {
		return g_MOVE_NONE;
	}
	const Square to = ConvSquare::FROM_FILE_RANK10(toFile, toRank);
	const std::string ptToString(moveStr.begin() + 4, moveStr.end());
	if (!g_stringToPieceTypeCSA.isLegalString(ptToString)) {
		return g_MOVE_NONE;
	}
	const PieceType ptTo = g_stringToPieceTypeCSA.value(ptToString);
	Move move;
	if (moveStr[0] == '0' && moveStr[1] == '0') {
		// drop
		move = ConvMove::Convert30_MakeDropMove(ptTo, to);
	}
	else {
		const File fromFile = ConvFile::FROM_CHAR_CSA10(moveStr[0]);
		const Rank fromRank = ConvRank::FROM_CHAR_CSA10(moveStr[1]);
		if (!ConvSquare::CONTAINS_OF20(fromFile, fromRank)) {
			return g_MOVE_NONE;
		}
		const Square from = ConvSquare::FROM_FILE_RANK10(fromFile, fromRank);
		PieceType ptFrom = ConvPiece::TO_PIECE_TYPE10(pos.GetPiece(from));
		if (ptFrom == ptTo) {
			// non promote
			move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_CaptureCategory(ptFrom, from, to, pos);
		}
		else if (ptFrom + PTPromote == ptTo) {
			// promote
			move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_CaptureCategory(ptFrom, from, to, pos);
			MakePromoteMove::APPEND_PROMOTE_FLAG(move);//, N00_Capture, ptFrom
		}
		else {
			return g_MOVE_NONE;
		}
	}

	if (pos.MoveIsPseudoLegal(move, true)
		&& pos.IsPseudoLegalMoveIsLegal<false, false>(move, pos.GetPinnedBB()))
	{
		return move;
	}
	return g_MOVE_NONE;
}

