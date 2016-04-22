#include <algorithm>
#include "../../header/n480_tt______/n480_300_tt.hpp"

void TranspositionTable::SetSize(const size_t mbSize) { // Mega Byte 指定
	// 確保する要素数を取得する。
	size_t newSize = (mbSize << 20) / sizeof(TTCluster);
	newSize = std::max(static_cast<size_t>(1024), newSize); // 最小値は 1024 としておく。
	// 確保する要素数は 2 のべき乗である必要があるので、MSB以外を捨てる。
	const int msbIndex = 63 - firstOneFromMSB(static_cast<u64>(newSize));
	newSize = UINT64_C(1) << msbIndex;

	if (newSize == this->GetSize()) {
		// 現在と同じサイズなら何も変更する必要がない。
		return;
	}

	m_size_ = newSize;
	delete [] m_entries_;
	m_entries_ = new (std::nothrow) TTCluster[newSize];
	if (!m_entries_) {
		std::cerr << "Failed to allocate transposition table: " << mbSize << "MB";
		exit(EXIT_FAILURE);
	}
	Clear();
}

void TranspositionTable::Clear() {
	memset(m_entries_, 0, GetSize() * sizeof(TTCluster));
}

void TranspositionTable::Store(const Key posKey, const ScoreIndex score, const Bound bound, Depth depth,
							   Move move, const ScoreIndex evalScore)
{
	TTEntry* tte = FirstEntry(posKey);
	TTEntry* replace = tte;
	const u32 posKeyHigh32 = posKey >> 32;

	if (depth < Depth0) {
		depth = Depth0;
	}

	for (int i = 0; i < g_clusterSize; ++i, ++tte) {
		// 置換表が空か、keyが同じな古い情報が入っているとき
		if (!tte->GetKey() || tte->GetKey() == posKeyHigh32) {
			// move が無いなら、とりあえず古い情報でも良いので、他の指し手を保存する。
			if (move.IsNone()) {
				move = tte->GetMove();
			}

			tte->SetSave(depth, score, move, posKeyHigh32,
					  bound, this->GetGeneration(), evalScore);
			return;
		}

		int c = (replace->GetGeneration() == this->GetGeneration() ? 2 : 0);
		c    += (tte->GetGeneration() == this->GetGeneration() || tte->GetType() == BoundExact ? -2 : 0);
		c    += (tte->GetDepth() < replace->GetDepth() ? 1 : 0);

		if (0 < c) {
			replace = tte;
		}
	}
	replace->SetSave(depth, score, move, posKeyHigh32,
				  bound, this->GetGeneration(), evalScore);
}

TTEntry* TranspositionTable::Probe(const Key posKey) const {
	const Key posKeyHigh32 = posKey >> 32;
	TTEntry* tte = FirstEntry(posKey);

	// firstEntry() で、posKey の下位 (size() - 1) ビットを hash key に使用した。
	// ここでは posKey の上位 32bit が 保存されている hash key と同じか調べる。
	for (int i = 0; i < g_clusterSize; ++i, ++tte) {
		if (tte->GetKey() == posKeyHigh32) {
			return tte;
		}
	}
	return nullptr;
}
