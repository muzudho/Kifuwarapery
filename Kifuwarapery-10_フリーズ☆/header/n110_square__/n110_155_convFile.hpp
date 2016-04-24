#pragma once

#include "../n110_square__/n110_150_file.hpp"


class ConvFile {
public:
	// 左右変換
	static inline constexpr File INVERSE10(const File f) { return FileNum - 1 - f; }

	static inline bool CONTAINS_OF10(const File f) { return (0 <= f) && (f < FileNum); }

	static inline char TO_CHAR_USI10(const File f) { return '1' + f; }

	static inline char TO_CHAR_CSA10(const File f) { return '1' + f; }

	static inline File FROM_CHAR_CSA10(const char c) { return static_cast<File>(c - '1'); }

	static inline File FROM_CHAR_USI10(const char c) { return static_cast<File>(c - '1'); }

};