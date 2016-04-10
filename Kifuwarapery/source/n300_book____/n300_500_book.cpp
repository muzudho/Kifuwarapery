#include <algorithm>	//std::max
#include <sstream>
#include "../../header/n110_square__/n110_100_square.hpp"
#include "../../header/n113_piece___/n113_150_piece.hpp"
#include "../../header/n113_piece___/n113_200_handPiece.hpp"
#include "../../header/n119_score___/n119_090_score.hpp"
#include "../../header/n220_position/n220_600_position.hpp"
#include "../../header/n280_move____/n280_500_mt64bit.hpp"
#include "../../header/n300_book____/n300_400_bookEntry.hpp"
#include "../../header/n300_book____/n300_500_book.hpp"
#include "../../header/n400_usi_____/n400_260_usiOperation.hpp"


#include "../../header/n900_main____/n900_200_searcher.hpp"		//TODO:1 これを外すのがむずかしい。
//class Searcher;


MT64bit Book::m_mt64bit_; // 定跡のhash生成用なので、seedは固定でデフォルト値を使う。
Key Book::m_ZobPiece[PieceNone][SquareNum];
Key Book::m_ZobHand[HandPieceNum][19]; // 持ち駒の同一種類の駒の数ごと
Key Book::m_ZobTurn;


void Book::Init() {
	for (Piece p = Empty; p < PieceNone; ++p) {
		for (Square sq = I9; sq < SquareNum; ++sq) {
			m_ZobPiece[p][sq] = m_mt64bit_.GetRandom();
		}
	}
	for (HandPiece hp = HPawn; hp < HandPieceNum; ++hp) {
		for (int num = 0; num < 19; ++num) {
			m_ZobHand[hp][num] = m_mt64bit_.GetRandom();
		}
	}
	m_ZobTurn = m_mt64bit_.GetRandom();
}

bool Book::Open(const char* fName) {
	m_fileName_ = "";

	if (is_open()) {
		close();
	}

	std::ifstream::open(fName, std::ifstream::in | std::ifstream::binary | std::ios::ate);

	if (!is_open()) {
		return false;
	}

	m_size_ = tellg() / sizeof(BookEntry);

	if (!good()) {
		std::cerr << "Failed to open book file " << fName  << std::endl;
		exit(EXIT_FAILURE);
	}

	m_fileName_ = fName;
	return true;
}

void Book::Binary_search(const Key key) {
	size_t low = 0;
	size_t high = m_size_ - 1;
	size_t mid;
	BookEntry entry;

	while (low < high && good()) {
		mid = (low + high) / 2;

		assert(mid >= low && mid < high);

		// std::ios_base::beg はストリームの開始位置を指す。
		// よって、ファイルの開始位置から mid * sizeof(BookEntry) バイト進んだ位置を指す。
		seekg(mid * sizeof(BookEntry), std::ios_base::beg);
		read(reinterpret_cast<char*>(&entry), sizeof(entry));

		if (key <= entry.m_key) {
			high = mid;
		}
		else {
			low = mid + 1;
		}
	}

	assert(low == high);

	seekg(low * sizeof(BookEntry), std::ios_base::beg);
}

Key Book::GetBookKey(const Position& pos) {
	Key key = 0;
	Bitboard bb = pos.GetOccupiedBB();

	while (bb.Exists1Bit()) {
		const Square sq = bb.PopFirstOneFromI9();
		key ^= m_ZobPiece[pos.GetPiece(sq)][sq];
	}
	const Hand hand = pos.GetHand(pos.GetTurn());
	for (HandPiece hp = HPawn; hp < HandPieceNum; ++hp) {
		key ^= m_ZobHand[hp][hand.NumOf(hp)];
	}
	if (pos.GetTurn() == White) {
		key ^= m_ZobTurn;
	}
	return key;
}

MoveScore Book::GetProbe(const Position& position, const std::string& fName, const bool pickBest) {
	BookEntry entry;
	u16 best = 0;
	u32 sum = 0;
	Move move = Move::GetMoveNone();
	const Key key = this->GetBookKey(position);
	const Score min_book_score = static_cast<Score>(static_cast<int>(position.GetSearcher()->m_engineOptions["Min_Book_Score"]));
	Score score = ScoreZero;

	if (m_fileName_ != fName && !Open(fName.c_str())) {
		return MoveScore(Move::GetMoveNone(), ScoreNone);
	}

	Binary_search(key);

	// 現在の局面における定跡手の数だけループする。
	while (read(reinterpret_cast<char*>(&entry), sizeof(entry)), entry.m_key == key && good()) {
		best = std::max(best, entry.m_count);
		sum += entry.m_count;

		// 指された確率に従って手が選択される。
		// count が大きい順に並んでいる必要はない。
		if (min_book_score <= entry.m_score
			&& ((m_random_.GetRandom() % sum < entry.m_count)
				|| (pickBest && entry.m_count == best)))
		{
			const Move tmp = Move(entry.m_fromToPro);
			const Square to = tmp.To();
			if (tmp.IsDrop()) {
				const PieceType ptDropped = tmp.GetPieceTypeDropped();
				move = UtilMove::MakeDropMove(ptDropped, to);
			}
			else {
				const Square from = tmp.From();
				const PieceType ptFrom = UtilPiece::ToPieceType(position.GetPiece(from));
				const bool promo = tmp.IsPromotion();
				if (promo) {
					move = UtilMove::MakeCapturePromoteMove(ptFrom, from, to, position);
				}
				else {
					move = UtilMove::MakeCaptureMove(ptFrom, from, to, position);
				}
			}
			score = entry.m_score;
		}
	}

	return MoveScore(move, score);
}

inline bool countCompare(const BookEntry& b1, const BookEntry& b2) {
	return b1.m_count < b2.m_count;
}

#if !defined MINIMUL
// 以下のようなフォーマットが入力される。
// <棋譜番号> <日付> <先手名> <後手名> <0:引き分け, 1:先手勝ち, 2:後手勝ち> <総手数> <棋戦名前> <戦形>
// <CSA1行形式の指し手>
//
// (例)
// 1 2003/09/08 羽生善治 谷川浩司 2 126 王位戦 その他の戦型
// 7776FU3334FU2726FU4132KI
//
// 勝った方の手だけを定跡として使うこととする。
// 出現回数がそのまま定跡として使う確率となる。
// 基本的には棋譜を丁寧に選別した上で定跡を作る必要がある。
// MAKE_SEARCHED_BOOK を on にしていると、定跡生成に非常に時間が掛かる。
void MakeBook(Position& pos, std::istringstream& ssCmd) {
	std::string fileName;
	ssCmd >> fileName;
	std::ifstream ifs(fileName.c_str(), std::ios::binary);
	if (!ifs) {
		std::cout << "I cannot open " << fileName << std::endl;
		return;
	}
	std::string line;
	std::map<Key, std::vector<BookEntry> > bookMap;

	while (std::getline(ifs, line)) {
		std::string elem;
		std::stringstream ss(line);
		ss >> elem; // 棋譜番号を飛ばす。
		ss >> elem; // 対局日を飛ばす。
		ss >> elem; // 先手
		const std::string sente = elem;
		ss >> elem; // 後手
		const std::string gote = elem;
		ss >> elem; // (0:引き分け,1:先手の勝ち,2:後手の勝ち)
		const Color winner = (elem == "1" ? Black : elem == "2" ? White : ColorNum);
		// 勝った方の指し手を記録していく。
		// 又は稲庭戦法側を記録していく。
		const Color saveColor = winner;

		if (!std::getline(ifs, line)) {
			std::cout << "!!! header only !!!" << std::endl;
			return;
		}
		pos.Set(g_DefaultStartPositionSFEN, pos.GetSearcher()->m_threads.GetMainThread());
		StateStackPtr SetUpStates = StateStackPtr(new std::stack<StateInfo>());
		UsiOperation usiOperation;
		while (!line.empty()) {
			const std::string moveStrCSA = line.substr(0, 6);
			const Move move = usiOperation.CsaToMove(pos, moveStrCSA);
			if (move.IsNone()) {
				pos.Print();
				std::cout << "!!! Illegal move = " << moveStrCSA << " !!!" << std::endl;
				break;
			}
			line.erase(0, 6); // 先頭から6文字削除
			if (pos.GetTurn() == saveColor) {
				// 先手、後手の内、片方だけを記録する。
				const Key key = Book::GetBookKey(pos);
				bool isFind = false;
				if (bookMap.find(key) != bookMap.end()) {
					for (std::vector<BookEntry>::iterator it = bookMap[key].begin();
						 it != bookMap[key].end();
						 ++it)
					{
						if (it->m_fromToPro == move.ProFromAndTo()) {
							++it->m_count;
							if (it->m_count < 1) {
								// 数えられる数の上限を超えたので元に戻す。
								--it->m_count;
							}
							isFind = true;
						}
					}
				}
				if (isFind == false) {
#if defined MAKE_SEARCHED_BOOK
					SetUpStates->push(StateInfo());
					pos.DoMove(move, SetUpStates->top());

					std::istringstream ssCmd("byoyomi 1000");
					UsiOperation usiOperation;
					usiOperation.Go(pos, ssCmd);
					pos.GetSearcher()->m_threads.WaitForThinkFinished();

					pos.UndoMove(move);
					SetUpStates->pop();

					// doMove してから search してるので点数が反転しているので直す。
					const Score score = -pos.GetCsearcher()->m_rootMoves[0].m_score_;
#else
					const Score GetScore = ScoreZero;
#endif
					// 未登録の手
					BookEntry be;
					be.m_score = score;
					be.m_key = key;
					be.m_fromToPro = static_cast<u16>(move.ProFromAndTo());
					be.m_count = 1;
					bookMap[key].push_back(be);
				}
			}
			SetUpStates->push(StateInfo());
			pos.DoMove(move, SetUpStates->top());
		}
	}

	// BookEntry::count の値で降順にソート
	for (auto& elem : bookMap) {
		std::sort(elem.second.rbegin(), elem.second.rend(), countCompare);
	}

#if 0
	// 2 回以上棋譜に出現していない手は削除する。
	for (auto& elem : bookMap) {
		auto& second = elem.second;
		auto erase_it = std::find_if(second.begin(), second.IsEnd(), [](decltype(*second.begin())& second_elem) { return second_elem.m_count < 2; });
		second.erase(erase_it, second.IsEnd());
	}
#endif

#if 0
	// narrow book
	for (auto& elem : bookMap) {
		auto& second = elem.second;
		auto erase_it = std::find_if(second.begin(), second.IsEnd(), [&](decltype(*second.begin())& second_elem) { return second_elem.m_count < second[0].m_count / 2; });
		second.erase(erase_it, second.IsEnd());
	}
#endif

	std::ofstream ofs("book.bin", std::ios::binary);
	for (auto& elem : bookMap) {
		for (auto& elel : elem.second) {
			ofs.write(reinterpret_cast<char*>(&(elel)), sizeof(BookEntry));
		}
	}

	std::cout << "book making was done" << std::endl;
}
#endif
