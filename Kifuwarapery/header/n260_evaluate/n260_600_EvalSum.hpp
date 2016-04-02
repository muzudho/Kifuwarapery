#pragma once

#include "../../header/n260_evaluate/n260_500_evalStorage.hpp"


extern const int kppArray[31];
extern const int kkpArray[15];
extern const int kppHandArray[ColorNum][HandPieceNum];

struct EvalSum {
#if defined USE_AVX2_EVAL
	EvalSum(const EvalSum& es) {
		_mm256_store_si256(&mm, es.mm);
	}
	EvalSum& operator = (const EvalSum& rhs) {
		_mm256_store_si256(&mm, rhs.mm);
		return *this;
	}
#elif defined USE_SSE_EVAL
	EvalSum(const EvalSum& es) {
		_mm_store_si128(&m[0], es.m[0]);
		_mm_store_si128(&m[1], es.m[1]);
	}
	EvalSum& operator = (const EvalSum& rhs) {
		_mm_store_si128(&m[0], rhs.m[0]);
		_mm_store_si128(&m[1], rhs.m[1]);
		return *this;
	}
#endif
	EvalSum() {}
	s32 sum(const Color c) const {
		const s32 scoreBoard = p[0][0] - p[1][0] + p[2][0];
		const s32 scoreTurn = p[0][1] + p[1][1] + p[2][1];
		return (c == Black ? scoreBoard : -scoreBoard) + scoreTurn;
	}
	EvalSum& operator += (const EvalSum& rhs) {
#if defined USE_AVX2_EVAL
		mm = _mm256_add_epi32(mm, rhs.mm);
#elif defined USE_SSE_EVAL
		m[0] = _mm_add_epi32(m[0], rhs.m[0]);
		m[1] = _mm_add_epi32(m[1], rhs.m[1]);
#else
		p[0][0] += rhs.p[0][0];
		p[0][1] += rhs.p[0][1];
		p[1][0] += rhs.p[1][0];
		p[1][1] += rhs.p[1][1];
		p[2][0] += rhs.p[2][0];
		p[2][1] += rhs.p[2][1];
#endif
		return *this;
	}
	EvalSum& operator -= (const EvalSum& rhs) {
#if defined USE_AVX2_EVAL
		mm = _mm256_sub_epi32(mm, rhs.mm);
#elif defined USE_SSE_EVAL
		m[0] = _mm_sub_epi32(m[0], rhs.m[0]);
		m[1] = _mm_sub_epi32(m[1], rhs.m[1]);
#else
		p[0][0] -= rhs.p[0][0];
		p[0][1] -= rhs.p[0][1];
		p[1][0] -= rhs.p[1][0];
		p[1][1] -= rhs.p[1][1];
		p[2][0] -= rhs.p[2][0];
		p[2][1] -= rhs.p[2][1];
#endif
		return *this;
	}
	EvalSum operator + (const EvalSum& rhs) const { return EvalSum(*this) += rhs; }
	EvalSum operator - (const EvalSum& rhs) const { return EvalSum(*this) -= rhs; }

	// ehash 用。
	void encode() {
#if defined USE_AVX2_EVAL
		// EvalSum は atomic にコピーされるので key が合っていればデータも合っている。
#else
		key ^= data[0] ^ data[1] ^ data[2];
#endif
	}
	void decode() { encode(); }

	union {
		std::array<std::array<s32, 2>, 3> p;
		struct {
			u64 data[3];
			u64 key; // ehash用。
		};
#if defined USE_AVX2_EVAL
		__m256i mm;
#endif
#if defined USE_AVX2_EVAL || defined USE_SSE_EVAL
		__m128i m[2];
#endif
	};
};


