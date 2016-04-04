#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n223_move____/n223_105_utilMove.hpp"
#include "../n240_tt______/n240_285_tTCluster.hpp"


class TranspositionTable {
public:
	TranspositionTable();
	~TranspositionTable();
	void setSize(const size_t mbSize); // Mega Byte 指定
	void clear();
	void store(const Key posKey, const Score score, const Bound bound, Depth depth,
			   Move move, const Score evalScore);
	TTEntry* probe(const Key posKey) const;
	void newSearch();
	TTEntry* firstEntry(const Key posKey) const;
	void refresh(const TTEntry* tte) const;

	size_t size() const { return size_; }
	TTCluster* entries() const { return entries_; }
	u8 generation() const { return generation_; }

private:
	TranspositionTable(const TranspositionTable&);
	TranspositionTable& operator = (const TranspositionTable&);

	size_t size_; // 置換表のバイト数。2のべき乗である必要がある。
	TTCluster* entries_;
	// iterative deepening していくとき、過去の探索で調べたものかを判定する。
	u8 generation_;
};

inline TranspositionTable::TranspositionTable()
	: size_(0), entries_(nullptr), generation_(0) {}

inline TranspositionTable::~TranspositionTable() {
	delete [] entries_;
}

inline TTEntry* TranspositionTable::firstEntry(const Key posKey) const {
	// (size() - 1) は置換表で使用するバイト数のマスク
	// posKey の下位 (size() - 1) ビットを hash key として使用。
	// ここで posKey の下位ビットの一致を確認。
	// posKey の上位32ビットとの一致は probe, store 内で確認するので、
	// ここでは下位32bit 以上が確認出来れば完璧。
	// 置換表のサイズを小さく指定しているときは下位32bit の一致は確認出来ないが、
	// 仕方ない。
	return entries_[posKey & (size() - 1)].m_data;
}

inline void TranspositionTable::refresh(const TTEntry* tte) const {
	const_cast<TTEntry*>(tte)->SetGeneration(this->generation());
}

inline void TranspositionTable::newSearch() {
	++generation_;
}

