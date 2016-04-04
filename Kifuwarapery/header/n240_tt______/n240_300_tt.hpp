#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n223_move____/n223_105_utilMove.hpp"
#include "../n240_tt______/n240_285_tTCluster.hpp"


class TranspositionTable {
public:

	TranspositionTable();

	~TranspositionTable();

	void SetSize(const size_t mbSize); // Mega Byte 指定

	void Clear();

	void Store(const Key posKey, const Score score, const Bound bound, Depth depth,
			   Move move, const Score evalScore);

	TTEntry* Probe(const Key posKey) const;

	void NewSearch();

	TTEntry* FirstEntry(const Key posKey) const;

	void Refresh(const TTEntry* tte) const;

	size_t GetSize() const { return m_size_; }

	TTCluster* GetEntries() const { return m_entries_; }

	u8 GetGeneration() const { return m_generation_; }

private:
	TranspositionTable(const TranspositionTable&);
	TranspositionTable& operator = (const TranspositionTable&);

	size_t m_size_; // 置換表のバイト数。2のべき乗である必要がある。
	TTCluster* m_entries_;
	// iterative deepening していくとき、過去の探索で調べたものかを判定する。
	u8 m_generation_;
};

inline TranspositionTable::TranspositionTable()
	: m_size_(0), m_entries_(nullptr), m_generation_(0) {}

inline TranspositionTable::~TranspositionTable() {
	delete [] m_entries_;
}

inline TTEntry* TranspositionTable::FirstEntry(const Key posKey) const {
	// (size() - 1) は置換表で使用するバイト数のマスク
	// posKey の下位 (size() - 1) ビットを hash key として使用。
	// ここで posKey の下位ビットの一致を確認。
	// posKey の上位32ビットとの一致は probe, store 内で確認するので、
	// ここでは下位32bit 以上が確認出来れば完璧。
	// 置換表のサイズを小さく指定しているときは下位32bit の一致は確認出来ないが、
	// 仕方ない。
	return m_entries_[posKey & (GetSize() - 1)].m_data;
}

inline void TranspositionTable::Refresh(const TTEntry* tte) const {
	const_cast<TTEntry*>(tte)->SetGeneration(this->GetGeneration());
}

inline void TranspositionTable::NewSearch() {
	++m_generation_;
}

